/******************************************************************************
    Hlavi�ka pre myBVS
*******************************************************************************/

typedef struct node{            // Samostatn� strom
    unsigned int key;                        // Kluc
    struct node* left;             
    struct node* right;
    int height;                     // Vyska
} node;

node* search(int e, node* g);         // Funkcia Search
node* insert(int key, node* g);         // Funkcia Insert
void showTree(node* g);
void inorder(node* g);