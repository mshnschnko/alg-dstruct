#include "labdstr.h"

unsigned** table;
int* result;
int iter, countOfMinus;

int CreateTable(unsigned tasks, unsigned fullTime)
{
	table = NULL;
	int i, j;
	table = (unsigned**)malloc((tasks + 1) * sizeof(unsigned*));
	if (!table)
		return -1;
	for (i = 0; i < tasks + 1; i++)
	{
		table[i] = (unsigned*)malloc(fullTime * sizeof(unsigned));
		if (!table[i])
		{
			for (j = 0; j < i; j++)
				free(table[j]);
			free(table);
			return -1;
		}
	}
	return 0;
}

void DestroyTable(unsigned n)
{
	int i;
	for (i = 0; i < n; i++)
		free(table[i]);
	free(table);
}

int GetAnswer(unsigned* timeOfTasks, int* rowIndexes, int k, int s)
{
	if (s < 0)
		return 0;
	if (table[k][s] == 0)
		return 0;
	if (table[k - 1][s] == table[k][s])
	{
		if (GetAnswer(timeOfTasks, rowIndexes, k - 1, s) != 0)
			return -1;
	}
		
	else
	{
		if (GetAnswer(timeOfTasks, rowIndexes, k - 1, s - timeOfTasks[rowIndexes[k] - 1]) != 0)
			return -1;
		int* tmp = (int*)realloc(result, (iter + 1) * sizeof(int));
		if (!tmp)
			return -1;
		result = tmp;
		result[iter] = rowIndexes[k];
		iter++;
	}
	return 0;
}

void PrintResult(FILE* output)
{
	int i;
	for (i = 0; i < iter - 1; i++)
	{
		if (result[i] != -1)
			fprintf(output, "%u ", result[i]);
		else
			fprintf(output, "\n");
	}
}

unsigned* FillRowIndexes(unsigned tasks)
{
	unsigned* rowIndexes = NULL;
	int i, j = 0, idx = 0;
	rowIndexes = (int*)malloc((tasks + 1 - iter + countOfMinus) * sizeof(int));
	if (!rowIndexes)
		return NULL;
	if (iter == 0)
	{
		for (i = 0; i <= tasks; i++)
			rowIndexes[i] = i;
		return rowIndexes;
	}
	for (i = 0; i <= tasks - iter + countOfMinus; i++)
	{
		
		for (j = 0; j < iter; j++)
			if (idx == result[j])
			{
				j = -1;
				idx++;
			}
		rowIndexes[i] = idx;
		idx++;
	}
	return rowIndexes;
}

int Distribution(unsigned tasks, unsigned fullTime, unsigned* timeOfTasks)
{
	int* rowIndexes = NULL;
	rowIndexes = FillRowIndexes(tasks);
	if (!rowIndexes)
		return -1;
	if (CreateTable(tasks, fullTime) == -1)
	{
		free(rowIndexes);
		return -1;
	}
		
	int i, j;
	for (i = 0; i < fullTime; i++)
		table[0][i] = 0;
	for (i = 1; i <= tasks - iter + countOfMinus; i++)
		for (j = 0; j < fullTime; j++)
		{
			if (j + 1 >= timeOfTasks[rowIndexes[i] - 1])
			{
				if (j < timeOfTasks[rowIndexes[i] - 1])
					table[i][j] = table[i - 1][j] > timeOfTasks[rowIndexes[i] - 1] ? table[i - 1][j] : timeOfTasks[rowIndexes[i] - 1];
				else
					table[i][j] = table[i - 1][j] > table[i - 1][j - timeOfTasks[rowIndexes[i] - 1]] + timeOfTasks[rowIndexes[i] - 1] ? table[i - 1][j] :
						table[i - 1][j - timeOfTasks[rowIndexes[i] - 1]] + timeOfTasks[rowIndexes[i] - 1];
			}
			else
				table[i][j] = table[i - 1][j];
		}
	if (GetAnswer(timeOfTasks, rowIndexes, i - 1, j - 1) != 0)
	{
		free(rowIndexes);
		return -1;
	}
	int* tmp = (int*)realloc(result, (iter + 1) * sizeof(int));
	if (!tmp)
	{
		free(rowIndexes);
		return -1;
	}
	result = tmp;
	result[iter] = -1;
	iter++;
	free(rowIndexes);
	return 0;
}

int LabSolution(const char* inputFileName, const char* outputFileName)
{
	FILE* input = fopen(inputFileName, "r");
	if (!input)
		return -1;
	FILE* output = fopen(outputFileName, "w");
	if (!output)
		return -1;
	unsigned tasks, fullTime, staffMembers;
	fscanf(input, "%u %u %u\n", &tasks, &fullTime, &staffMembers);
	unsigned* timeOfTasks = NULL;
	timeOfTasks = (unsigned*)malloc(tasks * sizeof(unsigned));
	if (!timeOfTasks)
	{
		fclose(input);
		fclose(output);
		return -1;
	}
	int i = 0;
	for (i = 0; i < tasks; i++)
		fscanf(input, "%u ", &timeOfTasks[i]);
	fclose(input);
	result = (int*)malloc(sizeof(int));
	if (!result)
	{
		fclose(output);
		free(timeOfTasks);
		return -1;
	}
	result[0] = -1;
	iter = 0;
	countOfMinus = 0;
	int activeTasks = tasks;
	while (staffMembers > 0 && activeTasks > 0)
	{
		if (Distribution(tasks, fullTime, timeOfTasks) == -1)
		{
			free(result);
			free(timeOfTasks);
			DestroyTable(tasks + 1);
			fclose(output);
			printf("ERROR in Distribution");
			return -1;
		}
		DestroyTable(tasks + 1);
		activeTasks = tasks + 1 - iter + countOfMinus;
		staffMembers--;
		countOfMinus++;
	}
	if (staffMembers == 0 && activeTasks > 0)
		fprintf(output, "0");
	if ((staffMembers == 0 && activeTasks == 0) || (staffMembers > 0 && activeTasks == 0))
		PrintResult(output);
	free(timeOfTasks);
	free(result);
	fclose(output);
	return 0;
}