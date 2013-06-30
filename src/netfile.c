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
	nf->fileid = malloc(sizeof(char) * (MAX_ID_LEN + 1));
	nf->partid = malloc(sizeof(char) * (MAX_ID_LEN + 1));
	nf->path = malloc(sizeof(char) * (strlen(file_path) +1));

	strncpy(nf->fileid, file_id, MAX_ID_LEN);
	strncpy(nf->partid, part_id, MAX_ID_LEN);
	strcpy(nf->path, file_path); 

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
		fprintf(storage, "%s\n", nf->fileid);
		fprintf(storage, "%s\n", nf->partid);
		fprintf(storage, "%s\n", nf->path);
		num_iterated++;
	}
	return num_iterated;
}

nf_list* load(FILE* storage)
{
	nf_list* nflist = nf_list_new();
	char file_id[MAX_ID_LEN + 1];
	char part_id[MAX_ID_LEN + 1];
	char* fpath;
	int index;

	memset(file_id, 0, MAX_ID_LEN + 1);
	memset(part_id, 0, MAX_ID_LEN + 1);

	for (index = 0; !feof(storage); index++) {
		fgets(file_id, MAX_ID_LEN + 1, storage); 
		fgets(part_id, MAX_ID_LEN + 1, storage);
		fpath = read_line(storage);
		nf_list_insert(nflist, make_net_file(file_id, part_id, fpath), index);
	}
	return nflist;
}

nf_list* nfsplit(net_file* nf, int num_pieces)
{
	nf_list* nflist = nf_list_new();
	net_file* newnf;
	char* filename;
	FILE* fout;
	FILE* content;
	int index, wrote;
	size_t orig_size;

	/* Get the size of the file to split so we know how many bytes to
	 * write into each new part file.
	 */
	filename = path_to(nf);
	content = fopen(filename, "rb");
	if (!content) return NULL;
	orig_size = fcontent_length(content);

	/* Split the file up by writing equal-sized pieces into new files
	 * represented by the list of new net_file structs created.
	 */
	for (index = 0; index < num_pieces && !feof(content); index++) {
		newnf = make_net_file(nf->fileid, "", nf->path);
		snprintf(newnf->partid, MAX_ID_LEN, "%spart%d", nf->partid, index);
		free(filename);
		filename = path_to(newnf);
		fout = fopen(filename, "wb");
		if (!fout) {
			nf_list_free_with(nflist, free_net_file);
			return NULL;
		}
		for (wrote = 0; wrote <= orig_size / num_pieces && !feof(content); wrote++)
			fputc(fgetc(content), fout);
		fclose(fout);
		nf_list_insert(nflist, newnf, index);
	}
	free(filename);
	fclose(content);
	return nflist;
}

net_file* nfjoin(nf_list* nflist)
{
	nf_list_iterator iter = nf_list_iterate(nflist);
	net_file* newnf;
	net_file* first;
	char* filename;
	FILE* fout;
	FILE* fin;

	first = nf_list_get(nflist, 0);
	newnf = make_net_file(first->fileid, "", first->path);
	fout = fopen(filename=path_to(newnf), "ab");
	while (nf_list_next(nflist, &iter)) {
		free(filename);
		filename = path_to(nf_list_get_at(nflist, iter));
		fin = fopen(filename, "rb");
		while (!feof(fin))
			fputc(fgetc(fin), fout);
		fclose(fin);
	}
	free(filename);
	fclose(fout);
	return newnf;
}

char* path_to(net_file* nf)
{
	char *buf = malloc(snprintf(NULL, 0, "%s/%s%s", nf->path, nf->fileid, nf->partid) + 1);
	sprintf(buf, "%s/%s%s", nf->path, nf->fileid, nf->partid);
	return buf;
}
