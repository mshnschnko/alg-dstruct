#include "pch.h"
#include "Stack.h"

TEST(StackListInit, StackListInit_TEST)
{
	List_t* head = StackListInit();
	EXPECT_FALSE(head == nullptr);
}

TEST(StackListPush, StackListPush_TEST)
{
	List_el elem3 = { 3, NULL };
	List_el elem2 = { 2, &elem3 };
	List_el elem1 = { 1, &elem2 };
	List_t head = { &elem1 };
	Data_t EntElem = 4;
	StackListPush(&head, EntElem);
	EXPECT_EQ(4, head.ptr->Data);
	EXPECT_TRUE(head.ptr->next == &elem1);
	EXPECT_TRUE(elem1.next == &elem2);
	EXPECT_TRUE(elem2.next == &elem3);
	EXPECT_TRUE(elem3.next == NULL);
}

TEST(StackListPop, StackListPop_TEST)
{
	List_el elem3 = { 3, NULL };
	List_el elem2 = { 2, &elem3 };
	List_el elem1 = { 1, &elem2 };
	List_t head = { &elem1 };
	StackListPop(&head);
	EXPECT_TRUE(head.ptr == &elem2);
	EXPECT_TRUE(elem2.next == &elem3);
	EXPECT_TRUE(elem3.next == NULL);
}

TEST(StackListTop, StackListTop_TEST)
{
	List_el elem3 = { 3, NULL };
	List_el elem2 = { 2, &elem3 };
	List_el elem1 = { 1, &elem2 };
	List_t head = { &elem1 };
	List_el* TopElem = StackListTop(&head);
	EXPECT_TRUE(1, TopElem->Data);
	EXPECT_TRUE(TopElem->next == &elem2);
}

TEST(StackListIsEmpty, StackListIsEmpty_TEST)
{

	List_t* head = NULL;
	EXPECT_EQ(1, StackListIsEmpty(head));
	head = (List_t*)malloc(sizeof(List_t));
	head->ptr = NULL;
	EXPECT_EQ(1, StackListIsEmpty(head));
	List_el elem3 = { 3, NULL };
	List_el elem2 = { 2, &elem3 };
	List_el elem1 = { 1, &elem2 };
	head->ptr = &elem1;
	EXPECT_EQ(0, StackListIsEmpty(head));
}

TEST(StackArrayInit, StackArrayInit_TEST)
{
	Array_t* array = StackArrayInit();
	EXPECT_NE(array, nullptr);
}

TEST(StackArrayPush, StackArrayPush_TEST)
{
	Array_t* StackArray = NULL;
	StackArray = (Array_t*)malloc(sizeof(Array_t));
	/*if (StackArray == NULL)
	{
		return NULL;
	}*/
	StackArray->data = NULL;
	StackArray->data = (Data_t*)malloc(sizeof(Data_t));
	StackArray->size = 1;
	StackArray->top = 0;
	Data_t NewData1 = 4;
	StackArrayPush(StackArray, NewData1);
	EXPECT_EQ(1, StackArray->size);
	EXPECT_EQ(1, StackArray->top);
	EXPECT_EQ(4, StackArray->data[0]);
	Data_t NewData2 = 7;
	StackArrayPush(StackArray, NewData2);
	EXPECT_EQ(2, StackArray->size);
	EXPECT_EQ(2, StackArray->top);
	EXPECT_EQ(7, StackArray->data[1]);
}

TEST(StackArrayPop, StackArrayPop_TEST)
{
	Array_t* StackArray = NULL;
	StackArray = (Array_t*)malloc(sizeof(Array_t));
	/*if (StackArray == NULL)
	{
		return NULL;
	}*/
	//StackArray = malloc(sizeof(Array_t));
	StackArray->data = NULL;
	StackArray->data = (Data_t*)malloc(2 * sizeof(Data_t));
	StackArray->size = 1;
	StackArray->top = 0;
	StackArray->data[0] = 1;
	StackArray->top++;
	StackArray->data[1] = 2;
	StackArray->size++;
	StackArray->top++;
	StackArrayPop(StackArray);
	EXPECT_EQ(0, StackArray->data[1]);
	EXPECT_EQ(2, StackArray->size);
	EXPECT_EQ(1, StackArray->top);
}

TEST(StackArrayTop, StackArrayTop_TEST)
{
	Array_t* StackArray = NULL;
	StackArray = (Array_t*)malloc(sizeof(Array_t));
	/*if (StackArray == NULL)
	{
		return NULL;
	}*/
	//StackArray = malloc(sizeof(Array_t));
	StackArray->data = NULL;
	StackArray->data = (Data_t*)malloc(sizeof(Data_t));
	StackArray->size = 1;
	StackArray->top = 0;
	StackArray->data[0] = 1;
	StackArray->top++;
	EXPECT_EQ(1, StackArrayTop(StackArray));
}

TEST(StackArrayIsEmpty, StackArrayIsEmpty_TEST)
{
	Array_t* StackArray = NULL;
	EXPECT_TRUE(StackArrayIsEmpty(StackArray));
	StackArray = (Array_t*)malloc(sizeof(Array_t));
	/*if (StackArray == NULL)
	{
		return NULL;
	}*/
	//StackArray = malloc(sizeof(Array_t));
	StackArray->data = NULL;
	EXPECT_TRUE(StackArrayIsEmpty(StackArray));
	StackArray->data = (Data_t*)malloc(sizeof(Data_t));
	StackArray->size = 1;
	StackArray->top = 0;
	EXPECT_TRUE(StackArrayIsEmpty(StackArray));
	StackArray->data[0] = 1;
	StackArray->top++;
	EXPECT_FALSE(StackArrayIsEmpty(StackArray));
}
