#include "Stack.h"

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