#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int key;
    unsigned char height;
    struct node* left;
    struct node* right;
} node;

unsigned char height(node* p);
int bfactor(node* p);
void fixheight(node* p);
node* rotateright(node* p);
node* rotateleft(node* q);
node* balance(node* p);
node* insert(node* p, int k);
node* findmin(node* p);
node* removemin(node* p);
node* delete(node* p, int k);
void search(node* p, int k);
void treedestroy(node* p);
int lab();