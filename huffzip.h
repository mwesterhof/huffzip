#ifndef HUFFZIP_H
#define HUFFZIP_H

#include <inttypes.h>

/* TODO: clearly define static functions and consider removing from H */

typedef uint8_t byte;

typedef struct {
    byte *bytes;
    int len;
} HuffByteSet;

typedef struct node {
    HuffByteSet byte_set;
    int count;
    struct node *sibling_left;
    struct node *sibling_right;
    struct node *parent;
    struct node *child_left;
    struct node *child_right;
} HuffNode;

typedef struct {
    HuffNode *nodes;
    int len;
} HuffNodes;

void get_histogram(byte*, int*);
HuffNodes get_nodes(byte*);
HuffNode *create_node(byte, int);
void destroy_nodes(HuffNodes*);
void add_node(HuffNodes*, HuffNode*);
void debug_nodes(HuffNodes);

#endif
