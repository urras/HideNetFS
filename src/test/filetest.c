#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "../netfile.h"
#include "../util.h"
#include "../constants.h"

int main(int argc, char** argv)
{
    net_file* generated;
    nf_list* netfiles;
    char* fid = malloc(sizeof(char) * (MAX_ID_LEN + 1));
    char* pid = malloc(sizeof(char) * (MAX_ID_LEN + 1));
    char* path = ".";
    int i;
    FILE* storage = fopen("netfiles.dat", "wb");

    srand(time(NULL));

    /* Create a net file to manipulate for testing. */
    fid = randstr(fid, MAX_ID_LEN);
	assert(fid);
    *pid = 0;
    rename("generateddata.txt", fid);
    generated = make_net_file(fid, pid, path);
	assert(generated);
    printf("Renamed generateddata.txt to %s and created net_file.\n", fid);

    /* Try to split, store, and recover the netfiles. */
    netfiles = nfsplit(generated, 2);
	assert(netfiles);
    printf("Split %s into 2 pieces.\n", fid);
    remove(fid);
    i = dump(netfiles, storage);
    printf("Dumped %d parts of %s into netfiles.dat.\n", i, fid);
    printf("Part 1: %s\n", path_to(nf_list_get(netfiles, 0), fid));
    printf("Part 2: %s\n", path_to(nf_list_get(netfiles, 1), fid));
    nf_list_free_with(netfiles, free_net_file);
    free_net_file(generated);
    printf("Freed both the list of split netfiles and generated netfile.\n");
    fclose(storage);
    storage = fopen("netfiles.dat", "rb");
    netfiles = load(storage);
    printf("Loaded netfile list from netfiles.dat.\n");
    generated = nfjoin(netfiles);
    printf("Rejoined netfiles into one.\n");

    /* Free resources and end. */
    fclose(storage);
    free(fid);
    free(pid);
    nf_list_free_with(netfiles, free_net_file);
    free_net_file(generated);
    printf("Resources freed. Ending.\n");
    return 0;
}
