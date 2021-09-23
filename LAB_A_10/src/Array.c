#include "Stack.h"

Array_t* StackArrayInit(void)
{
	Array_t* StackArray = NULL;
	StackArray = malloc(sizeof(Array_t)); // = NULL;
	if (StackArray == NULL)
	{
		return NULL;
	}
	//StackArray = malloc(sizeof(Array_t));
	StackArray->data = NULL;
	StackArray->data = malloc(sizeof(Data_t));
	StackArray->size = 1;
	StackArray->top = 0;
	return StackArray;
}

void StackArrayDestroy(Array_t* Array)
{
	free(Array);
}

void StackArrayPush(Array_t* Array, Data_t Data)
{
	if (Array->top < Array->size)
		Array->data[Array->top++] = Data;
	else
	{
		Data_t* tmp = NULL;
		tmp = (Data_t*)realloc(Array->data, (Array->size + 1) * sizeof(Data_t));
		if (tmp == NULL)
		{
			return;
		}
		Array->data = tmp;
		Array->data[Array->top++] = Data;
	}
}

void StackArrayPop(Array_t* Array)
{
	//free(&(Array->data[Array->top - 1]));
	Array->data[Array->top - 1] = 0;
	//Array->size--;
	Array->top--;
}

Data_t StackArrayTop(Array_t* Array)
{
	return Array->data[Array->top - 1];
}

int StackArrayIsEmpty(Array_t* Array)
{
	return 0;
}

void MenuArray(void)
{
	int num;
	Array_t* StackArray = NULL;
	Data_t PushEl;
	printf("\t\tMENU\n\tplease choose the option:\n1.Init Array Stack\n2.Destroy Array Stack\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\n0.FUCK GO BACK\n");
	scanf_s("%i", &num);
	while (num != 0)
	{
		switch (num)
		{
		case 1:
			StackArray = StackArrayInit();
			printf("\nArray Stack is created\n");
			break;
		case 2:
			StackArrayDestroy(StackArray);
			printf("\nArray Stack is destroyed\n");
			break;
		case 3:
			printf("\nEnter the data: ");
			scanf_s("%i", &PushEl);
			StackArrayPush(StackArray, PushEl);
			printf("\nElement %i was added to the stack\n", PushEl);
			break;
		case 4:
			StackArrayPop(StackArray);
			printf("\nUpper element was deleted from the stack\n");
			break;
		case 5:
			printf("\nDATA: %i\n", StackArrayTop(StackArray));
			break;
		case 6:
			if (StackArrayIsEmpty(StackArray))
				printf("\nThe stack is Empty\n");
			else
				printf("\nThe stack contains any data\n");
			break;
		case 7:
			system("cls");
			printf("\t\tMENU\n\tplease choose the option:\n1.Init Array Stack\n2.Destroy Stack Array\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\n0.FUCK GO BACK\n");
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
}