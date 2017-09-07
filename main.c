#include <stdio.h>

#include "huffzip.h"
#include <stdlib.h>


int main(int argc, char *argv[]) {
    char *teststring = "APPLEPIE";
    HuffNodes nodes;
    nodes = get_nodes(teststring);
    debug_nodes(nodes);
    swap_nodes(&nodes, 0, 1);
    debug_nodes(nodes);
    /* destroy_nodes(&nodes); */
    return 0;
}
