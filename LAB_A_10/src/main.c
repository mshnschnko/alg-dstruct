#include <stdio.h>
#include <stdlib.h>
#define  _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <crtdbg.h>

typedef int Data_t;

typedef struct List_el
{
	Data_t Data;
	struct List_el* next;
}List_el;

typedef struct
{
	List_el* ptr;
}List_t;

typedef struct
{
	Data_t* data;
	size_t size;
	size_t top;
}Array_t;

void MemoryLeaks(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}

List_t* StackListInit(void)
{
	List_t* head = NULL;
	head = malloc(sizeof(List_t));
	if (head != NULL)
		head->ptr = NULL;
	else
		return NULL;
	return head;
}

void StackListDestroy(List_t* head)
{
	if (head->ptr != NULL)
	{
		List_el* tmp = head->ptr->next, * p;
		free(head->ptr);
		while (tmp != NULL)
		{
			p = tmp;
			tmp = p->next;
			free(p);
		}
	}
	free(head);
}

void StackListPush(List_t* head, Data_t EntElem)
{
	List_el* elem;
	elem = malloc(sizeof(List_el));
	if (elem == NULL)
		return;
	elem->Data = EntElem;
	elem->next = head->ptr;
	head->ptr = elem;
}

void StackListPop(List_t* head)
{
	List_el* q = head->ptr;
	head->ptr = q->next;
	free(q);
}

List_el* StackListTop(List_t* head)
{
	return head->ptr;
}

int StackListIsEmpty(List_t* head)
{
	if (head == NULL || head->ptr == NULL)
		return 1;
	else
		return 0;
}

Array_t* StackArrayInit(void)
{
	Array_t* StackArray = malloc(sizeof(Array_t)); // = NULL;
	//StackArray = malloc(sizeof(Array_t));
	StackArray->data = NULL;
	StackArray->data = malloc(sizeof(Data_t));
	StackArray->size = 1;
	StackArray->top = 0;
	return StackArray;
}

void StackArrayPush(Array_t* Array, Data_t Data)
{
	if (Array->top < Array->size)
		Array->data[Array->top++] = Data;
	else
	{
		Array->data = realloc(Array->data, (Array->size + 1) * sizeof(Data_t));
		Array->data[Array->top++] = Data;
	}
}

void StackArrayPop(Array_t* Array)
{
	free(&(Array->data[Array->top - 1]));
	Array->size--;
	Array->top--;
}

Data_t StackArrayTop(Array_t* Array)
{
	return Array->data[Array->top - 1];
}

void StackArrayDestroy(Array_t* Array)
{
	free(Array);
}

int StackArrayIsEmpty(Array_t* Array)
{
	return 0;
}

void MenuList(void)
{
	int num;
	List_t* StackList = NULL;
	Data_t PushEl;
	printf("\t\tMENU\n\tplease choose the option:\n1.Init List Stack\n2.Destroy Stack List\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\n0.FUCK GO BACK\n");
	scanf_s("%i", &num);
	while (num != 0)
	{
		switch (num)
		{
		case 1:
			StackList = StackListInit();
			printf("\nList Stack is created\n");
			break;
		case 2:
			StackListDestroy(StackList);
			printf("\nList Stack is destroyed\n");
			break;
		case 3:
			printf("\nEnter the data: ");
			scanf_s("%i", &PushEl);
			StackListPush(StackList, PushEl);
			printf("\nElement %i was added to the stack\n", PushEl);
			break;
		case 4:
			StackListPop(StackList);
			printf("\nUpper element was deleted from the stack\n");
			break;
		case 5:
			printf("\nDATA: %i\n", StackListTop(StackList)->Data);
			break;
		case 6:
			if (StackListIsEmpty(StackList))
				printf("\nThe stack is Empty\n");
			else
				printf("\nThe stack contains any data\n");
			break;
		case 7:
			system("cls");
			printf("\t\tMENU\n\tplease choose the option:\n1.Init List Stack\n2.Destroy Stack List\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\n0.FUCK GO BACK\n");
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

int main(void)
{
	MemoryLeaks();
	int num;
	printf("\t\tWhat do you want work with?\n1.List\n2.Array\n0.EXIT\n\n");
	scanf_s("%i", &num);
	while (num != 0)
	{
		switch (num)
		{
		case 1:
			system("cls");
			MenuList();
			break;
		case 2:
			system("cls");
			MenuArray();
			break;
		case 0:
			break;
		default:
			printf("\nplease choose the option again: ");
			break;
		}
		system("cls");
		printf("\t\tWhat do you want work with?\n1.List\n2.Array\n0.EXIT\n\n");
		scanf_s("%i", &num);
	}

	//system("cls");
	printf("\n\tGOODBYE!");
	return 0;
}