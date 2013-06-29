#include "llist.h" 

#ifndef __NETFILE_H__
#define __NETFILE_H__

typedef struct net_file {
    char* fileid;
    char* partid;
    char* path;
};

LLIST_PROTO(net_file*, nf_list);

net_file* make_net_file(char* file_id, char* part_id, char* file_path);
void free_net_file(net_file* nf);
int dump(nf_list* netfiles, FILE* storage);
nf_list* load(FILE* storage);

#endif /* __NETFILE_H__ */
