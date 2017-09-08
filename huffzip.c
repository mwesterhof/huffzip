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

HuffNode *create_node(byte *original, int count) {
    HuffNode *new_node = (HuffNode*)malloc(sizeof(HuffNode));
    memset(new_node, 0, sizeof(HuffNode));

    new_node->byte_set.bytes = (byte*)malloc(sizeof(byte) + 1); /* TODO: check this gets freed */
    strcpy(new_node->byte_set.bytes, original);
    new_node->byte_set.len = strlen(original);
    new_node->count = count;

    return new_node;
}

void add_node(HuffNodes *nodes, HuffNode *node) {
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

void swap_nodes(HuffNodes *nodes, int i, int j) {
    HuffNode *node_i, *node_j, *far_left, *far_right;
    int swap;

    if (i == j)
        return;

    /* normalize input */
    if (i > j) {
        swap = i;
        i = j;
        j = swap;
    }
    if (j - i > 1)
        return;

    /* fetch the 2 nodes in question */
    node_i = get_node(nodes, i);
    node_j = get_node(nodes, j);

    /* also fetch the surrounding nodes (if any) */
    far_left = node_i->sibling_left;
    far_right = node_j->sibling_right;

    /* swap the pointers around */
    node_i->sibling_left = node_j;
    node_i->sibling_right = far_right;
    node_j->sibling_left = far_left;
    node_j->sibling_right = node_i;

    if (far_left)
        far_left->sibling_right = node_j;
    if (far_right)
        far_right->sibling_left = node_i;
    if (i == 0)
        nodes->nodes = node_j;
}

HuffNode *get_node(HuffNodes *nodes, int target) {
    HuffNode *current = nodes->nodes;
    for (int i=0; i<target; ++i) {
        current = current->sibling_right;
    }
    return current;
}

void sort_nodes_by_count(HuffNodes *nodes) {
    /* lame inefficient sorting algorithm, pulled straight out of my $#* */
    bool swap_done = true;
    int i=0, j=0;
    HuffNode *current, *next;

    while (swap_done) {
        current = &(nodes->nodes[0]);
        swap_done = false;
        for (i=0; i<nodes->len-1; ++i) {
            j = i + 1;
            next = current->sibling_right;
            if (current->count > next->count) {
                swap_nodes(nodes, i, j);
                swap_done = true;
            }
            current = next;
        }
    }
}


void merge_nodes(HuffNodes *nodes, int i, int j) {
    /* let's get multi-dimensional
     * merges 2 nodes together, adding their bytesets and counts,
     * adding it into the nodeset, to replace the original 2 nodes,
     * and letting it reference to these as it's children */
    HuffNode *node_i, *node_j, *parent, *far_left, *far_right;
    int swap, len;
    byte *buffer;

    /* normalize data */
    if (i > j) {
        swap = i;
        i = j;
        j = swap;
    }
    if (j - i != 1)
        return;

    node_i = get_node(nodes, i);
    node_j = get_node(nodes, j);

    far_left = node_i->sibling_left;
    far_right = node_j->sibling_right;

    len = (node_i->byte_set.len + node_j->byte_set.len) * sizeof(byte);
    buffer = malloc(len);
    memset(buffer, 0, len);
    memcpy(
        buffer,
        node_i->byte_set.bytes,
        node_i->byte_set.len*sizeof(byte)
    );
    memcpy(
        buffer+node_i->byte_set.len*sizeof(byte),
        node_j->byte_set.bytes,
        node_j->byte_set.len*sizeof(byte)
    );

    parent = create_node(buffer, node_i->count + node_j->count);
    /* set parent attributes */
    parent->sibling_left = node_i->sibling_left;
    parent->sibling_right = node_j->sibling_right;
    parent->parent = NULL;
    parent->child_left = node_i;
    parent->child_right = node_j;

    /* set node_i attributes */
    node_i->sibling_left = NULL;
    node_i->parent = parent;

    /* set node_j attributes */
    node_j->sibling_right = NULL;
    node_j->parent = parent;

    /* connect parent to outer nodes */
    if (far_left)
        far_left->sibling_right = parent;
    if (far_right)
        far_right->sibling_left = parent;

    /* if node_i is the first node, the node set should know about this */
    if (!far_left)
        nodes->nodes = parent;
    --nodes->len;
}


HuffNodes get_nodes(byte *string) {
    int total = 0;
    int stats[256];

    HuffNodes nodes;
    HuffNode *new_node;
    byte bytes[2];
    memset(&nodes, 0, sizeof(HuffNodes));
    memset(bytes, 0, 2);
    nodes.len = 0;

    get_histogram(string, stats);

    // nodes.nodes = (HuffNode*)malloc(sizeof(HuffNode) * total); /* TODO: check this gets freed */

    for (int i=0; i<256; ++i) {
        if (stats[i] > 0){
            printf("--creating node--, %c, %d\n", i, stats[i]);
            bytes[0] = (byte)i;
            new_node = create_node(bytes, stats[i]);
            add_node(&nodes, new_node);
        }
    }

    while (nodes.len != 1) {
        sort_nodes_by_count(&nodes);
        merge_nodes(&nodes, 0, 1);
    }

    return nodes;
}


void get_route_to_byte(HuffNode *node, byte target, char *buffer, char reference_bitvalue) {
    bool found=false;
    
    for (int i=0; i<node->byte_set.len && !found; ++i) {
        if (node->byte_set.bytes[i] == target) {
            found = true;
        }
    }
    if (!found) {
        return;
    }
    *buffer = reference_bitvalue;

    if (node->child_left) {
        get_route_to_byte(node->child_left, target, buffer+sizeof(byte), '0');
    }
    if (node->child_left) {
        get_route_to_byte(node->child_right, target, buffer+sizeof(byte), '1');
    }
}


void destroy_nodes(HuffNodes *nodes) {
    /* TODO: change to fancy recursive solution */
    for (int i=0; i<nodes->len; ++i) {
        free(nodes->nodes[i].byte_set.bytes);
    }
    free(nodes->nodes);
}


static void _debug_node(HuffNode *node, int level) {
    char *indent = (char*)malloc((level*2+1) * sizeof(char));
    for (int i=0; i<level; ++i)
        strcat(indent, "  ");
    printf("%sbytes: %s count: %d\n", indent, node->byte_set.bytes, node->count);
    if (node->child_left) {
        _debug_node(node->child_left, level+1);
        _debug_node(node->child_right, level+1);
    }
}


void debug_nodes(HuffNodes *nodes) {
    HuffNode *current = nodes->nodes;
    printf("---NODES---\n");
    while (current) {
        _debug_node(current, 0);
        current = current->sibling_right;
    }
}
