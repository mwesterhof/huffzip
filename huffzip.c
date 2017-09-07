#include <stdlib.h>
#include <string.h>

#include "huffzip.h"


void get_histogram(byte *string, int *result) {
    memset(result, 0, 256 * sizeof(int));

    /* tally each byte in string */
    for (int i=0, len=strlen(string); i<len; i++) {
        result[string[i]]++;
    }
}


HuffNodes get_nodes(byte *string) {
    /* TODO */
    int total = 0;
    int stats[256];

    HuffNodes nodes;
    nodes.len = 0;

    get_histogram(string, stats);

    for (int i=0; i<256; ++i) {
        if (stats[i] > 0)
            ++total;
    }

    nodes.nodes = (HuffNode*)malloc(sizeof(HuffNode) * total); /* TODO: check this gets freed */
    for (int i=0, node_i=0; i<256; ++i) {
        if (stats[i] > 0){
            nodes.nodes[node_i].byte_set = create_byteset(i);
            nodes.nodes[node_i].count = 1;
            ++node_i;
            ++(nodes.len);
        }
    }

    return nodes;
}

HuffByteSet create_byteset(byte original) {
    HuffByteSet byteset;
    byteset.bytes = (byte*)malloc(sizeof(byte) + 1); /* TODO: check this gets freed */
    byteset.bytes[0] = original;
    byteset.bytes[1] = 0;
    byteset.len = 1;
    return byteset;
}

void destroy_nodes(HuffNodes *nodes) {
    for (int i=0; i<nodes->len; ++i) {
        free(nodes->nodes[i].byte_set.bytes);
    }
    free(nodes->nodes);
}
