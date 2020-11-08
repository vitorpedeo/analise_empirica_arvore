#include <iostream>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct node { 
	int key; 
	struct node *left; 
	struct node *right; 
	int height; 
};
typedef struct node Node; 

int wentDownOnInsertion = 0;
int wentDownOnSearch = 0;
int wentDownOnDelete = 0;
int rightRotations = 0;
int leftRotations = 0;

Node* initTree() {
    return NULL;
}

int findNodeHeight(Node* node) { 
	if (node == NULL){
		return 0; 
    }

	return node->height; 
} 

int max(int a, int b) { 
	return (a > b)? a : b; 
} 

Node* newNode(int key) { 
	Node* node = new Node(); 
	
    node->key = key; 
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; 

	return node; 
} 

Node* rightRotate(Node* node) {
	rightRotations++; 
	Node* auxOne = node->left; 
	Node* auxTwo = auxOne->right; 

	auxOne->right = node; 
	node->left = auxTwo; 

	node->height = max(findNodeHeight(node->left), findNodeHeight(node->right)) + 1; 
	auxOne->height = max(findNodeHeight(auxOne->left), findNodeHeight(auxOne->right)) + 1; 

	return auxOne; 
} 

Node* leftRotate(Node* node) { 
	leftRotations++;
	Node* auxOne = node->right; 
	Node* auxTwo = auxOne->left; 

	auxOne->left = node; 
	node->right = auxTwo; 

	node->height = max(findNodeHeight(node->left), findNodeHeight(node->right)) + 1; 
	auxOne->height = max(findNodeHeight(auxOne->left), findNodeHeight(auxOne->right))+1; 

	return auxOne; 
} 

int getBalance(Node* node) { 
	if (node == NULL) {
		return 0; 
    }

	return findNodeHeight(node->left) - findNodeHeight(node->right); 
} 

Node* insertNode(Node* node, int key) { 
	if (node == NULL) {
		return(newNode(key)); 
    }

	if (key < node->key) {
		wentDownOnInsertion++;
		node->left = insertNode(node->left, key); 
    } else if (key > node->key) {
		wentDownOnInsertion++;
		node->right = insertNode(node->right, key); 
    }
	 
	node->height = 1 + max(findNodeHeight(node->left), findNodeHeight(node->right)); 
	int balance = getBalance(node); 

	if (balance > 1 && key < node->left->key) {
		return rightRotate(node); 
    }

	if (balance < -1 && key > node->right->key) {
		return leftRotate(node); 
    }

	if (balance > 1 && key > node->left->key) { 
		node->left = leftRotate(node->left); 
		return rightRotate(node); 
	} 

	if (balance < -1 && key < node->right->key) { 
		node->right = rightRotate(node->right); 
		return leftRotate(node); 
	} 

	return node; 
} 

bool searchNode(Node* root, int value) {
   bool dec = false;
   
   if (root != NULL) {
      
      if (value == root->key) {
           dec = true;
      } else if (value < root->key) {
		  wentDownOnSearch++;
           dec = searchNode(root->left,value); 
      } else if (value >= root->key) {
		  wentDownOnSearch++;
           dec = searchNode(root->right,value);
      }
   }

   return dec;
}

Node* minValueNode(Node* node) { 
	Node* current = node; 

	while (current->left != NULL) 
		current = current->left; 

	return current; 
} 

Node* deleteNode(Node* root, int key) { 
	if (root == NULL) {
		return root; 
    }

	if (key < root->key) {
		wentDownOnDelete++;
		root->left = deleteNode(root->left, key); 
    } else if(key > root->key) {
		wentDownOnDelete++;
		root->right = deleteNode(root->right, key); 
    } else { 
		if( (root->left == NULL) || (root->right == NULL) ) { 
			Node* temp = root->left ? root->left : root->right; 
		
			if (temp == NULL) { 
				temp = root; 
				root = NULL; 
			} else {
			    *root = *temp; 
			    free(temp);
            }  
		} else { 
			Node* temp = minValueNode(root->right); 
			root->key = temp->key; 
			root->right = deleteNode(root->right, temp->key); 
		} 
	} 

	if (root == NULL) {
	    return root; 
    }

	root->height = 1 + max(findNodeHeight(root->left), findNodeHeight(root->right)); 
	int balance = getBalance(root); 

	if (balance > 1 && getBalance(root->left) >= 0) {
		return rightRotate(root); 
    } 

	if (balance > 1 && getBalance(root->left) < 0) { 
		root->left = leftRotate(root->left); 
		return rightRotate(root); 
	} 

	if (balance < -1 && getBalance(root->right) <= 0) {
		return leftRotate(root); 
    } 

	if (balance < -1 && getBalance(root->right) > 0) { 
		root->right = rightRotate(root->right); 
		return leftRotate(root); 
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

int main() { 
    auto start = high_resolution_clock::now();

    srand(time(NULL));

    Node *root = initTree();

    int treeSize = 100;
    for (int i = 0 ; i < treeSize ; i++) {
       int randomNumber = rand() % 100000 + 1;
       root = insertNode(root, randomNumber);
    }

    int randomNumberToSearch = rand() % 100000 + 1;
    searchNode(root, randomNumberToSearch);
    
    int randomNumberToDelete = rand() % 100000 + 1;
    root = deleteNode(root, randomNumberToDelete);

    cout << "Quantidade de vezes que desceu na insercao: " << wentDownOnInsertion;
    cout << "\nQuantidade de vezes que desceu na busca: " << wentDownOnSearch;
    cout << "\nQuantidade de vezes que desceu na remocao: " << wentDownOnDelete;
    cout << "\nQuantidade rotacoes a direita: " << rightRotations;
    cout << "\nQuantidade rotacoes a esquerda: " << leftRotations;
    
    freeTree(root);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "\nTempo de execucao: " << duration.count() << " microsegundos";

    return 0;
} 
