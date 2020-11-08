#include <iostream>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct node{
   int serial;
   struct node *left;
   struct node *right;
};

typedef struct node Node;

int wentDownOnInsertion = 0;
int wentDownOnSearch = 0;
int wentDownOnDelete = 0;

Node* initTree() {
    return NULL;
}

Node* createNode(int value) {
   Node* node = new Node();
   node->serial = value;
   node->right = NULL;
   node->left = NULL;

   return node; 
}

Node* insertNode(Node* root, int element) {
   if (root == NULL) {
       root = createNode(element);
   } else if (element > root->serial) {
       wentDownOnInsertion++;
       root->right = insertNode(root->right,element);
   } else if (element < root->serial) {
       wentDownOnInsertion++;
       root->left = insertNode(root->left,element); 
   }

   return root;
}

void freeTree(Node* root) {
   if (root != NULL) {
       freeTree(root->left);
       freeTree(root->right);
       free(root);
   }
}

int searchNode(Node* root, int value) {
   bool dec = false;
   
   if (root != NULL) {
      
      if (value == root->serial) {
           dec = true;
      } else if (value < root->serial) {
           wentDownOnSearch++;
           dec = searchNode(root->left,value); 
      } else if (value >= root->serial) {
           wentDownOnSearch++;
           dec = searchNode(root->right,value);
      }
   }

   return dec;
}

Node* minNodeValue(Node* node) { 
    Node* currentNode = node; 
  
    while (currentNode && currentNode->left != NULL) 
        currentNode = currentNode->left; 
  
    return currentNode; 
}

Node* deleteNode(Node* root, int value) { 
    if (root == NULL) {
        return root;
    }  
  
    if (value < root->serial) {
        wentDownOnDelete++;
        root->left = deleteNode(root->left, value); 
    } else if (value > root->serial) {
        wentDownOnDelete++;
        root->right = deleteNode(root->right, value); 
    } else { 
        if (root->left == NULL) { 
            Node* temp = root->right; 
            free(root); 
            return temp; 
        } else if (root->right == NULL) { 
            Node* temp = root->left; 
            free(root); 
            return temp; 
        } 
  
        Node* temp = minNodeValue(root->right); 
        root->serial = temp->serial; 
        root->right = deleteNode(root->right, temp->serial); 
    }

    return root; 
} 

int main() {
    auto start = high_resolution_clock::now();

    srand(time(NULL));

    Node *root = initTree();

    int treeSize = 100;
    for (int i = 0 ; i < treeSize ; i++) {
       int randomNumber = rand() % 1000 + 1;
       root = insertNode(root, randomNumber);
    }

    int randomNumberToSearch = rand() % 1000 + 1;
    searchNode(root, randomNumberToSearch);
    
    int randomNumberToDelete = rand() % 1000 + 1;
    root = deleteNode(root, randomNumberToDelete);

    cout << "Quantidade de vezes que desceu na insercao: " << wentDownOnInsertion;
    cout << "\nQuantidade de vezes que desceu na busca: " << wentDownOnSearch;
    cout << "\nQuantidade de vezes que desceu na remocao: " << wentDownOnDelete;

    freeTree(root);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "\n" << duration.count() << " microsegundos";

    return 0;
}