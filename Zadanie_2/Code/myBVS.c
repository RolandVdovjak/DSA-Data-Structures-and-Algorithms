/*********************************************************************************
    Implementacia funkcii insert a search na AVL binarnom vyhladavacom strome
*********************************************************************************/

#include <stdio.h>
#include "myBVS.h"

static int height(node* n){             // Pomocne funkcie potrebne pri otacani, vracia vysku vrchola posunuteho ako parameter
                                        // Ako parameter sa pouziva node->left alebo node->right
    if (n == NULL) return -1;           
    else return n->height;
}

static int bigger(int l, int r){        // Vrati vacsie cislo z dvoch zadanych
    return l > r ? l : r;
}

static node* R(node* par){              // Rotacia vpravo   
    node* lchild = NULL;
    
        // Rotovanie vpravo
    lchild = par->left;                             // Novy vrchol = lavy podkoren
    par->left = lchild->right;                      // Vsetko medzi starym a novym korenom patri lavej casti povodneho
    lchild->right = par;                            // Pravy podkoren noveho vrcholu = povodny vrchol
        // Nastavenie vysky
    par->height = bigger(height(par->left), height(par->right)) + 1;
    lchild->height = bigger(height(lchild->left), par->height) + 1;

    return lchild;
}

static node* L(node* par){
    node* rchild = NULL;

        // Rotovanie vpravo
    rchild = par->right;                            // Novy vrchol = pravy podkoren
    par->right = rchild->left;                      // Vsetko medzi starym a novym korenom
    rchild->left = par;                             // Lavy podkoren noveho vrcholu = povodny vrchol
        // Nastavenie vysky
    par->height = bigger(height(par->left), height(par->right)) + 1;
    rchild->height = bigger(height(rchild->right), par->height) + 1;

    return rchild;
}

static node* LR(node* par){     // Vlavo vpravo
    par->left = L(par->left);
    return R(par);
}

static node* RL(node* par){     // Vpravo vlavo
    par->right = R(par->right);
    return L(par);
}

// Vkladanie vrcholu do BS
node* insert(int k, node* g){

    if (g == NULL) {
        g = (node*)malloc(sizeof(node));
        g->key = k;
        g->height = 0;
        g->left = g->right = NULL;

    }
    else if (k < g->key) {                                             // Chod vlavo
        g->left = insert(k, g->left);
        if (height(g->left) - height(g->right) == 2)
            if (k < g->left->key) g = R(g);                    // Ak je kluc laveho nasledujuceho vrcholu vacsi ako nas kluc, rotujeme vpravo, inak vlavo vpravo
            else g = LR(g);
    }
    else if (k > g->key) {                                             // Chod vpravo
        g->right = insert(k, g->right);
        if (height(g->right) - height(g->left) == 2)
            if (k > g->right->key) g = L(g);                   // Ak je kluc praveho nasledujuceho vrcholu vacsi ako nas kluc, rotujeme vlavo, inak vpravo vlavo
            else g = RL(g);
    }

    g->height = bigger(height(g->left), height(g->right)) + 1;
    return g;
}

void inorder(node* g) {     // Pre kontrolu inorder vypis
    if (g != NULL) {
        inorder(g->left);
        printf("%4d (%d)", g->key, g->height);
        inorder(g->right);
    }return;
}

node* search(int k, node* g){           // Rekurzivne vyhladavanie na principe porovnavania vrcholov pod vræholom, kde sa program nachadza
    if (g == NULL) return NULL;
    if (k < g->key) return search(k, g->left);
    else    if (k > g->key) return search(k, g->right);
            else return g;
    
}

void showTree(node* g) {     // Pre lepsiu pracu so stromom ho zobrazi do konzoly
    if (g == NULL)
        return;
    printf("%d ", g->key);

    if (g->left != NULL) printf("(L:%d)", g->left->key);
    if (g->right != NULL) printf("(R:%d)", g->right->key);

    // printf("\t\t\t\t\th:%d", g->height);
    printf("\n");

    showTree(g->left);
    showTree(g->right);
}