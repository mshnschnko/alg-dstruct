#include "pch.h"
#include "memallocator.h"
#include <string.h>
#define TEST_MEMORY_BLOCK_SIZE 10
#define TEST_BLOCKS_COUNT 100

TEST(memgetminimumsize, min_size_expected16)
{
	int a = memgetminimumsize();
	EXPECT_EQ(a, 16);
}

TEST(memgetblocksize, min_size_expected16)
{
	int a = memgetminimumsize();
	EXPECT_EQ(a, 16);
}

TEST(meminit, meminit_expectedValidVal)
{
	const int TestSizeInit = memgetminimumsize() + 1;
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	ASSERT_EQ(init, 1);
	ASSERT_EQ(*FirstSize(ptr), -TestSizeInit);
	ASSERT_EQ(*NextBlock(ptr), nullptr);
	ASSERT_EQ(*PrevBlock(ptr), nullptr);
	ASSERT_TRUE(*FirstSize(ptr) < 0);
	ASSERT_EQ(*FirstSize(ptr), -TestSizeInit);
	free(ptr);
}

TEST(meminit_Test, meminit_meminitTooLittleBytes_expectZeroBytesInit)
{
	const int SizeNotEnoughToInit = memgetminimumsize();
	void* ptr = malloc(SizeNotEnoughToInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, SizeNotEnoughToInit);
	EXPECT_EQ(init, 0);
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocAllInitMemory_expectedFlagTrue)
{
	const int TestSizeInit = memgetminimumsize() + TEST_MEMORY_BLOCK_SIZE;
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	char* a = (char*)memalloc(TEST_MEMORY_BLOCK_SIZE);
	EXPECT_EQ(init, 1);
	EXPECT_EQ(*FirstSize(ptr), TestSizeInit);
	EXPECT_EQ(*NextBlock(ptr), nullptr);
	EXPECT_EQ(*PrevBlock(ptr), nullptr);
	EXPECT_EQ(*SecondSize(ptr), TestSizeInit);
	EXPECT_TRUE(*FirstSize(ptr) > 0);
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocAllInitMemoryWriteSmth_expectBlockInfoNotCorrupted)
{
	const int TestSizeInit = memgetminimumsize() + TEST_MEMORY_BLOCK_SIZE;
	const char text[TEST_MEMORY_BLOCK_SIZE] = "abcdefghi";
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	char* a = (char*)memalloc(TEST_MEMORY_BLOCK_SIZE);
	strcpy(a, text);
	EXPECT_EQ(init, 1);
	EXPECT_EQ(*FirstSize(ptr), TestSizeInit);
	EXPECT_EQ(*NextBlock(ptr), nullptr);
	EXPECT_EQ(*PrevBlock(ptr), nullptr);
	EXPECT_EQ(*SecondSize(ptr), TestSizeInit);
	EXPECT_TRUE(!strcmp(a, text));
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocAllInitMemoryNextMemallocFail_expectSecondMemallocReturnNull)
{
	const int TestSizeInit = memgetminimumsize() + TEST_MEMORY_BLOCK_SIZE;
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	char* a = (char*)memalloc(TEST_MEMORY_BLOCK_SIZE);
	char* b = (char*)memalloc(TEST_MEMORY_BLOCK_SIZE);
	EXPECT_EQ(b, (void*)NULL);
	EXPECT_EQ(init, 1);
	EXPECT_EQ(*FirstSize(ptr), TestSizeInit);
	EXPECT_EQ(*NextBlock(ptr), nullptr);
	EXPECT_EQ(*PrevBlock(ptr), nullptr);
	EXPECT_EQ(*SecondSize(ptr), TestSizeInit);
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocInitTextBlockAndCharBlock_expectMemoryAllocatedCorrect)
{
	const int TestSizeInit = memgetminimumsize() + TEST_MEMORY_BLOCK_SIZE + memgetblocksize() + sizeof(char);
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	char* a_text = (char*)memalloc(TEST_MEMORY_BLOCK_SIZE);
	char* b_char = (char*)memalloc(sizeof(char));
	void* desc_a = (void*)((char*)a_text - sizeof(int) - 2 * sizeof(int*));
	void* desc_b = (void*)((char*)b_char - sizeof(int) - 2 * sizeof(int*));
	EXPECT_EQ(init, 1);
	EXPECT_EQ(*FirstSize(desc_a), TEST_MEMORY_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*SecondSize(desc_a), TEST_MEMORY_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*NextBlock(desc_a), nullptr);
	EXPECT_EQ(*PrevBlock(desc_a), nullptr);
	EXPECT_EQ(*FirstSize(desc_b), (int)((char*)sizeof(char) + memgetblocksize()));
	EXPECT_EQ(*SecondSize(desc_b), (int)((char*)sizeof(char) + memgetblocksize()));
	EXPECT_EQ(*NextBlock(desc_b), nullptr);
	EXPECT_EQ(*PrevBlock(desc_b), nullptr);
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocInitTextBlockAndCharBlock_expectNoBlocksCorrupted)
{
	const int TestSizeInit = memgetminimumsize() + TEST_MEMORY_BLOCK_SIZE + memgetblocksize() + sizeof(char);
	const char text[TEST_MEMORY_BLOCK_SIZE] = "abcdefghi";
	const char b = 'b';
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	char* a_text = (char*)memalloc(TEST_MEMORY_BLOCK_SIZE);
	char* b_char = (char*)memalloc(sizeof(char));
	void* desc_a = (void*)((char*)a_text - sizeof(int) - 2 * sizeof(void*));
	void* desc_b = (void*)((char*)b_char - sizeof(int) - 2 * sizeof(void*));
	strcpy(a_text, text);
	*b_char = b;
	EXPECT_EQ(init, 1);
	EXPECT_EQ(*FirstSize(desc_a), TEST_MEMORY_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*NextBlock(desc_a), nullptr);
	EXPECT_EQ(*PrevBlock(desc_a), nullptr);
	EXPECT_EQ(*SecondSize(desc_a), TEST_MEMORY_BLOCK_SIZE + memgetblocksize());	
	EXPECT_EQ(*FirstSize(desc_b), (int)sizeof(char) + memgetblocksize());
	EXPECT_EQ(*NextBlock(desc_b), nullptr);
	EXPECT_EQ(*PrevBlock(desc_b), nullptr);
	EXPECT_EQ(*SecondSize(desc_b), (int)sizeof(char) + memgetblocksize());
	EXPECT_TRUE(!strcmp(a_text, text));
	EXPECT_EQ(*b_char, b);
	free(ptr);
}

