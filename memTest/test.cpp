#include "pch.h"
#include "memallocator.h"
#include <string.h>
#define TEST_MEMORY_TEXT_BLOCK_SIZE 10

/*TEST(memgetminimumsize, min_size_expected16) {
	int a = memgetminimumsize();
	EXPECT_EQ(a, 16);
}

TEST(meminit, meminit_expectedValidVal) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + 1;
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	int* desc = (int*)ptr;
	ASSERT_EQ(bytes_init, 1);
	ASSERT_EQ(*FirstSize(desc), -TEST_MEMORY_SIZE_INIT);
	ASSERT_EQ(*NextBlock(desc), (void*)NULL);
	ASSERT_EQ(*PrevBlock(desc), (void*)NULL);
	ASSERT_TRUE(*FirstSize(desc) < 0);
	ASSERT_EQ(*FirstSize(desc), -TEST_MEMORY_SIZE_INIT);
	free(ptr);
}

TEST(meminit_Test, meminit_meminitTooLittleBytes_expectZeroBytesInit) {
	const int TEST_MEMORY_SIZE_NOT_ENOUGH_TO_INIT = memgetminimumsize();
	void* ptr = malloc(TEST_MEMORY_SIZE_NOT_ENOUGH_TO_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_NOT_ENOUGH_TO_INIT);
	EXPECT_EQ(bytes_init, 0);
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocAllInitMemory_expectedFlagTrue) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE;
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	//void* desc = ptr;
	char* a = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*FirstSize(ptr), TEST_MEMORY_SIZE_INIT);
	EXPECT_EQ(*NextBlock(ptr), (void*)NULL);
	EXPECT_EQ(*PrevBlock(ptr), (void*)NULL);
	EXPECT_EQ(*SecondSize(ptr), TEST_MEMORY_SIZE_INIT);
	EXPECT_TRUE(*FirstSize(ptr) > 0);
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocAllInitMemoryWriteSmth_expectBlockInfoNotCorrupted) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE;
	const char sometext[TEST_MEMORY_TEXT_BLOCK_SIZE] = "abccbbfff";
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	char* a = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	// copying smth to *a not more than TEST_MEMORY_TEXT_BLOCK_SIZE so if smth goes wrong block info will be damaged
	strcpy(a, sometext);
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*FirstSize(ptr), TEST_MEMORY_SIZE_INIT);
	EXPECT_EQ(*NextBlock(ptr), (void*)NULL);
	EXPECT_EQ(*PrevBlock(ptr), (void*)NULL);
	//EXPECT_EQ(*isFree(desc), 0);
	EXPECT_EQ(*SecondSize(ptr), TEST_MEMORY_SIZE_INIT);
	EXPECT_TRUE(!strcmp(a, sometext));
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocAllInitMemoryNextMemallocFail_expectSecondMemallocReturnNull) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE;
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	char* a = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	char* b = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	EXPECT_EQ(b, (void*)NULL);
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*FirstSize(ptr), TEST_MEMORY_SIZE_INIT);
	EXPECT_EQ(*NextBlock(ptr), (void*)NULL);
	EXPECT_EQ(*PrevBlock(ptr), (void*)NULL);
	//EXPECT_EQ(*isFree(desc), 0);
	EXPECT_EQ(*SecondSize(ptr), TEST_MEMORY_SIZE_INIT);
	free(ptr);
}*/

/*TEST(memalloc_Test, memalloc_memallocInitTextBlockAndCharBlock_expectMemoryAllocatedCorrect) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize() + sizeof(char);
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	char* a_text = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	char* b_char = (char*)memalloc(sizeof(char));
	void* desc_a = (void*)((char*)a_text - sizeof(int) - 2 * sizeof(int*));
	void* desc_b = (void*)((char*)b_char - sizeof(int) - 2 * sizeof(int*));
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*FirstSize(desc_a), TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*SecondSize(desc_a), TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*NextBlock(desc_a), (void*)NULL);
	EXPECT_EQ(*PrevBlock(desc_a), (void*)NULL);

	//EXPECT_EQ(*isFree(desc_a), 0);
	//EXPECT_EQ(*FirstSize(desc_a), TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());

	EXPECT_EQ(*FirstSize(desc_b), (int)((char*)sizeof(char) + memgetblocksize()));
	EXPECT_EQ(*SecondSize(desc_b), (int)((char*)sizeof(char) + memgetblocksize()));
	EXPECT_EQ(*NextBlock(desc_b), (void*)NULL);
	EXPECT_EQ(*PrevBlock(desc_b), (void*)NULL);

	//EXPECT_EQ(*isFree(desc_b), 0);
	free(ptr);
}*/

TEST(memalloc_Test, memalloc_memallocInitTextBlockAndCharBlock_expectNoBlocksCorrupted) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize() + sizeof(char);
	const char sometext[TEST_MEMORY_TEXT_BLOCK_SIZE] = "abccbbfff";
	const char b = 'b';
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	char* a_text = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	char* b_char = (char*)memalloc(sizeof(char));
	void* desc_a = (void*)((char*)a_text - sizeof(int) - 2 * sizeof(void*));
	void* desc_b = (void*)((char*)b_char - sizeof(int) - 2 * sizeof(void*));
	strcpy(a_text, sometext);
	*b_char = b;
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*FirstSize(desc_a), TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*NextBlock(desc_a), (void*)NULL);
	EXPECT_EQ(*PrevBlock(desc_a), (void*)NULL);
	EXPECT_EQ(*SecondSize(desc_a), TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());

	//EXPECT_EQ(*isFree(desc_a), 0);
	
	EXPECT_EQ(*FirstSize(desc_b), (int)sizeof(char) + memgetblocksize());
	EXPECT_EQ(*NextBlock(desc_b), (void*)NULL);
	EXPECT_EQ(*PrevBlock(desc_b), (void*)NULL);

	//EXPECT_EQ(*isFree(desc_b), 0);

	EXPECT_EQ(*SecondSize(desc_b), (int)sizeof(char) + memgetblocksize());
	EXPECT_TRUE(!strcmp(a_text, sometext));
	EXPECT_EQ(*b_char, b);
	free(ptr);
}

TEST(memfree_Test, memfree_memfreeFreeOneBlock_expectSizeIsPositive) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + sizeof(char);
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	// making all init memory allocated block
	void* all_init_memory_block_desc = ptr;
	*FirstSize(all_init_memory_block_desc) = *FirstSize(all_init_memory_block_desc);
	*NextBlock(all_init_memory_block_desc) = NULL;
	*PrevBlock(all_init_memory_block_desc) = NULL;
	//*isFree(all_init_memory_block_desc) = 0;
	*SecondSize(all_init_memory_block_desc) = *SecondSize(all_init_memory_block_desc);
	blockhead = NULL;
	blocktail = NULL;
	// freeing this block
	void* user_ptr_to_block = (void*)((char*)all_init_memory_block_desc + sizeof(int) + 2 * sizeof(int*));
	memfree(user_ptr_to_block);
	EXPECT_TRUE(*FirstSize(all_init_memory_block_desc) < 0);
	EXPECT_EQ(blockhead, ptr);
	EXPECT_EQ(blocktail, ptr);
	free(ptr);
}