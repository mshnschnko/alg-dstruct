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

Array_t StackArrayInit()
{
	Array_t StackArray;
	StackArray.data = malloc(sizeof(Data_t));
	StackArray.size = 1;
	StackArray.top = 0;
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

}

int main(void)
{
	MemoryLeaks();
	int num;
	List_t* StackList = NULL;
	Data_t PushEl;
	printf("\t\tMENU\n\tplease choose the option:\n1.Init List Stack\n2.Destroy Stack List\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\nPRESS ANY KEY TO EXIT\n");
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
			printf("\t\tMENU\n\tplease choose the option:\n1.Init List Stack\n2.Destroy Stack List\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\n0.EXIT\n");
			break;
		case 0:
			break;
		default:
			printf("\nplease choose the option again: ");
			break;
		}
		scanf_s("%i", &num);
	}
	//system("cls");
	printf("\n\tGOODBYE!");
	return 0;
}