TEST(memfree_Test, memfree_memfreeFreeOneBlock_expectSizeIsNegative)
{
	const int TestSizeInit = memgetminimumsize() + sizeof(char);
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	*FirstSize(ptr) = TestSizeInit;
	*NextBlock(ptr) = NULL;
	*PrevBlock(ptr) = NULL;
	*SecondSize(ptr) = *SecondSize(ptr);
	blockhead = NULL;
	blocktail = NULL;
	void* user_ptr = (void*)((char*)ptr + sizeof(int) + 2 * sizeof(int*));
	memfree(user_ptr);
	EXPECT_TRUE(*FirstSize(ptr) < 0);
	EXPECT_EQ(blockhead, ptr);
	EXPECT_EQ(blocktail, ptr);
	free(ptr);
}

TEST(memfree_Test, memfree_memfreeFreeBlockWhereRightBlockIsFree_expectBlocksMerged)
{
	const int TestSizeInit = memgetminimumsize() + memgetblocksize() + 2 * sizeof(char);
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	void* allocated_block = ptr;
	*FirstSize(allocated_block) = (int)(sizeof(char) + memgetblocksize());
	*NextBlock(allocated_block) = NULL;
	*PrevBlock(allocated_block) = NULL;
	*SecondSize(allocated_block) = *FirstSize(allocated_block);
	void* free_block = (void*)((char*)allocated_block + abs(*FirstSize(allocated_block)));
	blockhead = free_block;
	blocktail = free_block;
	*FirstSize(free_block) = -((int)(sizeof(char) + memgetblocksize()));
	*NextBlock(free_block) = NULL;
	*PrevBlock(free_block) = NULL;
	*SecondSize(free_block) = *FirstSize(free_block);
	void* user_ptr_to_allocated_block = (void*)((char*)allocated_block + sizeof(int) + 2 * sizeof(void*));
	memfree(user_ptr_to_allocated_block);
	EXPECT_EQ(*FirstSize(allocated_block), -blocksize);
	EXPECT_EQ(*SecondSize(allocated_block), -blocksize);
	EXPECT_EQ(blockhead, ptr);
	EXPECT_EQ(blocktail, ptr);
	free(ptr);
}

