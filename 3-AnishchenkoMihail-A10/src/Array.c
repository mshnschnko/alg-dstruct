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
		return NULL;
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

void MenuArray(void)
{
	int num, DestroyFlag = 0;
	Array_t* StackArray = NULL;
	Data_t PushEl;
	Data_t* top;
	printf("\t\tMENU\n\tplease choose the option:\n1.Init Array Stack\n2.Destroy Array Stack\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\n0.Back to main menu\n");
	scanf_s("%i", &num);
	while (num != 0)
	{
		switch (num)
		{
		case 1:
			StackArray = StackArrayInit();
			if (StackArray != NULL)
			{
				printf("\nArray Stack is created\n");
				DestroyFlag = 0;
			}
			else
				printf("MEMORY ALLOCATION ERROR");
			break;
		case 2:
			StackArrayDestroy(StackArray);
			printf("\nArray Stack is destroyed\n");
			DestroyFlag = 1;
			break;
		case 3:
			printf("\nEnter the data: ");
			scanf_s("%i", &PushEl);
			if(StackArrayPush(StackArray, PushEl))
				printf("\nElement %i was added to the stack\n", PushEl);
			else
				printf("MEMORY ALLOCATION ERROR");
			break;
		case 4:
			if (StackArrayPop(StackArray))
				printf("\nUpper element was deleted from the stack\n");
			else
				printf("\nERROR: Stack is empty\n");
			break;
		case 5:
			top = StackArrayTop(StackArray);
			if (top != NULL)
				printf("\nDATA: %i\n", *top);
			else
				printf("\nERROR: Stack is empty\n");
			break;
		case 6:
			if (StackArrayIsEmpty(StackArray))
				printf("\nThe stack is Empty\n");
			else
				printf("\nThe stack contains any data\n");
			break;
		case 7:
			system("cls");
			printf("\t\tMENU\n\tplease choose the option:\n1.Init Array Stack\n2.Destroy Stack Array\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\n0.Back to main menu\n");
			break;
		case 0:
			return;
			break;
		default:
			printf("\nplease choose the option again: ");
			break;
		}
		scanf_s("%i", &num);
	}
	if (DestroyFlag == 0)
		StackArrayDestroy(StackArray);
}