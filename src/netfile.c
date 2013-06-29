#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "netfile.h"
#include "llist.h"
#include "constants.h"

LLIST(net_file*, nf_list);

net_file* make_net_file(char* file_id, char* part_id, char* file_path)
{
    size_t fid_len = strlen(file_id);
    size_t pid_len = strlen(part_id);
    size_t path_len = strlen(file_path);
    
    net_file* nf = malloc(sizeof(net_file));
    nf->fileid = malloc(sizeof(char) * (fid_len + 1));
    nf->partid = malloc(sizeof(char) * (pid_len + 1));
    nf->path = malloc(sizeof(char) * (path_len + 1));
    
    strlcpy(netf->fileid, file_id, fid_len + 1);
    strlcpy(netf->partid, part_id, pid_len + 1);
    strlcpy(netf->path, file_path, path_len + 1);
    
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
