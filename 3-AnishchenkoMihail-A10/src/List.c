#include "Stack.h"

List_t* StackListInit(void)
{
	List_t* head = NULL;
	head = (List_t*)malloc(sizeof(List_t));
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

int StackListPush(List_t* head, Data_t EntElem)
{
	List_el* elem;
	elem = malloc(sizeof(List_el));
	if (elem == NULL)
		return 0;
	elem->Data = EntElem;
	elem->next = head->ptr;
	head->ptr = elem;
	return 1;
}

int StackListPop(List_t* head)
{
	if (StackListIsEmpty(head) == 0)
	{
		List_el* q = head->ptr;
		head->ptr = q->next;
		free(q);
		return 1;
	}
	else
		return 0;
}

Data_t* StackListTop(List_t* head)
{
	if (StackListIsEmpty(head) == 0)
		return &(head->ptr->Data);
	else
		return NULL;
}

int StackListIsEmpty(List_t* head)
{
	if (head == NULL || head->ptr == NULL)
		return 1;
	else
		return 0;
}

void MenuList(void)
{
	int num;
	List_t* StackList = NULL;
	Data_t PushEl;
	Data_t* top;
	printf("\t\tMENU\n\tplease choose the option:\n1.Init List Stack\n2.Destroy Stack List\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\n0.Back to main menu\n");
	scanf_s("%i", &num);
	while (num != 0)
	{
		switch (num)
		{
		case 1:
			StackList = StackListInit();
			if (StackList != NULL)
				printf("\nList Stack is created\n");
			else
				printf("MEMORY ALLOCATION ERROR");
			break;
		case 2:
			StackListDestroy(StackList);
			printf("\nList Stack is destroyed\n");
			break;
		case 3:
			printf("\nEnter the data: ");
			scanf_s("%i", &PushEl);
			if (StackListPush(StackList, PushEl))
				printf("\nElement %i was added to the stack\n", PushEl);
			else
				printf("MEMORY ALLOCATION ERROR");
			break;
		case 4:
			if (StackListPop(StackList))
				printf("\nUpper element was deleted from the stack\n");
			else
				printf("\nERROR: Stack is empty\n");
			break;
		case 5:
			top = StackListTop(StackList);
			if (top != NULL)
				printf("\nDATA: %i\n", *top);
			else
				printf("\nERROR: Stack is empty\n");
			break;
		case 6:
			if (StackListIsEmpty(StackList))
				printf("\nThe stack is Empty\n");
			else
				printf("\nThe stack contains any data\n");
			break;
		case 7:
			system("cls");
			printf("\t\tMENU\n\tplease choose the option:\n1.Init List Stack\n2.Destroy Stack List\n3.Push\n4.Pop\n5.Top\n6.Is Stack empty?\n7.Clear log\n\n0.Back to main menu\n");
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