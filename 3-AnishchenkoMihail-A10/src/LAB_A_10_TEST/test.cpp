#include "pch.h"
#include "Stack.h"

TEST(StackListInit, StackListInit_TEST)
{
	List_t* head = StackListInit();
	EXPECT_NE(nullptr, head);
	free(head);
}

TEST(StackListPush_BeginWithEmpty, StackListPush_TEST)
{
	List_t head = { NULL };
	Data_t NewData = 1;
	EXPECT_EQ(1, StackListPush(&head, NewData));
	EXPECT_EQ(1, head.ptr->Data);
	EXPECT_EQ(nullptr, head.ptr->next);
	free(head.ptr);
}

TEST(StackListPush_BeginWithElems, StackListPush_TEST)
{
	List_el elem2 = { 2, NULL };
	List_el elem1 = { 1, &elem2 };
	List_t head = { &elem1 };
	Data_t NewData = 3;
	EXPECT_EQ(1, StackListPush(&head, NewData));
	EXPECT_EQ(3, head.ptr->Data);
	EXPECT_TRUE(head.ptr->next == &elem1);
	EXPECT_TRUE(elem1.next == &elem2);
	EXPECT_EQ(1, elem1.Data);
	EXPECT_EQ(2, elem2.Data);
	EXPECT_TRUE(elem2.next == NULL);
	free(head.ptr);
}

TEST(StackListPop_StackWithoutElems, StackListPop_TEST)
{
	List_t head = { NULL };
	EXPECT_EQ(0, StackListPop(&head));
}

TEST(StackListPop_Normal, StackListPop_TEST)
{
	List_el* elem2 = NULL;
	elem2 = (List_el*)malloc(sizeof(List_el));
	ASSERT_NE(nullptr, elem2);
	elem2->Data = 2;
	elem2->next = NULL;
	List_el* elem1 = NULL;
	elem1 = (List_el*)malloc(sizeof(List_el));
	ASSERT_NE(nullptr, elem1);
	elem1->Data = 1;
	elem1->next = elem2;
	List_t* head = NULL;
	head = (List_t*)malloc(sizeof(List_t));
	ASSERT_NE(nullptr, head);
	head->ptr = elem1;
	EXPECT_EQ(1, StackListPop(head));
	EXPECT_TRUE(head->ptr == elem2);
	EXPECT_TRUE(elem2->next == NULL);
	free(head);
	free(elem2);
}

TEST(StackListTop_WithoutElems, StackListTop_TEST)
{
	List_t head = { NULL };
	EXPECT_EQ(nullptr, StackListTop(&head));
}

TEST(StackListTop_Normal, StackListTop_TEST)
{
	List_el elem2 = { 2, NULL };
	List_el elem1 = { 1, &elem2 };
	List_t head = { &elem1 };
	EXPECT_EQ(&(elem1.Data), StackListTop(&head));
}

TEST(StackListIsEmpty_Empty, StackListIsEmpty_TEST)
{
	List_t head = { NULL };
	EXPECT_EQ(1, StackListIsEmpty(&head));
}

TEST(StackListIsEmpty_NotEmpty, StackListIsEmpty_TEST)
{
	List_el elem3 = { 3, NULL };
	List_el elem2 = { 2, &elem3 };
	List_el elem1 = { 1, &elem2 };
	List_t head = { &elem1 };
	EXPECT_EQ(0, StackListIsEmpty(&head));
}

TEST(StackArrayInit, StackArrayInit_TEST)
{
	Array_t* array = StackArrayInit();
	EXPECT_NE(array, nullptr);
	free(array->data);
	free(array);
}

TEST(StackArrayPush_BeginWithEmpty, StackArrayPush_TEST)
{
	Data_t data[1];
	Array_t StackArray = { data, 1, 0 };
	Data_t NewData = 1;
	EXPECT_EQ(1, StackArrayPush(&StackArray, NewData));
	EXPECT_EQ(1, StackArray.size);
	EXPECT_EQ(1, StackArray.top);
	EXPECT_EQ(1, StackArray.data[0]);
}

TEST(StackArrayPush_BeginWithElems, StackArrayPush_TEST)
{
	Data_t* data = (Data_t*)malloc(sizeof(Data_t));
	ASSERT_NE(nullptr, data);
	data[0] = 1;
	Array_t StackArray = { data, 1, 1 };
	Data_t NewData = 2;
	EXPECT_EQ(1, StackArrayPush(&StackArray, NewData));
	EXPECT_EQ(2, StackArray.size);
	EXPECT_EQ(2, StackArray.top);
	EXPECT_EQ(2, StackArray.data[1]);
	free(StackArray.data);
}

TEST(StackArrayPop_WithoutElems, StackArrayPop_TEST)
{
	Data_t data[1];
	Array_t StackArray = { data, 1, 0 };
	EXPECT_EQ(0, StackArrayPop(&StackArray));
}

TEST(StackArrayPop_Normal, StackArrayPop_TEST)
{
	Data_t data[2] = { 1, 2 };
	Array_t StackArray = { data, 2, 2 };
	EXPECT_EQ(1, StackArrayPop(&StackArray));
	EXPECT_EQ(0, StackArray.data[1]);
	EXPECT_EQ(2, StackArray.size);
	EXPECT_EQ(1, StackArray.top);
}

TEST(StackArrayTop_WithoutElems, StackArrayTop_TEST)
{
	Data_t data[1];
	Array_t StackArray = { data, 1, 0 };
	EXPECT_EQ(nullptr, StackArrayTop(&StackArray));
}

TEST(StackArrayTop_WithElems, StackArrayTop_TEST)
{
	Data_t data[2] = { 1, 2 };
	Array_t StackArray = { data, 2, 2 };
	EXPECT_EQ(&(StackArray.data[1]), StackArrayTop(&StackArray));
}

TEST(StackArrayIsEmpty_Empty, StackArrayIsEmpty_TEST)
{
	Data_t data[1];
	Array_t StackArray = { data, 1, 0 };
	EXPECT_EQ(1, StackArrayIsEmpty(&StackArray));
}

TEST(StackArrayIsEmpty_NotEmpty, StackArrayIsEmpty_TEST)
{
	Data_t data[2] = { 1, 2 };
	Array_t StackArray = { data, 2, 2 };
	EXPECT_EQ(0, StackArrayIsEmpty(&StackArray));
}