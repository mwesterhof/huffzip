#include <stdio.h>
#include <string.h>

#include "huffzip.h"
#include <stdlib.h>


int main(int argc, char *argv[]) {
    char *teststring = "Moves Like Jagger (metal cover by Leo Moracchioli)";
    char *route = (char*)malloc(strlen(teststring));
    byte_mapping mapping[CHARSET];
    HuffNodes nodes;
    nodes = get_nodes(teststring);
    debug_nodes(&nodes);

    map_bytes(&nodes, mapping);
    printf("map for \"e\": %s\n", mapping['e'].bits);


    destroy_nodes(&nodes);
    destroy_map(mapping);
    free(route);
    return 0;
}
