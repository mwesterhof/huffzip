#include <stdio.h>
#include <string.h>

#include "huffzip.h"
#include <stdlib.h>


int main(int argc, char *argv[]) {
    char *teststring = "APPLEPIE";
    char *route = (char*)malloc(strlen(teststring));
    /* char *teststring = "Moves Like Jagger (metal cover by Leo Moracchioli)"; */
    HuffNodes nodes;
    nodes = get_nodes(teststring);
    debug_nodes(&nodes);
    get_route_to_byte(nodes.nodes, 'A', route, ' ');
    puts(route);
    /* destroy_nodes(&nodes); */
    return 0;
}
