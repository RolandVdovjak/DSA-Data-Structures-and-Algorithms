/******************************************************************************
    HASH TABLE WITH LINEAR PROBING zo stránky 
    https://github.com/prabaprakash/Data-Structures-and-Algorithms-Programs/blob/master/Hashing%20-%20Linear%20Probing%20(Open%20addressing).c?fbclid=IwAR0Q-3ceRk5cnhXnMtTSpDrJMjlYfVxm4VVqtZZGaPlTR6cmhwecKSHD4gU
*******************************************************************************/

#include "takenHash.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 200117

unsigned long arr[SIZE];
unsigned long count;

unsigned long CalculateHash(unsigned long key)
{
    return key % SIZE;
}

void InsertLP(unsigned long element)
{
    if (count == SIZE) {
        printf("Error.\nTable if FULL\n");
        exit(EXIT_FAILURE);
    }
    unsigned long probe = CalculateHash(element);
    while (arr[probe] != NULL && arr[probe] != -1)
    {
        probe = (probe + 1) % SIZE;
    }
    arr[probe] = element;
    count++;
}

void Delete(unsigned long element)
{
    if (count == 0) {
        printf("Error.\nTable if EMPTY\n");
        exit(EXIT_FAILURE);
    }
    unsigned long loc = Search(element);
    if (loc != -1)
    {
        arr[loc] = -1;
        count--;
    }
}

unsigned long Search(unsigned long element)
{
    if (count == 0) {
        printf("Error.\nTable if EMPTY\n");
        exit(EXIT_FAILURE);
    }
    unsigned long probe = CalculateHash(element);
    while (arr[probe] != 0)
    {
        if (arr[probe] == element)
            return probe;
        probe = (probe + 1) % SIZE;
    }
    printf("\nElement %d not found.\n", element);
    return -1;
}

void PrintList()
{
    unsigned long i;
    for (i = 0; i < SIZE; i++)
        printf("%d%c", arr[i], ' ');
}