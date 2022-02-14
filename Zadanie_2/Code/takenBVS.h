/******************************************************************************
    Hlavicka vytvoprena pre pracu s "takenBVS.c"
*******************************************************************************/

enum nodeColor {
    RED,
    BLACK
};

typedef struct rbNode {
    int data, color;
    struct rbNode* link[2];
} ;
struct rbNode* root;

struct rbNode* createNode(int data);
void insertion(int data);
void deletion(int data);
void inorderTraversal(struct rbNode* node);
struct rbNode* rbSearch(int data);