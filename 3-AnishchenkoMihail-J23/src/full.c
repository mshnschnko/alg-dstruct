#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 100000

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
int DeleteTable(Htable** ht);

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
		res %= size;
	}
	return res;
}

int AddTable(Htable* ht, char* str) {
	unsigned x, y;
	unsigned htSize = ht->size;
	x = Func1(str, htSize);
	y = Func2(str, htSize);
	for (unsigned i = 0; i < htSize; i++) {
		if (ht[x].status == NOT_INTENTED || ht[x].status == FREE) {
			ht[x].str = (char*)calloc(sizeof(char), strlen(str));
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

int DeleteTable(Htable** ht) {
	free(*ht);
	*ht = NULL;
	return 1;
}

int main() {
	char query[STR_SIZE + 2] = " ";
	char command;
	char str[STR_SIZE] = { 0 };
	const int TABLE_SIZE = 1000000; //must be big prime number
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
			DeleteTable(&ht);
			return 0;
		}
	}
	DeleteTable(&ht);
	return 0;
}