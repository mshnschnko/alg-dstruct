#include "hash_23.h"

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
			ht[x].str = (char*)calloc(strlen(str) + 1, sizeof(char));
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
	if (ht)
		free(ht);
	return;
}