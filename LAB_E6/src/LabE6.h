#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

#define RECURSION_ERROR 1
#define ORIENT_LEFT -1
#define ORIENT_RIGHT 1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tree_t
{
	struct tree_t* left;
	struct tree_t* right;
	char* name;
} tree_t;

typedef struct
{
	char* str;
	tree_t* tree;
} treePrint_t;

tree_t* TreeParser();
tree_t* ReadBinaryTree(const char* inputFileName);
int FillTreePrint(tree_t* root, int space, int len, int orient);
void FindMax(tree_t* tree);
void FindVertLines(tree_t* tree);
int PrintTree(tree_t* tree, FILE* output);
void FreeTree(tree_t* tree);
void FreePrint(int count);
int Solution(const char* inputFileName, const char* outputFileName);

#ifdef __cplusplus
}
#endif