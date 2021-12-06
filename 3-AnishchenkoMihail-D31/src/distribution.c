#pragma warning(disable: 4996)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned** table;
int* result;
int iter;

int CreateTable(unsigned T, unsigned D)
{
	table = NULL;
	int i, j;
	table = (unsigned**)malloc((T + 1) * sizeof(unsigned*));
	if (!table)
		return 0;
	for (i = 0; i < T + 1; i++)
	{
		table[i] = (unsigned*)malloc((D + 1) * sizeof(unsigned));
		if (!table[i])
		{
			for (j = 0; j < i; j++)
				free(table[j]);
			free(table);
			return 0;
		}
	}
	return 1;
}

void DestroyTable(unsigned n)
{
	int i;
	for (i = 0; i < n; i++)
		free(table[i]);
	free(table);
}

int PrintAnswer(unsigned* timeOfTasks, int k, int s)
{
	if (table[k][s] == 0)
		return -1;
	if (table[k - 1][s] == table[k][s])
		PrintAnswer(timeOfTasks, k - 1, s);
	else
	{
		PrintAnswer(timeOfTasks, k - 1, s - timeOfTasks[k - 1]);
		int* tmp = (int*)realloc(result, (iter + 1) * sizeof(int));
		if (!tmp)
			return;
		result = tmp;
		result[iter] = k;
		iter++;
	}
}

void PrintTable(unsigned tasks, unsigned fullTime)
{
	int i, j;
	for (i = 0; i <= tasks; i++)
	{
		for (j = 0; j <= fullTime; j++)
			printf("%u ", table[i][j]);
		printf("\n");
	}
}

void PrintResult(void)
{
	int i;
	for (i = 0; i < iter; i++)
	{
		if (result[i] != -1)
			printf("%u ", result[i]);
		else
			printf("\n");
	}
}

int Distribution(unsigned tasks, unsigned fullTime, unsigned staff, unsigned* timeOfTasks)
{
	if (!CreateTable(tasks, fullTime))
		return -1;
	int i, j;
	for (i = 0; i <= fullTime; i++)
		table[0][i] = 0;
	for (i = 0; i <= fullTime; i++)
		printf("%u ", table[0][i]);
	printf("\n");
	for (i = 0; i <= tasks; i++)
	{
		printf("%i", i);
		table[i][0] = 0;
	}
	printf("\n");

	
	for (i = 1; i <= tasks; i++)
	{
		for (j = 1; j <= fullTime; j++)
		{
			if (j >= timeOfTasks[i - 1])
				table[i][j] = table[i - 1][j] > table[i - 1][j - timeOfTasks[i - 1]] + timeOfTasks[i - 1] ? table[i - 1][j] : table[i - 1][j - timeOfTasks[i - 1]] + timeOfTasks[i - 1];
			else
				table[i][j] = table[i - 1][j];
		}
		PrintTable(tasks, fullTime);
	}
	PrintAnswer(timeOfTasks, i - 1, j - 1);
	int* tmp = (int*)realloc(result, (iter + 1) * sizeof(int));
	if (!tmp)
		return -1;
	result = tmp;
	result[iter] = -1;
	iter++;
	return 0;
}

int LabSolution(FILE* input)
{
	unsigned T, D, m;
	fscanf(input, "%u %u %u\n", &T, &D, &m);
	unsigned* timeOfTasks = NULL;
	timeOfTasks = (unsigned*)malloc(T * sizeof(unsigned));
	int i = 0;
	for (i = 0; i < T; i++)
		fscanf(input, "%u ", &timeOfTasks[i]);
	printf("%u %u %u\n", T, D, m);
	result = (int*)malloc(sizeof(int));
	if (!result)
	{
		free(timeOfTasks);
		return -1;
	}
	iter = 0;
	if (Distribution(T, D, m, timeOfTasks) == -1)
	{
		free(timeOfTasks);
		DestroyTable(T);
		printf("pizda");
		return -1;
	}
	PrintResult();
	free (timeOfTasks);
	DestroyTable(T);
	free(result);
	return 0;
}

int main(void)
{
	const char* inputFileName = "input.txt";
	FILE* input = fopen(inputFileName, "r");
	if (!input)
		return -1;
	return LabSolution(input);
}