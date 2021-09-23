#pragma once
#include <stdio.h>
#include <stdlib.h>

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

List_t* StackListInit(void);
void StackListDestroy(List_t* head);
void StackListPush(List_t* head, Data_t EntElem);
void StackListPop(List_t* head);
List_el* StackListTop(List_t* head);
int StackListIsEmpty(List_t* head);
void MenuList(void);

Array_t* StackArrayInit(void);
void StackArrayDestroy(Array_t* Array);
void StackArrayPush(Array_t* Array, Data_t Data);
void StackArrayPop(Array_t* Array);
Data_t StackArrayTop(Array_t* Array);
int StackArrayIsEmpty(Array_t* Array);
void MenuArray(void);