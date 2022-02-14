#include "myHash.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned long s;
unsigned long used = 0;

// Inicializacia pamate pre hasovaciu tabulku
item* initT(unsigned long size) {
	size = (size/6 + 1) * 6 + 1;		// Premenenie zadanej velkosti na prvocislo
	item * table = (item*) malloc(size * sizeof(item));
	item * tableitem = (item*) malloc(sizeof(item));

		tableitem->val = -1;
		tableitem->next = NULL;
		tableitem->key = NULL;

	for (int i = 0; i < size; i++) {		// Naplnenie struktury
		*(table + i) = *tableitem;
	}
		s = size;
	return table;
}

 long int Hfunction(long int key) {		// Hasovacia funkcia
	return key % s;
}

bool Hinsert(item* table, long val, long key) {		// Vkladanie do trabulky
	if (table == NULL) return false;
	
	item* it = malloc(sizeof(item));
	it->key = key;
	it->val = val;
	it->next = NULL;

	long index = Hfunction(it->key);
	item* temp;
	temp = table + index;

	if (table[index].val != -1) {				// Ak je index v tabulke obsadeny, pripise sa na koniec spajaneho zoznamu struktur- bucketu
		while (temp != NULL && temp->next != NULL) { 
			temp = temp->next;	// postupovanie v strukture
		} temp->next = it;
	}
	else {
		table[index] = *it;
		used++;
	}
	return true;
}

long Hsearch(item* table, long key) {
	long index = Hfunction(key);
	item* bucket = &table[index];
	
	if (table[index].key == key) return table[index].val;		// Ak je v buckete len jeden prvok, vrati jeho hodnotu
	else while (bucket->key != key) {	// Kym sa kluce nerovnaju
		bucket = bucket->next;		// Posuva sa po spajanej strukture
	} 
	
	return bucket->val;
}

void Hprint(item* table) {		// Vypis pre lepsiu vyzualizaciu
	for (int i = 0; i < s; i++) {
		if (table[i].val == -1) printf("%d\t\t---\n", i);
		else if (table[i].next == NULL)printf("%d\t\th: %ld (k: %ld)\n", i, table[i].val, table[i].key);
		else {
			item* tmp = table;
			tmp += i;
			printf("%d\t\t", i);
			while (tmp != NULL) {
				printf("h: %ld (k: %ld)\t\t- ", tmp->val, tmp->key);
				tmp = tmp->next;
			} printf("\n");
		}
	}
	printf("used buckets :%ld", used);
}

