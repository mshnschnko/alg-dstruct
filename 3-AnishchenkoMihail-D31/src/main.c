#include "labdstr.h"

int main(void)
{
	MemoryLeaks();
	const char* inputFileName = "input.txt";
	const char* outputFileName = "output.txt";
	int k = LabSolution(inputFileName, outputFileName);
	return k;
}