TEST(memfree_Test, memfree_memfreeFreeBlockWhereLeftBlockIsFree_expectBlocksMerged)
{
	const int TestSizeInit = memgetminimumsize() + memgetblocksize() + 2 * sizeof(char);
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	void* free_block = (void*)ptr;
	blockhead = free_block;
	*FirstSize(free_block) = -((int)sizeof(char) + memgetblocksize());
	*NextBlock(free_block) = NULL;
	*PrevBlock(free_block) = NULL;
	*SecondSize(free_block) = *FirstSize(free_block);
	void* allocated_block = (void*)((char*)free_block + abs(*FirstSize(free_block)));
	*FirstSize(allocated_block) = (int)sizeof(char) + memgetblocksize(); // block is allocated so size is negative
	*NextBlock(allocated_block) = NULL;
	*PrevBlock(allocated_block) = NULL;
	*SecondSize(allocated_block) = *FirstSize(allocated_block);
	void* user_ptr_to_allocated_block = (void*)((char*)allocated_block + sizeof(int) + 2 * sizeof(int*));
	memfree(user_ptr_to_allocated_block);
	EXPECT_EQ(*FirstSize(free_block), -blocksize);
	EXPECT_EQ(*SecondSize(free_block), -blocksize);
	EXPECT_EQ(blockhead, ptr);
	EXPECT_EQ(blocktail, ptr);
	free(ptr);
}

TEST(memfree_Test, memfree_memfreeFreeBlockBetweenTwoFreeBlocks_expectBlocksMerged)
{
	const int TestSizeInit = memgetminimumsize() + 2 * memgetblocksize() + 3 * sizeof(char);
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	void* first_free_block = ptr;
	blockhead = first_free_block;
	*FirstSize(first_free_block) = -((int)sizeof(char) + memgetblocksize());
	*NextBlock(first_free_block) = NULL;
	*PrevBlock(first_free_block) = NULL;
	*SecondSize(first_free_block) = *FirstSize(first_free_block);
	void* allocated_block = (void*)((char*)first_free_block + abs(*FirstSize(first_free_block)));
	*FirstSize(allocated_block) = (int)sizeof(char) + memgetblocksize();
	*NextBlock(allocated_block) = NULL;
	*PrevBlock(allocated_block) = NULL;
	*SecondSize(allocated_block) = *FirstSize(allocated_block);
	void* third_free_block = (void*)((char*)allocated_block + *FirstSize(allocated_block));
	*FirstSize(third_free_block) = -((int)sizeof(char) + memgetblocksize());
	*NextBlock(third_free_block) = NULL;
	*PrevBlock(third_free_block) = first_free_block;
	*SecondSize(third_free_block) = *FirstSize(third_free_block);
	*NextBlock(first_free_block) = third_free_block;
	void* user_ptr_to_allocated_block = (void*)((char*)allocated_block + sizeof(int) + 2 * sizeof(int*));
	memfree(user_ptr_to_allocated_block);
	EXPECT_EQ(*FirstSize(first_free_block), -blocksize);
	EXPECT_EQ(*SecondSize(first_free_block), -blocksize);
	EXPECT_EQ(*NextBlock(first_free_block), nullptr);
	EXPECT_EQ(*PrevBlock(first_free_block), nullptr);
	EXPECT_EQ(blockhead, ptr);
	EXPECT_EQ(blocktail, ptr);
	free(ptr);
}

