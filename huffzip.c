/* TODO: remove this include, it's just here for debug stuff */
#include <stdio.h>


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

HuffNode *create_node(byte original, int count) {
    HuffNode *new_node = (HuffNode*)malloc(sizeof(HuffNode));
    memset(new_node, 0, sizeof(HuffNode));

    new_node->byte_set.bytes = (byte*)malloc(sizeof(byte) + 1); /* TODO: check this gets freed */
    new_node->byte_set.bytes[0] = original;
    new_node->byte_set.bytes[1] = 0;
    new_node->byte_set.len = 1;
    new_node->count = count;

    return new_node;
}

void add_node(HuffNodes *nodes, HuffNode *node) {
    /* TODO: implement */
    HuffNode *current = NULL, *next = nodes->nodes;
    while (next != NULL) {
        current = next;
        next = next->sibling_right;
    }
    if (current == NULL)
        nodes->nodes = node;
    else {
        current->sibling_right = node;
        node->sibling_left = current;
    }
    nodes->len++;
}

HuffNodes get_nodes(byte *string) {
    int total = 0;
    int stats[256];

    HuffNodes nodes;
    HuffNode *new_node;
    memset(&nodes, 0, sizeof(HuffNodes));
    nodes.len = 0;

    get_histogram(string, stats);

    // nodes.nodes = (HuffNode*)malloc(sizeof(HuffNode) * total); /* TODO: check this gets freed */

    for (int i=0; i<256; ++i) {
        if (stats[i] > 0){
            printf("--creating node--, %c, %d\n", i, stats[i]);
            new_node = create_node(i, stats[i]);
            add_node(&nodes, new_node);
        }
    }

    return nodes;
}

void destroy_nodes(HuffNodes *nodes) {
    /* TODO: change to fancy recursive solution */
    for (int i=0; i<nodes->len; ++i) {
        free(nodes->nodes[i].byte_set.bytes);
    }
    free(nodes->nodes);
}


void debug_nodes(HuffNodes nodes) {
    HuffNode *current = nodes.nodes;
    
    printf("len: %d\n", nodes.len);
    while(current) {
        printf("node [%s] (%d)\n", current->byte_set.bytes, current->count);
        current = current->sibling_right;
    }
}
