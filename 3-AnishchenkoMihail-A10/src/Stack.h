#pragma once
#ifdef __cplusplus
extern "C" {
#endif
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
int StackListPush(List_t* head, Data_t EntElem);
int StackListPop(List_t* head);
Data_t* StackListTop(List_t* head);
int StackListIsEmpty(List_t* head);
void MenuList(void);

Array_t* StackArrayInit(void);
void StackArrayDestroy(Array_t* Array);
int StackArrayPush(Array_t* Array, Data_t Data);
int StackArrayPop(Array_t* Array);
Data_t* StackArrayTop(Array_t* Array);
int StackArrayIsEmpty(Array_t* Array);
void MenuArray(void);
#ifdef __cplusplus
}
#endif
