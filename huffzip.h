#ifndef HUFFZIP_H
#define HUFFZIP_H

#include <inttypes.h>

#define CHARSET 256

/* TODO: clearly define static functions and consider removing from H */

typedef uint8_t byte;
typedef enum {false, true} bool;

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

typedef struct {
    char bits[CHARSET];
    bool *bits_compiled;
    int length;
} byte_mapping;

void get_histogram(byte*, int*);
HuffNodes get_nodes(byte*);
HuffNode *get_node(HuffNodes*, int);
HuffNode *create_node(byte*, int);
void destroy_node(HuffNode*);
void destroy_nodes(HuffNodes*);
void destroy_map(byte_mapping*);
void add_node(HuffNodes*, HuffNode*);
void swap_nodes(HuffNodes*, int, int);
void sort_nodes_by_count(HuffNodes*);
void merge_nodes(HuffNodes*, int, int);
void get_route_to_byte(HuffNode*, byte, char*, char);
void debug_nodes(HuffNodes*);
void map_bytes(HuffNodes*, byte_mapping*);
void write_mapped_bits(char*, byte*, byte_mapping*);

#endif
