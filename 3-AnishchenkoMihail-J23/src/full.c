#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC
#define STR_SIZE 1000
#define OTHER_PRIME 999961

typedef enum {
	NOT_INTENTED = 0,
	NOT_FREE,
	FREE
} codes;

typedef struct Htable {
	char* str;
	unsigned size;
	codes status;
} Htable;

int Init(Htable** ht, int htSize);
int AddTable(Htable* ht, char* str);
int FindTable(Htable* ht, char* str);
int RemoveTable(Htable* ht, char* str);
void DeleteTable(Htable* ht);

int Init(Htable** ht, int htSize) {
	*ht = (Htable*)malloc(sizeof(Htable) * htSize);
	if (*ht == NULL)
		return 0;
	(**ht).size = htSize;
	for (int i = 0; i < htSize; i++)
		(*ht)[i].status = NOT_INTENTED;
	return 1;
}

unsigned Func1(char* str, int size) {
	unsigned res = 0;
	for (int i = 0; str[i] != 0; i++) {
		res += (unsigned)str[i];
		res %= size;
	}
	return res;
}

unsigned Func2(char* str, int size) {
	unsigned res = 0;
	for (int i = 0; str[i] != 0; i++) {
		res += (unsigned)str[i] + res * 2;
		res %= OTHER_PRIME;
	}
	return res + 1;
}

int AddTable(Htable* ht, char* str) {
	if (FindTable(ht, str))
		return 0;
	unsigned x, y;
	unsigned htSize = ht->size;
	x = Func1(str, htSize);
	y = Func2(str, htSize);
	for (unsigned i = 0; i < htSize; i++) {
		if (ht[x].status == NOT_INTENTED || ht[x].status == FREE) {
			if (ht[x].status == FREE) {
				free(ht[x].str);
				ht[x].str = NULL;
			}
			ht[x].str = (char*)calloc(strlen(str) + 1, sizeof(char));
			if (!ht[x].str)
				return 0;
			strcpy(ht[x].str, str);
			ht[x].status = NOT_FREE;
			return 1;
		}
		else if (ht[x].status == NOT_FREE && strcmp(ht[x].str, str) == 0)
			return 0;
		else
			x = (x + y) % htSize;
	}
	return 0;
}

int FindTable(Htable* ht, char* str) {
	unsigned x, y;
	unsigned htSize = ht->size;
	x = Func1(str, htSize), y = Func2(str, htSize);
	for (unsigned i = 0; i < htSize; i++) {
		if (ht[x].status == NOT_FREE && strcmp(ht[x].str, str) == 0)
			return 1;
		else if (ht[x].status == NOT_INTENTED)
			return 0;
		x = (x + y) % htSize;
	}
	return 0;
}

int RemoveTable(Htable* ht, char* str) {
	unsigned x, y;
	unsigned htSize = ht->size;
	x = Func1(str, htSize);
	y = Func2(str, htSize);
	for (unsigned i = 0; i < htSize; i++) {
		if (ht[x].status == NOT_FREE && strcmp(ht[x].str, str) == 0) {
			ht[x].status = FREE;
			return 1;
		}
		else if (ht[x].status == NOT_INTENTED)
			return 0;
		x = (x + y) % htSize;
	}
	return 0;
}

void DeleteTable(Htable* ht) {
	if (ht) {
		for (int i = 0; i < ht->size; i++)
			if (ht[i].status != NOT_INTENTED)
				free(ht[i].str);
		free(ht);
	}
}

void MemoryLeaks(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}

int main() {
	char query[STR_SIZE + 2] = " ";
	char command;
	char str[STR_SIZE] = { 0 };
	const int TABLE_SIZE = 999983;
	Htable* ht;
	if (!Init(&ht, TABLE_SIZE))
		return 1;
	while (fgets(query, STR_SIZE, stdin)) {
		sscanf(query, "%c%s", &command, &str);
		switch (command) {
		case 'a':
			AddTable(ht, str);
			break;
		case 'r':
			RemoveTable(ht, str);
			break;
		case 'f':
			if (FindTable(ht, str))
				fprintf(stdout, "yes\n");
			else
				fprintf(stdout, "no\n");
			break;
		default:
			DeleteTable(ht);
			MemoryLeaks();
			return 0;
		}
	}
	DeleteTable(ht);
	MemoryLeaks();
	return 0;
}