TEST(memalloc_FuncTest, memalloc_listptrSizeLessThenAskedToMalloc_expectRightBlock)
{
	const int TestSizeInit = 3 * memgetblocksize() + 1 + 5 + 5;
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	char* a = (char*)memalloc(5);
	char* b = (char*)memalloc(5);
	char* c = (char*)memalloc(1);
	EXPECT_TRUE(a);
	EXPECT_TRUE(b);
	EXPECT_TRUE(c);
	memfree(a);
	memfree(c);
	char* d = (char*)memalloc(5);
	c = (char*)memalloc(1);
	EXPECT_TRUE(d);
	EXPECT_TRUE(c);
	EXPECT_TRUE(d == a);
	memfree(d);
	memfree(b);
	memfree(c);
	EXPECT_EQ(*FirstSize(ptr), -blocksize);
	EXPECT_EQ(*SecondSize(ptr), -blocksize);
	EXPECT_EQ(blockhead, ptr);
	EXPECT_EQ(blocktail, ptr);
	memdone();
	free(ptr);
}

TEST(memallocator_StressTest, memalloc_manyBlocksAllocAndFree_expectMemoryStateSimilarToAfterInit)
{
	const int TEST_BLOCK_SIZE = 16;
	const int TEST_MEMORY_SIZE = TEST_BLOCKS_COUNT * TEST_BLOCK_SIZE;
	const int TestSizeInit = TEST_BLOCKS_COUNT * (TEST_BLOCK_SIZE + memgetblocksize());
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	void** blocks = (void**)malloc(TEST_BLOCKS_COUNT * sizeof(void*));
	if (!blocks)
	{
		free(ptr);
		ASSERT_TRUE(blocks);
	}
	for (int i = 0; i < TEST_BLOCKS_COUNT; i++)
		blocks[i] = (char*)memalloc(TEST_BLOCK_SIZE);
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++)
		memfree(blocks[2 * i]);
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++)
		blocks[2 * i] = (char*)memalloc(TEST_BLOCK_SIZE);
	for (int i = 0; 2 * i + 1 < TEST_BLOCKS_COUNT; i++)
		memfree(blocks[2 * i + 1]);
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++)
		memfree(blocks[2 * i]);
	EXPECT_EQ(*FirstSize(ptr), -blocksize);
	EXPECT_EQ(*SecondSize(ptr), -blocksize);
	EXPECT_EQ(*NextBlock(ptr), nullptr);
	EXPECT_EQ(*PrevBlock(ptr), nullptr);
	memdone();
	free(blocks);
	free(ptr);
}

TEST(memallocator_StressTest, memalloc_manyRandomBlocksAllocAndFree_expectMemoryStateSimilarToAfterInit)
{
	const int TEST_BLOCK_SIZE = 16;
	const int TEST_MEMORY_SIZE = TEST_BLOCKS_COUNT * TEST_BLOCK_SIZE;
	const int TestSizeInit = 2 * TEST_BLOCKS_COUNT * (TEST_BLOCK_SIZE + memgetblocksize());
	void* ptr = malloc(TestSizeInit);
	ASSERT_TRUE(ptr);
	int init = meminit(ptr, TestSizeInit);
	if (!init)
	{
		free(ptr);
		ASSERT_TRUE(init);
	}
	void** blocks = (void**)malloc(TEST_BLOCKS_COUNT * sizeof(void*));
	if (!blocks)
	{
		free(ptr);
		ASSERT_TRUE(blocks);
	}
	for (int i = 0; i < TEST_BLOCKS_COUNT; i++)
		blocks[i] = (char*)memalloc(rand() % TEST_BLOCK_SIZE + 1);
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++)
		memfree(blocks[2 * i]);
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++)
		blocks[2 * i] = (char*)memalloc(TEST_BLOCK_SIZE);
	for (int i = 0; 2 * i + 1 < TEST_BLOCKS_COUNT; i++)
		memfree(blocks[2 * i + 1]);
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++)
		memfree(blocks[2 * i]);
	EXPECT_EQ(*FirstSize(ptr), -blocksize);
	EXPECT_EQ(*SecondSize(ptr), -blocksize);
	EXPECT_EQ(*NextBlock(ptr), nullptr);
	EXPECT_EQ(*PrevBlock(ptr), nullptr);
	memdone();
	free(blocks);
	free(ptr);
}