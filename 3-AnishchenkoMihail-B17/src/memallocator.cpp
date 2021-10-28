#include "memallocator.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SUCCESS 1
#define FAIL 0

#define TRUE 1
#define FALSE 0

#define FORWARD 1
#define BACK 0

#define DESC_SIZE (2 * sizeof(int) + 2 * sizeof(int*))

void* blockhead;
void* blocktail;
void* pmemory;
int blocksize;
int direction;

int* FirstSize(void* desc)
{
	return (int*)(desc);
}

void** NextBlock(void* desc)
{
	return (void**)((char*)desc + sizeof(int));
}

void** PrevBlock(void* desc)
{
	return (void**)((char*)desc + sizeof(int) + sizeof(void*));
}

int* SecondSize(void* desc)
{
	return (int*)((char*)desc + abs(*FirstSize(desc)) - sizeof(int));
}

int memgetminimumsize()
{
	return (int)DESC_SIZE;
}

int memgetblocksize()
{
	return (int)DESC_SIZE;
}

int meminit(void* pMemory, int size)
{
	if (size <= memgetminimumsize())
		return FAIL;
	pmemory = pMemory;
	*FirstSize(pmemory) = -size;
	*SecondSize(pmemory) = -size;
	*NextBlock(pmemory) = NULL;
	*PrevBlock(pmemory) = NULL;
	blockhead = pmemory;
	blocktail = blockhead;
	blocksize = size;
	direction = FORWARD;
	return SUCCESS;
}

void memdone()
{
	void* CurrentBlock = pmemory;
	while ((char*)CurrentBlock < (char*)pmemory + blocksize)
	{
		if (*FirstSize(CurrentBlock) > 0)
			fprintf(stderr, "MEMORY LEAK:\tBlock 0x%p; size: %i bytes.\n", CurrentBlock, abs(*FirstSize(CurrentBlock)));
		CurrentBlock = (void*)((char*)CurrentBlock + abs(*FirstSize(CurrentBlock)));
	}
}

void* Forward(void* head, int size)
{
	if (head == NULL)
		return NULL;
	while (abs(*FirstSize(head)) < size + memgetminimumsize())
	{
		if (*NextBlock(head) == NULL)
		{
			fprintf(stderr, "MEMORY ALLOCATION ERROR:\tthere is no suitable memory block (forward).\n");
			return NULL;
		}
		head = *NextBlock(head);
	}
	return head;
}

void* Back(void* tail, int size)
{
	if (tail == NULL)
		return NULL;
	while (abs(*FirstSize(tail)) < size + memgetminimumsize())
	{
		if (*PrevBlock(tail) == NULL)
		{
			fprintf(stderr, "MEMORY ALLOCATION ERROR:\tthere is no suitable memory block (back).\n");
			return NULL;
		}
		tail = *PrevBlock(tail);
	}
	return tail;
}

