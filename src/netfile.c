#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "netfile.h"
#include "llist.h"
#include "util.h"
#include "constants.h"

LLIST(net_file*, nf_list);

net_file* make_net_file(char* file_id, char* part_id, char* file_path)
{
    net_file* nf = malloc(sizeof(net_file));
    nf->fileid = malloc(sizeof(char) * MAX_ID_LEN + 1);
    nf->partid = malloc(sizeof(char) * MAX_ID_LEN + 1);
    nf->path = malloc(sizeof(char) * MAX_PATH_LEN + 1);

    strlcpy(netf->fileid, file_id, strlen(file_id) + 1);
    strlcpy(netf->partid, part_id, strlen(part_id) + 1);
    strlcpy(netf->path, file_path, strlen(file_path) + 1);
    
    return nf;
}

void free_net_file(net_file* nf)
{
    free(nf->fileid);
    free(nf->partid);
    free(nf->path);
    free(nf);
}

int dump(nf_list* netfiles, FILE* storage)
{
    nf_list_iterator iter;
    int num_iterated = 0;
    for (iter = nf_list_iterate(netfiles); nf_list_next(netfiles, &iter); ) {
        net_file* nf = nf_list_get_at(netfiles, iter);
        fprintf(storage, "%s\0\n", nf->fileid);
        fprintf(storage, "%s\0\n", nf->partid);
        fprintf(storage, "%s\0\n\n", nf->path);
        num_iterated++;
    }
    return num_iterated;
}

nf_list* load(FILE* storage)
{
    nf_list* nflist = nf_list_new();
    char file_id[MAX_ID_LEN + 1];
    char part_id[MAX_ID_LEN + 1];
    char fpath[MAX_PATH_LEN + 1];
    int index;

    memset(file_id, 0, MAX_ID_LEN + 1);
    memset(part_id, 0, MAX_ID_LEN + 1);
    memset(fpath, 0, MAX_PATH_LEN + 1);

    for (index = 0; !feof(storage); index++) {
        fgets(file_id, MAX_ID_LEN + 2, storage); // Might be losing bytes for
        fgets(part_id, MAX_ID_LEN + 2, storage); // successive strings because
        fgets(fpath, MAX_PATH_LEN + 2, storage); // of the endline being read.
        nf_list_insert(nflist, make_net_file(file_id, part_id, fpath), index);
    }
    return nflist;
}

nf_list* nfsplit(net_file* nf, int num_pieces)
{
    nf_list* nflist = nf_list_new();
    net_file* newnf;
    char filename[2 * MAX_ID_LEN + MAX_PATH_LEN + 1];
    FILE* fout, content;
    int index, wrote;
    size_t orig_size;

    /* Get the size of the file to split so we know how many bytes to
     * write into each new part file.
     */
    memset(filename, 0, 2 * MAX_ID_LEN + MAX_PATH_LEN + 1);
    filename = path_to(nf, filename);
    content = fopen(filename, "rb");
    orig_size = fcontent_length(content);

    /* Split the file up by writing equal-sized pieces into new files
     * represented by the list of new net_file structs created.
     */
    for (index = 0; index < num_pieces && !feof(content); index++) {
        memset(filename, 0, 2 * MAX_ID_LEN + MAX_PATH_LEN + 1);
        newnf = make_net_file(nf->fileid, "", nf->path);
        snprintf(newnf->partid, MAX_ID_LEN, "%spart%d", nf->partid, index);
        filename = path_to(newnf, filename);
        fout = fopen(filename, "wb");
        if (!fout) {
            nf_list_free_with(nflist, free_new_file);
            return NULL;
        }
        for (wrote = 0; wrote <= orig_size / num_pieces && !feof(content); wrote++)
            fputc(fgetc(content), fout);
        fclose(fout);
        nf_list_insert(nflist, newnf, index);
    }
    fclose(content);
    return nflist;
}

net_file* nfjoin(nf_list* nflist)
{
    nf_list_iterator iter = nf_list_iterate(nflist);
    net_file* newnf, first;
    char filename[2 * MAX_ID_LEN + MAX_PATH_LEN + 1];
    char* lastpart;
    FILE* fout, fin;

    memset(filename, 0, 2 * MAX_ID_LEN + MAX_PATH_LEN + 1);
    first = nf_list_get(nflist, 0);
    newnf = make_net_file(first->fileid, "", first->path);
    fout = fopen(path_to(newnf, filename), "ab");
    while (nf_list_next(nflist, &iter)) {
        memset(filename, 0, 2 * MAX_ID_LEN + MAX_PATH_LEN + 1);
        filename = path_to(iter, filename);
        fin = fopen(filename, "rb");
        while (!feof(fin))
            fputc(fgetc(fin), fout);
        fclose(fin);
    }
    fclose(fout);
    return newnf;
}

char* path_to(net_file* nf, char* buffer)
{
    return snprintf(filename, 2 * MAX_ID_LEN + MAX_PATH_LEN, "%s/%s%s", 
                    nf->path, nf->fileid, nf->partid);
}
