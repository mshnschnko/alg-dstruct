#include "LabE6.h"

char* fileText;
int positionInFile;
int maxWordSize = 0;
int recursErr = !RECURSION_ERROR;
int nodes = 0;

tree_t* TreeParser()
{
	if (fileText[positionInFile] == '\'')
	{
		positionInFile += 2;
		if (fileText[positionInFile] == '\0')
			return NULL;
		char c = fileText[positionInFile++];
		for (; c == '(' || c == ')'; positionInFile++)
			c = fileText[positionInFile];
		positionInFile--;
		return NULL;
	}
	char c = 0;
	int size_of_name = 0;
	tree_t* tree = malloc(sizeof(tree_t));
	if (!tree)
	{
		recursErr = RECURSION_ERROR;
		return NULL;
	}
	for (int i = 0; c != '('; i++)
	{
		c = fileText[positionInFile + i];
		size_of_name++;
	}
	size_of_name--;
	tree->name = malloc((size_of_name + 1) * sizeof(char));
	if (!(tree->name))
	{
		recursErr = RECURSION_ERROR;
		return NULL;
	}
	int i;
	for (i = 0; i < size_of_name; i++)
	{
		tree->name[i] = fileText[positionInFile];
		positionInFile++;
	}
	tree->name[i] = '\0';
	positionInFile++;
	tree_t* left = TreeParser();
	if (recursErr)
		return NULL;
	tree_t* right = TreeParser();
	if (recursErr)
		return NULL;
	tree->left = left;
	tree->right = right;
	return tree;
}

tree_t* ReadBinaryTree(const char* inputFileName)
{
	FILE* treeFile = fopen(inputFileName, "r");
	if (treeFile == NULL)
	{
		//printf("Error in opening file");
		return NULL;
	}
	fseek(treeFile, 0, SEEK_END);
	int size = ftell(treeFile);
	rewind(treeFile);
	if (size < 1)
	{
		//printf("Error: empty file");
		return NULL;
	}
	fileText = malloc((size + 2) * sizeof(char));
	fscanf(treeFile, "%s", fileText);
	fclose(treeFile);
	positionInFile = 0;
	tree_t* tree = TreeParser();
	free(fileText);
	return tree;
}

treePrint_t* treePrint;
int elem = 0;

int FillTreePrint(tree_t* root, int space, int len, int orient)
{
	space += maxWordSize;
	int size = strlen(root->name);
	if (root->right != NULL)
		if (FillTreePrint(root->right, space, size, ORIENT_RIGHT) == -1)
			return -1;
	treePrint[elem].str = calloc((space + 1), sizeof(char));
	if (!treePrint[elem].str)
		return -1;
	treePrint[elem].tree = root;
	for (int i = 0; i < space - 2 * maxWordSize + len; i++)
		treePrint[elem].str[i] = ' ';
	if (space - 2 * maxWordSize + len > 0)
	{
		if (orient == ORIENT_RIGHT)
			treePrint[elem].str[space - 2 * maxWordSize + len] = '/';
		else if (orient == ORIENT_LEFT)
			treePrint[elem].str[space - 2 * maxWordSize + len] = '\\';
		for (int i = space - 2 * maxWordSize + len + 1; i < space - maxWordSize; i++)
			treePrint[elem].str[i] = '-';
	}
	for (int i = 0; i < size; i++)
		treePrint[elem].str[space - maxWordSize + i] = root->name[i];
	elem++;
	if (root->left != NULL)
		if (FillTreePrint(root->left, space, size, ORIENT_LEFT) == -1)
			return -1;
	return 0;
}

void FindMax(tree_t* tree)
{
	if (tree != NULL)
	{
		int len = strlen(tree->name);
		if (maxWordSize < len)
			maxWordSize = len;
		FindMax(tree->left);
		FindMax(tree->right);
		nodes++;
	}
}

void FindVertLines(tree_t* tree)
{
	if (tree != NULL)
	{
		int rightIdx = -1;
		int leftIdx = -1;
		int ownIdx;
		for (int i = 0; i < nodes; i++)
			if (tree == treePrint[i].tree)
			{
				ownIdx = i;
				break;
			}
		for (int i = 0; i < nodes; i++)
		{
			if (tree->right == treePrint[i].tree)
				rightIdx = i;
			else if (tree->left == treePrint[i].tree)
				leftIdx = i;
			else if (rightIdx > 0 && leftIdx > 0)
				break;
		}
		if (rightIdx >= 0)
		{
			int len = strlen(treePrint[ownIdx].str);
			for (int i = rightIdx + 1; i < ownIdx; i++)
				treePrint[i].str[len] = '|';
		}
		if (leftIdx >= 0)
		{
			int len = strlen(treePrint[ownIdx].str);
			for (int i = ownIdx + 1; i < leftIdx; i++)
				treePrint[i].str[len] = '|';
		}
		FindVertLines(tree->right);
		FindVertLines(tree->left);
	}
}

int PrintTree(tree_t* tree, FILE* output)
{
	FindMax(tree);
	maxWordSize++;
	treePrint = malloc(nodes * sizeof(treePrint_t));
	if (!treePrint)
		return -1;
	if (FillTreePrint(tree, 0, 0, 0) == -1)
		return -1;
	FindVertLines(tree);
	for (int i = 0; i < nodes; i++)
		fprintf(output, "%s\n", treePrint[i].str);
}

void FreeTree(tree_t* tree)
{
	if (tree->left)
		FreeTree(tree->left);
	if (tree->right)
		FreeTree(tree->right);
	if (tree->name)
		free(tree->name);
	free (tree);
}

void FreePrint(int count)
{
	for (int i = 0; i < count; i++)
	{
		free(treePrint[i].str);
		//FreeTree(treePrint[i].tree);
	}
	free(treePrint);
}

int Solution(const char* inputFileName, const char* outputFileName)
{
	tree_t* tree = ReadBinaryTree(inputFileName);
	if (!tree)
		return -1;
	FILE* output = fopen(outputFileName, "w");
	if (!output)
	{
		FreeTree(tree);
		return -1;
	}
	if (PrintTree(tree, output) == -1)
	{
		FreeTree(tree);
		FreePrint(elem);
		return -1;
	}
	//_PrintTree(stdout, tree, 0, 1);
	FreeTree(tree);
	//free(treePrint);
	FreePrint(nodes);
	fclose(output);
	return 0;
}