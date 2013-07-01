#include <stdio.h>

#include "constants.h"
#include "netfile.h"

#include "hmap.h"

#ifndef __NETNODE_H__
#define __NETNODE_H__

/* Maps file ids to net_files. */
HMAP_PROTO(char*, net_file, directory);

/* Maps addresses to socket files. */
HMAP_PROTO(char*, FILE*, connections);

typedef struct {
    char *address;
    unsigned int last_ping;
    connections neighbours;
    directory stored_files;
} net_node;

#endif /* __NETNODE_H__ */
