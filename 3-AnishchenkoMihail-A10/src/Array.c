#include "Stack.h"

Array_t* StackArrayInit(void)
{
	Array_t* StackArray = NULL;
	StackArray = (Array_t*)malloc(sizeof(Array_t));
	if (StackArray == NULL)
		return NULL;
	StackArray->data = NULL;
	StackArray->data = (Data_t*)malloc(sizeof(Data_t));
	if (StackArray->data == NULL)
	{
		free(StackArray);
		return NULL;
	}
	StackArray->size = 1;
	StackArray->top = 0;
	return StackArray;
}

void StackArrayDestroy(Array_t* Array)
{
	free(Array->data);
	free(Array);
}

int StackArrayPush(Array_t* Array, Data_t Data)
{
	if (Array->top < Array->size)
		Array->data[Array->top++] = Data;
	else
	{
		Data_t* tmp = NULL;
		tmp = (Data_t*)realloc(Array->data, (++Array->size) * sizeof(Data_t));
		if (tmp == NULL)
			return 0;
		Array->data = tmp;
		Array->data[Array->top++] = Data;
	}
	return 1;
}

int StackArrayPop(Array_t* Array)
{
	if (StackArrayIsEmpty(Array) == 0)
	{
		Array->data[Array->top - 1] = 0;
		Array->top--;
		return 1;
	}
	else
		return 0;
}

Data_t* StackArrayTop(Array_t* Array)
{
	if (StackArrayIsEmpty(Array) == 0)
		return &(Array->data[Array->top - 1]);
	else
		return NULL;
}

int StackArrayIsEmpty(Array_t* Array)
{
	if (Array == NULL || Array->top == 0)
		return 1;
	else
		return 0;
}