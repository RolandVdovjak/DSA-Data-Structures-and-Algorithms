/****************************************************************************************
                                          FIIT STU
Predmet: Datove struktury a algoritmy
Semester: LS 2020/2021
Autor: Roland Vdovjak

                                         Zadanie 02

Funkcia programu: Spravca pamati
Casti: Binarny vyhladavaci strom, hasovacia tabulka
Deadline: 05.04.2021
*****************************************************************************************/

#include "myBVS.h"
#include "takenBVS.h"
#include "myHash.h"
#include "takenHash.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int size = 100;

int main() {

    srand(time(NULL));
    clock_t start, end;
    double time_used;

                    /************************************
              
                              TESTOVANIE BVS

                    *************************************/
    printf("\n\n*************        TESTOVANIE HASH BVS        *************\n");
    node* g = NULL, * t;
    int i;
    long int j = 0;
    const int max = pow(10, 5); // 25000000;        // Maxim·lna veækosù BVS
    
    //MY BVS- AVL
                //. INSERT .//

    start = clock();
    for (i = 0; i < max; i++) {             // Naplnenie
        j = ((j + 2) % max) + 1;
        g = insert(j, g);
    }
    end = clock();
    time_used = ((double)(end - start));
    printf("Cas potrebny na naplnenie %ld vrcholoveho AVL stromu: %gms\n", max, time_used);
   
                //. SEARCH .//

    start = clock();
    for (i = 0; i < max; i++) {             // Prehæadanie
        j = ((j + 2) % max) + 1;
        t = search(j, g);
    }
    end = clock();
    time_used = ((double)(end - start));
    printf("Cas potrebny na prehladanie %ld vrcholoveho AVL stromu: %gms\n", max, time_used);


    //TAKEN BVS- RED-BLACk
                //. INSERT .//
    
    start = clock();
    for (i = 0; i < max; i++) {
        j = ((j + 2) % max) + 1;
        insertion(j);
        //printf("%d ", j);

    }
    end = clock();
    time_used = ((double)(end - start));
    printf("Cas potrebny na naplnenie %ld vrcholoveho RED-BLACK stromu: %gms\n", max, time_used);
        
    struct rbNode* r;
    r = (struct rbNode*)malloc(sizeof(struct rbNode));

            //. SEARCH .//

    start = clock();
    for (i = 0; i < max; i++) {
        j = ((j + 2) % max) + 1;
        r = rbSearch(j);
    }
    end = clock();
    time_used = ((double)(end - start));

    printf("Cas potrebny na prehladanie %ld vrcholoveho RED-BLACK stromu: %gms\n", max, time_used);
    

                    /************************************
                    
                          TESTOVANIE HASH TABULKY

                    *************************************/
    printf("\n\n*************        TESTOVANIE HASH TABULKY        *************\n");

    srand(time(0));
    unsigned long tableSize = 40117;
    unsigned long items = pow(10, 5);
    unsigned long *keys = malloc(items * sizeof(long));
    item *h = initT(tableSize);
    unsigned long j2, k, num;
    int th, ht, m;

    
    // Pre rovnake vysledky testovania naplnime pole long intov, tie vkladame do hashtabulky
    for (int i = 0; i < items; i++) {
        th = rand() % 10000;
        ht = rand() % 1500;
        num = th + ht * 1000;
        keys[i] = num;
    }

    //MY HASH- Method: Chaining

    start = clock();
    for (int i = 0; i < items; i++) {
        k = keys[i];
        if (Hinsert(h, i, k));
    }
    end = clock();
    time_used = ((double)(end - start));
    printf("Cas potrebny na vlozenie %ld klucov do tabulky s Chainingom %gms\n", items, time_used);

    //Hprint(h);

    start = clock();
    for (int i = 0; i < items; i++) {
        j2 = Hsearch(h, keys[i]);
    }
    end = clock();
    time_used = ((double)(end - start));
    printf("Cas potrebny na vyhladanie %ld klucov do tabulky s Chainingom %gms\n", items, time_used);

    //TAKEN HASH- Method: Linear probing

    start = clock();
    for (int i = 0; i < items; i++) {
        k = keys[i];
        InsertLP(k);
    }
    end = clock();
    time_used = ((double)(end - start));
    printf("Cas potrebny na vlozenie %ld klucov do tabulky s Linear probingom %gms\n", items, time_used);

    //PrintList();
    start = clock();
    for (int i = 0; i < items; i++) {
        j = Search(keys[i]);
    }
    end = clock();
    time_used = ((double)(end - start));
    printf("Cas potrebny na vyhladanie %ld klucov do tabulky s Linear probingom %gms\n", items, time_used);
      
    
	return 0;
}