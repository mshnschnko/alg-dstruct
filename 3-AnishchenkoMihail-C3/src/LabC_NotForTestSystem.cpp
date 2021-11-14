#include "LabC.h"

int NumLen(int num)
{
	int len = 0;
	while (num > 0)
	{
		len++;
		num /= 10;
	}
	return len;
}

void FreeGraph(Graph_t* graph)
{
	for (int i = 0; i < graph->vertexesCount; i++)
		free(graph->vertexesArray[i].neighbours);
	free(graph->vertexesArray);
	free(graph);
	return;
}

Queue_t* InitQueue(void)
{
	Queue_t* queue = NULL;
	queue = (Queue_t*)malloc(sizeof(Queue_t));
	if (!queue)
		return NULL;
	queue->back = NULL;
	queue->front = NULL;
	return queue;
}

void QueueDestroy(Queue_t* queue)
{
	if (queue->front != NULL)
	{
		Elem_t* tmp = queue->front->next, * p;
		free(queue->front);
		while (tmp != NULL)
		{
			p = tmp;
			tmp = p->next;
			free(p);
		}
	}
	free(queue);
}

int QueueIsEmpty(Queue_t* queue)
{
	if (queue->front)
		return FALSE;
	else
		return TRUE;
}

int Push(Queue_t* queue, int vertex)
{
	Elem_t* element = NULL;
	element = (Elem_t*)malloc(sizeof(Elem_t));
	if (!element)
		return 0;
	element->vertex = vertex;
	element->next = NULL;
	if (QueueIsEmpty(queue))
	{
		queue->back = element;
		queue->front = element;

	}
	else
	{
		queue->back->next = element;
		queue->back = element;
	}
	return 1;
}

//void Pop(Queue_t* queue)
//{
//	Elem_t* elemToFree = queue->front;
//	queue->front = queue->front->next;
//	free(elemToFree);
//	return;
//}

int Pop(Queue_t* queue)
{
	Elem_t* temp;
	if (QueueIsEmpty(queue))
		return 0;
	temp = queue->front;
	queue->front = queue->front->next;
	free(temp);
	return 1;
}

int TopFront(Queue_t* queue)
{
	return queue->front->vertex;
}

int TopBack(Queue_t* queue)
{
	return queue->back->vertex;
}

Graph_t* ReadGraphFromStream(FILE* stream)
{
	int i;

	char* countBuffer = NULL;
	countBuffer = (char*)malloc(MAX_LENGTH * sizeof(char));
	if (!countBuffer)
		return NULL;
	fgets(countBuffer, MAX_LENGTH, stream);
	Graph_t* graph = NULL;
	graph = (Graph_t*)malloc(sizeof(Graph_t));
	if (!graph)
		return NULL;
	graph->vertexesCount = atoi(countBuffer);
	free(countBuffer);
	if (!graph->vertexesCount)
	{
		free(graph);
		return NULL;
	}
	int maxLineLength = (NumLen(graph->vertexesCount - 1) + 1) * graph->vertexesCount + 1;
	graph->vertexesArray = NULL;
	graph->vertexesArray = (Vertex_t*)malloc(graph->vertexesCount * sizeof(Vertex_t));
	if (!graph->vertexesArray)
	{
		FreeGraph(graph);
		return NULL;
	}
	for (i = 0; i < graph->vertexesCount; i++)
	{
		graph->vertexesArray[i].neighbours = NULL;
		graph->vertexesArray[i].neighboursCount = 0;
	}
	char* lineBuffer = NULL;
	lineBuffer = (char*)malloc(maxLineLength * sizeof(char));
	if (!lineBuffer)
	{
		FreeGraph(graph);
		return NULL;
	}
	int currentVertex, neighbour, * temp, neighbourIdx;
	char* BufIter = NULL;
	for (i = 0; i < graph->vertexesCount; i++)
	{
		fgets(lineBuffer, maxLineLength, stream);
		BufIter = lineBuffer;
		sscanf(BufIter, "%d", &currentVertex);
		BufIter += (NumLen(currentVertex) + 1) * sizeof(char);
		while (sscanf(BufIter, "%d", &neighbour) > 0)
		{
			BufIter += NumLen(neighbour) + 1;
			temp = (int*)realloc(graph->vertexesArray[currentVertex].neighbours, ++graph->vertexesArray[currentVertex].neighboursCount * sizeof(int));
			if (!temp)
			{
				FreeGraph(graph);
				free(lineBuffer);
				return NULL;
			}
			graph->vertexesArray[currentVertex].neighbours = temp;
			temp = (int*)realloc(graph->vertexesArray[neighbour].neighbours, ++graph->vertexesArray[neighbour].neighboursCount * sizeof(int));
			if (!temp)
			{
				FreeGraph(graph);
				free(lineBuffer);
				return NULL;
			}
			graph->vertexesArray[neighbour].neighbours = temp;
			graph->vertexesArray[currentVertex].neighbours[graph->vertexesArray[currentVertex].neighboursCount - 1] = neighbour;
			graph->vertexesArray[neighbour].neighbours[graph->vertexesArray[neighbour].neighboursCount - 1] = currentVertex;
		}
	}
	free(lineBuffer);
	return graph;
}

int WidthTraversal(FILE* stream, Graph_t* graph)
{
	if (!graph)
		return FALSE;
	Queue_t* queue = InitQueue();
	if (!queue)
	{
		FreeGraph(graph);
		return FALSE;
	}
	int* use = (int*)malloc(graph->vertexesCount * sizeof(int));
	if (!use)
	{
		FreeGraph(graph);
		free(queue);
		return FALSE;
	}
	if (!Push(queue, 0))
	{
		FreeGraph(graph);
		if (QueueIsEmpty(queue))
			free(queue);
		else
			QueueDestroy(queue);
	}
	use[0] = TRUE;
	int i;
	while (!QueueIsEmpty(queue))
	{
		fprintf(stream, "%i ", TopFront(queue));
		for (i = 0; i < graph->vertexesArray[TopFront(queue)].neighboursCount; i++)
		{
			if (use[graph->vertexesArray[TopFront(queue)].neighbours[i]] != TRUE)
			{
				if (!Push(queue, graph->vertexesArray[TopFront(queue)].neighbours[i]))
				{
					FreeGraph(graph);
					if (QueueIsEmpty(queue))
						free(queue);
					else
						QueueDestroy(queue);
				}
				use[graph->vertexesArray[TopFront(queue)].neighbours[i]] = TRUE;
			}
		}
		if (!Pop(queue))
		{
			FreeGraph(graph);
			if (QueueIsEmpty(queue))
				free(queue);
			else
				QueueDestroy(queue);
		}
	}
	free(queue);
	free(use);
	return TRUE;
}