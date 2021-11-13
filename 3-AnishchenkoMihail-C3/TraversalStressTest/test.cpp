#include "pch.h"
#include "LabC.h"

/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10 Pro, Version 20H2, Build 19042.1288
PC configuration:
    CPU: Intel(R) Core(TM) i5-3230M CPU @ 2.60GHz   2.60 GHz
    Motherboard: ASUSTeK COMPUTER INC.  X75VC
    RAM: 12 GB, DDR3
    SSD: SSD M.2 256GB
*/

/*
Lab C, Variant 3 (breadth-first traversal with list of contiguous vertices):
Results:
    Graph creation time: 26854 ms ms
    Traversal of the graph time: 49188 ms
    Total run time: 76045 ms
    Memory Costs: About 224 MB
*/

const char* inputFilename = "Input.txt";
const int vertexesCount = 650000;

TEST(WidthTraversal_StressTest, GraphCreation)
{
    FILE* input = fopen(inputFilename, "w");
    if (!input)
    {
        printf("File didn't open for writting\n");
        ASSERT_TRUE(input);
    }
    fprintf(input, "%i\n", vertexesCount);
    for (int currentVertex = 0; currentVertex < vertexesCount - 1; currentVertex++)
    {
        fprintf(input, "%i ", currentVertex);
        for (int neighbour = currentVertex + 1 + rand() % (vertexesCount - 1); neighbour < vertexesCount; neighbour += 1 + rand() % (vertexesCount - 1))
            fprintf(input, "%i ", neighbour);
        fprintf(input, "\n");
    }
    fprintf(input, "%i \n", vertexesCount - 1);
    fclose(input);
}

TEST(WidthTraversal_StressTest, GraphTraversal)
{
    FILE* input = fopen(inputFilename, "r");
    if (!input)
    {
        printf("File input didn't open for reading\n");
        ASSERT_TRUE(input);
    }
    FILE* output = fopen("Output.txt", "w");
    if (!output)
    {
        printf("File Output didn't open for writting\n");
        fclose(input);
        ASSERT_TRUE(output);
    }
    Graph_t* graph = ReadGraphFromStream(input);
    if (!graph)
    {
        printf("Error in reading graph\n");
        fclose(input);
        fclose(output);
        ASSERT_TRUE(graph);
    }
    fclose(input);
    int result = WidthTraversal(output, graph);
    if (!result)
    {
        printf("Error in WidthTraversal\n");
        fclose(output);
        FreeGraph(graph);
        ASSERT_TRUE(result);
    }
    fclose(output);
}