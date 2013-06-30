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
	int len;

	net_file* nf = malloc(sizeof(net_file));
	nf->fileid = malloc(sizeof(char) * (MAX_ID_LEN + 1));
	nf->partid = malloc(sizeof(char) * (MAX_ID_LEN + 1));
	nf->path = malloc(sizeof(char) * (strlen(file_path) +1));

	strncpy(nf->fileid, file_id, MAX_ID_LEN);
	nf->fileid[MAX_ID_LEN] = '\0';
	len = strlen(nf->fileid);
	if (len && nf->fileid[len-1] == '\n') nf->fileid[len-1] = '\0';

	strncpy(nf->partid, part_id, MAX_ID_LEN);
	nf->partid[MAX_ID_LEN] = '\0';
	len = strlen(nf->partid);
	if (len && nf->partid[len-1] == '\n') nf->partid[len-1] = '\0';

	strcpy(nf->path, file_path); 
	len = strlen(nf->path);
	if (len && nf->path[len-1] == '\n') nf->path[len-1] = '\0';

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
	char *file_id, *part_id, *fpath;

	while (!feof(storage) && !ferror(storage)) {
		if (!(file_id = read_line(storage)) || !*file_id) break;
		if (!(part_id = read_line(storage))) break;
		if (!(fpath = read_line(storage)) || !*fpath) break;
		nf_list_insert(nflist, make_net_file(file_id, part_id, fpath), -1);
		free(file_id);
		free(part_id);
		free(fpath);
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
	int index;
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
		copyn(fout, content, orig_size/num_pieces);
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
	if (!fout) {
		free_net_file(newnf);
		return NULL;
	}
	while (nf_list_next(nflist, &iter)) {
		free(filename);
		filename = path_to(nf_list_get_at(nflist, iter));
		fin = fopen(filename, "rb");
		if (!fin) break;
		copy(fout, fin);
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
