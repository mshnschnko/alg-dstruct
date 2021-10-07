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