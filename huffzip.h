#ifndef HUFFZIP_H
#define HUFFZIP_H

#include <inttypes.h>

typedef uint8_t byte;

typedef struct {
    byte *bytes;
    int len;
} HuffByteSet;

typedef struct node {
    HuffByteSet byte_set;
    int count;
} HuffNode;

typedef struct {
    HuffNode *nodes;
    int len;
} HuffNodes;

void get_histogram(byte*, int*);
HuffNodes get_nodes(byte*);
HuffByteSet create_byteset(byte);
void destroy_nodes(HuffNodes*);

#endif
