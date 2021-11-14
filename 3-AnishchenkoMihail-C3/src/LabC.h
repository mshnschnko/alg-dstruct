#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 10

typedef struct Elem_t
{
	int vertex;
	struct Elem_t* next;
} Elem_t;

typedef struct
{
	Elem_t* front;
	Elem_t* back;
} Queue_t;

typedef struct
{
	int* neighbours;
	int neighboursCount;
} Vertex_t;

typedef struct
{
	int vertexesCount;
	Vertex_t* vertexesArray;
} Graph_t;

int NumLen(int num);
Graph_t* ReadGraphFromStream(FILE* stream);
void FreeGraph(Graph_t* graph);
int WidthTraversal(FILE* stream, Graph_t* graph);
Queue_t* InitQueue(void);
int QueueisEmpty(Queue_t* queue);
void Push(Queue_t* queue, int vertex);
void Pop(Queue_t* queue);
int TopFront(Queue_t* queue);
int TopBack(Queue_t* queue);

#ifdef __cplusplus
}
#endif