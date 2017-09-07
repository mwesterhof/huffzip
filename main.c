#include <stdio.h>

#include "huffzip.h"
#include <stdlib.h>


int main(int argc, char *argv[]) {
    char *teststring = "APPLEPIE";
    HuffNodes nodes;
    nodes = get_nodes(teststring);
    destroy_nodes(&nodes);
    return 0;
}
