/****************************************************************************************
                                          FIIT STU
Predmet: Datove struktury a algoritmy
Semester: LS 2020/2021
Autor: Roland Vdovjak

                                         Zadanie 03

Funkcia programu: Binarny rozhodovaci strom
Casti: BDD_create, BDD_reduce, BDD_use
Deadline: 03.05.2021
*****************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define N 13         // Pocet vstupnych premennych premennych
#define REPEAT 20           // Pocet opakovani testu
#define VLENGHT pow(2,N)        // Dlzka vektora

long cnt = 0;

    // Node
typedef struct node {
    char* val;      // Hodnota
    struct node* n_left, * n_right;     // Potomkovia
} NODE;

    // Binary Decision Diagram
typedef struct bdd {
    int var_num;        // Pocet premennych
    int size;       // Velkost BDD, resp pocet vrcholov
    NODE* root;         // Smernik na koren
} BDD;

    // Vektor
typedef struct bf {
    char* vector;
} BF;

        //..........POMOCNE..FUNKCIE..........//

    // Vytvaranie vektora
char* getBF() {
    int len = pow(2, N);
    char* vector = malloc(len);

    for (int i = 0; i < len; i++) {
        if (rand() % 2 > 0) {
            vector[i] = '0';
        }
        else {
            vector[i] = '1';
        }
    }
    vector[len] = '\0';

    return vector;
}

    // Kopirovanie od n-teho po m-ty znak stringu
char* subString(char* string, int n, int m) {
    int len = m - n;

    char* sub = malloc(len+1);
    sub[len] = '\0';

    for (int i = 0; i < len; i++) {
        *(sub + i) = *(string + n + i);
    }

    return sub;
}

    // Vytvorenie stromu, ktoreho koren sa priradi BDD strukture
NODE* createDiagram(BF* bfunkcia) {

    NODE* root = malloc(sizeof(NODE));      // Alokovanie noveho vrcholu
    root->val = bfunkcia;       // Priradovanie hodnoty vrcholu
    root->n_left = root->n_right = NULL;    // Prednastavenie childov na NULL

    int vLen = strlen(bfunkcia);        // Dlzka aktualnej bFunkcie

    if (vLen != 1) {    //Opakuje sa pokym nie sme v listoch
        if (root->n_left == NULL) {
            root->n_left = createDiagram(subString(bfunkcia, 0, vLen / 2));       // Lava cast podstromu
        }

        if (root->n_right == NULL) {
            root->n_right = createDiagram(subString(bfunkcia, vLen / 2, vLen));      // Prava cast podstromu
        }
    }
    return root;
}

    // Pomocna funkcia na vypis stromu
void BDD_print(NODE* root) {
    
     if (root->n_left != NULL) {
         BDD_print(root->n_left);
     }

     printf("%s ", root->val);

     if (root->n_right != NULL) {
         BDD_print(root->n_right);
    }
}

    // Pomocna funkcia na vytvorenie pola unikatnych vrcholov podla ich hodnoty
void BDD_fillArray(int* counter, NODE* root, NODE** array) {

    if (root->n_left != NULL) {
        BDD_fillArray(counter, root->n_left, array);
    }
        
        // Porovnava, ci sa hodnoty zhoduju
    for (int i = 0; i < *counter; i++) {
        if (strcmp(array[i]->val, root->val) == 0) return;
    }
    array[*counter] = root;     // Priradi na podlednu poziciu v liste dany unikatny vrchol
    (*counter)++;

    if (root->n_right != NULL) {
        BDD_fillArray(counter, root->n_right, array);
    }
}

    // Pomocna funkcia na uvolnovanie pamate
void Child_Diagram_Free(NODE* child) {
    cnt++;
    if (child->n_left != NULL) {
        Child_Diagram_Free(child->n_left);
        Child_Diagram_Free(child->n_right);
        free(child);
        child = NULL;       
    }
}

        // Pomocna funkcia 
void reduce1(int* counter, NODE* root, NODE** array) {

    NODE* rCh, * lCh;
    rCh = root->n_right;
    lCh = root->n_left;

    if (root->n_left != NULL) {
        for (int i = 0; i < *counter; i++) {        // Counter predstavuje velkost pola s unikatnymi vrcholmi

                // Ak sa hodnoty rovnaju a zaroven sa vrcholy nerovnaju, nie je potrebne mat obidva
            if ((strcmp(array[i]->val, lCh->val) == 0) && (lCh != array[i])) {
                Child_Diagram_Free(root->n_left);
                root->n_left = array[i];
                lCh = array[i];
            }

            if ((strcmp(array[i]->val, rCh->val) == 0) && (rCh != array[i])) {
                Child_Diagram_Free(root->n_right);
                root->n_right = array[i];
                rCh = array[i];
            }
        }
    }

    if (root->n_left != NULL) {
        reduce1(counter, root->n_left, array);
    }

    if (root->n_right != NULL) {
        reduce1(counter, root->n_right, array);
    }

}


void reduce2(int* counter, NODE* root, NODE** test) {

    NODE* rCh, * lCh;
    rCh = root->n_right;
    lCh = root->n_left;
    bool b = false;

    if (lCh->n_left != NULL && rCh->n_right != NULL) {
        
            // Ak sa potomkovia potomka rovnaju, potomka mozme preskocit
        if (lCh->n_left == lCh->n_right) {

            root->n_left = lCh->n_left;  
            test[*counter] = lCh;
            lCh = root->n_left;             
            (*counter)++;
        }

        if (rCh->n_left == rCh->n_right) {

            root->n_right = rCh->n_right;
            test[*counter] = rCh;       
            rCh = root->n_right;
            (*counter)++;
        }
    }

    if (lCh->n_left != NULL) {
        reduce2(counter, root->n_left, test);
    }

    if (rCh->n_right != NULL) {
        reduce2(counter, root->n_right, test);
    }
}

        //..........C.R.E.A.T.E..........//

BDD* BDD_create(BF* bfunkcia) {
    int vLen = strlen(bfunkcia), level = -1, levelLen = 0;
    int j;
    BDD* diagram;

        // Overenie korektnosti vstupu
    if (vLen == pow(2, N)) {

        diagram = (BDD*) malloc(sizeof(BDD));      // Alokacia
            // Pociatocne priradenie hodnot
        diagram->var_num = N;       
        diagram->root = createDiagram(bfunkcia);
        diagram->size = pow(2, N + 1) - 1;

    }
    else {
        printf("ERROR - vector is not 2^N");
        return;
    }

    return diagram;
}
    
        //.............U.S.E.............//

int BDD_use(BDD* bdd, char* vstupy) {

    if (bdd->root == NULL) return -1;

    NODE* temp = bdd->root;
    int i = 0, val = -1, height = 0;
    bool found = false;

    while (i < strlen(vstupy) && !found) {      // Pokial nenasiel alebo nepresiel cez presny pocet vrcholov
        
        if (vstupy[height] == '1') {         // Cesta cez strom, dolava ak == '0', doprava ak =='1'
            temp = temp->n_right;         
        }
        else {
            temp = temp->n_left;          
        }
        
        if (temp->n_left == NULL) found = true;     // Nasiel
        i++;
        height++;
    }

    val = (strcmp(temp->val, "1")) ? 0 : 1;     // Priradenie koncovej honoty

    return val;
}

        //..........R.E.D.U.C.E..........//

int BDD_reduce(BDD* bdd) {

    if (bdd->root == NULL) return -1;

    int counter = 0, freeCounter = 0, l = 0;

        // Polia na vrcholy
    NODE** BDD_uniqueArray = (NODE**) malloc(sizeof(NODE*) * (bdd->size - pow(2, N) + 2));
    NODE** Same_Val_Nodes = (NODE**) malloc(sizeof(NODE*) * (bdd->size - pow(2, N) + 2));

        // Naplnenie pola
    BDD_fillArray(&counter, bdd->root, BDD_uniqueArray);

        // Redukovanie duplicitnych vrcholov
    reduce1(&counter, bdd->root, BDD_uniqueArray);
    bdd->size = counter;

        // Redukovanie vrcholov s rovnakymi potomkami
    reduce2(&freeCounter, bdd->root, Same_Val_Nodes);

        // Vrcholy ktore treba uvolnit z reduce2
    NODE** Same_Val_Nodes_Free = (NODE**)malloc(sizeof(NODE*) * freeCounter);

    for (int j = 0; j < freeCounter; j++) {
        cnt++;
        bdd->size--;
        for (int k = 0; k < l; k++) {
            if (Same_Val_Nodes_Free[k] == Same_Val_Nodes[j])
                break;           
        }
        
        Same_Val_Nodes_Free[l] = Same_Val_Nodes[j];
        //free(Same_Val_Nodes[j]);
        l++;
    }

    return VLENGHT - bdd->size;
}


//...................M.A.I.N.....................//

int main() {
    srand(time(0));

    int vrchol = (pow(2, N + 1) - 1), a;
    float avgP = 0, avgT = 0, avgTU = 0, time_used, time_usedU;
    BDD* diagram;
    clock_t start, end, startU, endU;
    float per;
    
    char** u = malloc(sizeof(char*) * VLENGHT);
    for (int i = 0; i < VLENGHT; i++) {
        u[i] = malloc(sizeof(char) * N + 1);
    }
    for (int j = 0; j < VLENGHT; j++) {
        for (int i = 0; i < N; i++) {
            if (rand() % 10 > 4) {
                u[j][i] = '0';
            }
            else {
                u[j][i] = '1';
            }
            u[j][N] = '\0';
        }

    }
    
    for (int i = 0; i < REPEAT; i++) {
        
start = clock();       
        diagram = BDD_create(getBF());
        BDD_reduce(diagram);
end = clock();

startU = clock();
        for(int j = 0; j < pow(2, N); j++){
           a = BDD_use(diagram, u[j]);
           if (a == -1) printf("Er");
        }
endU = clock();

        per = ((float)(diagram->size)/vrchol) * 100;
        
        printf("povodne : %d \nredukovane : %d\npercenta : %lf\n", vrchol, diagram->size, per);
        cnt = 0;
        avgP += per;
        time_used = ((double)(end - start));
        time_usedU = ((double)(endU - startU));
        avgT += time_used;
        avgTU += time_usedU;
        printf("Time used : %gms\n\n", time_used);
    }
    
    printf("AVG %% : %lf\nAVG TIME CREATE & REDUCE : %gms\nAVG TIME USE : %gms\n", avgP / REPEAT, avgT / REPEAT, avgTU / REPEAT);


    return 0;
}
