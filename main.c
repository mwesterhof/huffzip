#include <stdio.h>
#include <string.h>

#include "huffzip.h"
#include <stdlib.h>


int main(int argc, char *argv[]) {
    /* char *teststring = "Moves Like Jagger (metal cover by Leo Moracchioli)"; */
    /* char *teststring = "AAB"; */
    char *teststring = "BBBAAABBAAAABBBBAAA";
    byte_mapping mapping[CHARSET];
    HuffNodes nodes;


    nodes = get_nodes(teststring);
    debug_nodes(&nodes);
    map_bytes(&nodes, mapping);
    printf("map for \"A\": %s\n", mapping['A'].bits);
    printf("map for \"B\": %s\n", mapping['B'].bits);
    write_mapped_bits("result.bin", teststring, mapping);


    destroy_nodes(&nodes);
    destroy_map(mapping);
    return 0;
}
