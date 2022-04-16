#pragma once
#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False !(True)

typedef struct node {
    int size;
    int key[3];
    struct node* first;
    struct node* second;
    struct node* third;
    struct node* fourth;
    struct node* parent;
}node;

char find(int value, int* key, int size);
void swap(int* x, int* y);
void sort2(int* x, int* y);
void sort3(int* x, int* y, int* z);
void sortkeys(int size, int* key);
node* createnode(int value);
node* createnode2(int value, node* first, node* second, node* third, node* fourth, node* parent);
void insertkey(int value, node* treenode);
void removekey(int value, node* treenode);
void becomenode2(int value, int* key, node* treenode, node* first, node* second);
char isleaf(node* tree);
node* split(node* item);
node* insertnode(node* tree, int value);
node* search(node* tree, int value);
node* searchmin(node* tree);
node* merge(node* leaf);
node* redistribute(node* leaf);
node* fix(node* leaf);
node* removenode(node* tree, int value);
void treedestroy(node* t);
void printtree(node* tree);