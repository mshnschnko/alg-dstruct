#pragma once
#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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