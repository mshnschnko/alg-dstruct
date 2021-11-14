#include "LabC.h"

int main()
{
	Graph_t* graph = ReadGraphFromStream(stdin);
	if (!graph)
		return -1;
	if (!WidthTraversal(stdout, graph))
	{
		FreeGraph(graph);
		return -1;
	}
	FreeGraph(graph);
	return 0;
}