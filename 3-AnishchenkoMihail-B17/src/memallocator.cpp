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

#define DESC_SIZE (2 * sizeof(int) + 2 * sizeof(void*))

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
	void* currentblock = pmemory;
	while ((char*)currentblock < (char*)pmemory + blocksize)
	{
		if (*FirstSize(currentblock) > 0)
			fprintf(stderr, "MEMORY LEAK:\tBlock 0x%p; size: %i bytes.\n", currentblock, abs(*FirstSize(currentblock)));
		currentblock = (void*)((char*)currentblock + abs(*FirstSize(currentblock)));
	}
}

void* Forward(void* head, int size)
{
	if (head == NULL)
		return NULL;
	while (abs(*FirstSize(head)) < size + memgetblocksize())
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
	while (abs(*FirstSize(tail)) < size + memgetblocksize())
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
	void* siutableblock = NULL;
	if (direction == FORWARD)
	{
		siutableblock = blockhead;
		if (siutableblock == NULL)
			return NULL;
		siutableblock = Forward(siutableblock, size);
		if (siutableblock == NULL)
			return NULL;
		if (siutableblock == blockhead)
			movehead = TRUE;
		direction = BACK;
	}
	else if (direction == BACK)
	{
		siutableblock = blocktail;
		if (siutableblock == NULL)
			return NULL;
		siutableblock = Back(siutableblock, size);
		if (siutableblock == NULL)
			return NULL;
		if (siutableblock == blocktail)
			movetail = TRUE;
		direction = FORWARD;
	}
	if (abs(*FirstSize(siutableblock)) > size + 2 * memgetblocksize()) //then this block can be divided into two and the second one can be used in the future
	{
		void* newblock = (void*)((char*)siutableblock + memgetblocksize() + size);
		*NextBlock(newblock) = *NextBlock(siutableblock);
		*PrevBlock(newblock) = *PrevBlock(siutableblock);
		*FirstSize(newblock) = -(abs(*FirstSize(siutableblock)) - memgetblocksize() - size);
		*SecondSize(newblock) = *FirstSize(newblock);
		if (*NextBlock(siutableblock) != NULL)
		{
			void* next = *NextBlock(siutableblock);
			*PrevBlock(next) = newblock;
		}
		else
			blocktail = newblock;
		if (*PrevBlock(siutableblock) != NULL)
		{
			void* prev = *PrevBlock(siutableblock);
			*NextBlock(prev) = newblock;
		}
		else
			blockhead = newblock;
		*FirstSize(siutableblock) = size + memgetblocksize();
		*SecondSize(siutableblock) = *FirstSize(siutableblock);
	}
	else
	{
		void* next = *NextBlock(siutableblock);
		void* prev = *PrevBlock(siutableblock);
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
		else
		{
			*NextBlock(prev) = next;
			*PrevBlock(next) = prev;
		}
		*FirstSize(siutableblock) = abs(*FirstSize(siutableblock));
		*SecondSize(siutableblock) = *FirstSize(siutableblock);
	}
	*NextBlock(siutableblock) = NULL;
	*PrevBlock(siutableblock) = NULL;
	return (void*)((char*)siutableblock + memgetblocksize() - sizeof(int));
}

void memfree(void* p)
{
	if (p == NULL)
	{
		fprintf(stderr, "MEMORY FREE ERROR:\tthis pointer is nullptr\n");
		return;
	}
	int mergedwithleft = FALSE;
	int mergedwithright = FALSE;
	void* head = (void*)((char*)p - memgetblocksize() + sizeof(int));
	*FirstSize(head) = -(abs(*FirstSize(head)));
	*SecondSize(head) = *FirstSize(head);
	void* next = NULL;
	if (((char*)head + abs(*FirstSize(head))) < ((char*)pmemory + blocksize))
		next = (void*)((char*)head + abs(*FirstSize(head)));
	void* prev = NULL;
	if ((int*)((char*)head - 1) > (int*)pmemory)
		prev = (void*)((char*)head - abs(*((int*)head - 1)));
	if (prev != NULL)
		if (*FirstSize(prev) < 0)
		{
			*FirstSize(prev) = *FirstSize(prev) + *FirstSize(head); //both numbers are negative, so we sum one from the other
			*SecondSize(prev) = *FirstSize(prev);
			head = prev;
			mergedwithleft = TRUE;
		}
	if (next != NULL)
		if (*FirstSize(next) < 0)
		{
			if (mergedwithleft == FALSE)
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
			mergedwithright = TRUE;
		}
	if (mergedwithleft == FALSE && mergedwithright == FALSE)
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