void* memalloc(int size)
{
	int movehead = FALSE;
	int movetail = FALSE;
	if (size <= 0 || size > blocksize - memgetminimumsize())
	{
		fprintf(stderr, "MEMORY ALLOCATION ERROR:\tincorrect size of allocated memory.\n");
		return NULL;
	}
	void* siutable_block = NULL;
	if (direction == FORWARD)
	{
		siutable_block = blockhead;
		if (siutable_block == NULL)
			return NULL;
		siutable_block = Forward(siutable_block, size);
		if (siutable_block == NULL)
			return NULL;
		if (siutable_block == blockhead)
			movehead = TRUE;
		direction = BACK;
	}
	else if (direction == BACK)
	{
		siutable_block = blocktail;
		if (siutable_block == NULL)
			return NULL;
		siutable_block = Back(siutable_block, size);
		if (siutable_block == NULL)
			return NULL;
		if (siutable_block == blocktail)
			movetail = TRUE;
		direction = FORWARD;
	}
	if (abs(*FirstSize(siutable_block)) > size + 2 * memgetminimumsize()) //then this block can be divided into two and the second one can be used in the future
	{
		void* newblock = (void*)((char*)siutable_block + memgetminimumsize() + size);
		*NextBlock(newblock) = *NextBlock(siutable_block);
		*PrevBlock(newblock) = *PrevBlock(siutable_block);
		*FirstSize(newblock) = -(abs(*FirstSize(siutable_block)) - memgetminimumsize() - size);
		*SecondSize(newblock) = *FirstSize(newblock);
		if (*NextBlock(siutable_block) != NULL)
		{
			void* next = *NextBlock(siutable_block);
			*PrevBlock(next) = newblock;
		}
		else
			blocktail = newblock;
		if (*PrevBlock(siutable_block) != NULL)
		{
			void* prev = *PrevBlock(siutable_block);
			*NextBlock(prev) = newblock;
		}
		else
			blockhead = newblock;
		*FirstSize(siutable_block) = size + memgetminimumsize();
		*SecondSize(siutable_block) = *FirstSize(siutable_block);
	}
	else
	{
		void* next = *NextBlock(siutable_block);
		void* prev = *PrevBlock(siutable_block);
		if (next == NULL && prev == NULL)
		{
			blockhead = NULL;
			blocktail = NULL;
		}
		else if (next == NULL)
		{
			*NextBlock(prev) = NULL;
			blocktail = prev;
		}
		else if (prev == NULL)
		{
			*PrevBlock(next) = NULL;
			blockhead = next;
		}
		*FirstSize(siutable_block) = abs(*FirstSize(siutable_block));
		*SecondSize(siutable_block) = *FirstSize(siutable_block);
	}
	*NextBlock(siutable_block) = NULL;
	*PrevBlock(siutable_block) = NULL;
	return (void*)((char*)siutable_block + memgetminimumsize() - sizeof(int));
}

void memfree(void* p)
{
	if (p == NULL)
	{
		fprintf(stderr, "MEMORY FREE ERROR:\tthis pointer is nullptr\n");
		return;
	}
	int merged_with_left = FALSE;
	int merged_with_right = FALSE;
	void* head = (void*)((char*)p - memgetminimumsize() + sizeof(int));
	*FirstSize(head) = -(abs(*FirstSize(head)));
	*SecondSize(head) = *FirstSize(head);
	void* next = NULL;
	if ((int*)((char*)head + abs(*FirstSize(head))) < (int*)((char*)pmemory + blocksize))
		next = (void*)((char*)head + abs(*FirstSize(head)));
	void* prev = NULL;
	if ((int*)((char*)head - 1) > (int*)pmemory)
		prev = (void*)((char*)head - abs(*((char*)head - sizeof(int))));
	if (prev != NULL)
		if (*FirstSize(prev) < 0)
		{
			*FirstSize(prev) = *FirstSize(prev) + *FirstSize(head); //both numbers are negative, so we sum one from the other
			*SecondSize(prev) = *FirstSize(prev);
			head = prev;
			merged_with_left = TRUE;
		}
	if (next != NULL)
		if (*FirstSize(next) < 0)
		{
			if (merged_with_left == FALSE)
			{
				*NextBlock(head) = blockhead;
				*PrevBlock(blockhead) = head;
				*PrevBlock(head) = NULL;
				blockhead = head;
			}
			if (*PrevBlock(next) != NULL)
				*NextBlock(*PrevBlock(next)) = *NextBlock(next);
			if (*NextBlock(next) != NULL)
				*PrevBlock(*NextBlock(next)) = *PrevBlock(next);
			if (blockhead == next)
				blockhead = *NextBlock(blockhead);
			if (blocktail == next)
				blocktail = *PrevBlock(blocktail);
			*FirstSize(head) = *FirstSize(head) + *FirstSize(next);
			*SecondSize(head) = *FirstSize(head);
			merged_with_right = TRUE;
		}
	if (merged_with_left == FALSE && merged_with_right == FALSE)
	{
		*NextBlock(head) = blockhead;
		if (blockhead != NULL)
			*PrevBlock(blockhead) = head;
		*PrevBlock(head) = NULL;
		blockhead = head;
	}
	if (blocktail == NULL)
		blocktail = head;
}