/******************************************************************************
	Hlavièka pre myHash
*******************************************************************************/
#include <math.h>
#include <stdbool.h>

typedef struct ITEM {
	long int key;
	int val;
	struct ITEM* next;
} item;

bool Hinsert(item** table, long val, long key);
long Hsearch(item* table, long key);
void Hprint(item* table);
item* initT(unsigned long size);
item* resize(item* table);