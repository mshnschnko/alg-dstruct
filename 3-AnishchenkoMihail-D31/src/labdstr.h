#pragma once
#pragma warning(disable: 4996)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

	int CreateTable(unsigned T, unsigned D);
	void DestroyTable(unsigned n);
	int GetAnswer(unsigned* timeOfTasks, int* rowIndexes, int k, int s);
	void PrintResult(FILE* output);
	unsigned* FillRowIndexes(unsigned tasks);
	int Distribution(unsigned tasks, unsigned fullTime, unsigned* timeOfTasks);
	int LabSolution(const char* inputFileName, const char* outputFileName);

#ifdef __cplusplus
}
#endif