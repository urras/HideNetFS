#include "llist.h" 

#ifndef __NETFILE_H__
#define __NETFILE_H__

typedef struct {
	char* fileid;
	char* partid;
	char* path;
} net_file;

LLIST_PROTO(net_file*, nf_list);

net_file* make_net_file(char* file_id, char* part_id, char* file_path);
void free_net_file(net_file* nf);
int dump(nf_list* netfiles, FILE* storage);
nf_list* load(FILE* storage);
nf_list* nfsplit(net_file* nf, int num_pieces);
net_file* nfjoin(nf_list* nflist);
char* path_to(net_file* nf);

#endif /* __NETFILE_H__ */
