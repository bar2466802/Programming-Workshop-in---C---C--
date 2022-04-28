/**
 * Ex3 - Bar Melinarskiy - 318189982
 * Exercise 3 - RB Tree implementation
 */
#ifndef RB_TREE_C
#define RB_TREE_C

#include "RBTree.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SUCCESS_CODE 1
#define ERROR_CODE 0
#define EQUAL (0)

/**
 * Constructs a new RBTree with the given CompareFunc and freeFunc.
 * @param comp: a function two compare two variables.
 * @param freeFunc: pointer to a function that frees a data item.
 * @return: the newly constructed tree, NULL if the tree couldn't be constructed
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
	//Check the given functions are valid
	if(compFunc == NULL || freeFunc == NULL)
	{
		return NULL;
	}
	//Ask for dynamic memory for the new tree
	RBTree* tree = (RBTree*)malloc(sizeof(RBTree)); //new tree pointer
	if(tree == NULL) //check the memory allocation was successful
	{
		return NULL;
	}
	tree->root = NULL; //initialize the root node
	tree->compFunc = compFunc; //set the compare function
	tree->freeFunc = freeFunc; //set the free function
	tree->size = 0; //initialize the tree size
	return tree;
}
/**
 * Create a new node to insert to the tree
 * @param parent: the new node's parent-node.
 * @param data: item to add to the tree.
 * @return: the new node, NULL if the creation was unsuccessful.
 */
Node* createNewNode(Node* parent, void *data)
{
	if(data == NULL) //Make sure the data isn't null before we create the new node
	{
		return NULL;
	}
	//Ask for dynamic memory for the new node
	Node* node = (Node*)malloc(sizeof(Node)); //new node pointer
	if(node == NULL) //check the memory allocation was successful
	{
		return NULL;
	}
	//Set the node's values
	node->color = RED;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
	node->data = data;
	return node;
}

/**
 * Get the node with the given data, if exist in the tree.
 * @param tree: the tree to check an item in.
 * @param data: info to check if exist.
 * @return: NULL if the item is not in the tree, the Node if it is.
 */
Node* rBTreeSearch(const RBTree *tree, const void *data)
{
	if(tree == NULL || data == NULL) //check the given pointers are valid
	{
		return NULL;
	}

	Node* currentNode = tree->root;
	while(currentNode != NULL && currentNode->data != NULL)
	{
		int compResult = tree->compFunc(currentNode->data, data);
		if(compResult == EQUAL)
		{
			return currentNode; //found the node with the given data
		}

		if(compResult < EQUAL)
		{
			//current nod has a lower value so check right son
			currentNode = currentNode->right;
		}
		else
		{
			//current nod has a higher value so check left son
			currentNode = currentNode->left;
		}
	}

	return NULL;
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to check an item in.
 * @param data: info to check if exist.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
	Node* node = NULL;
	node = rBTreeSearch(tree, data);
	if(node == NULL)
	{
		return ERROR_CODE;
	}
	return  SUCCESS_CODE;
//	if(tree == NULL || data == NULL) //check the given pointers are valid
//	{
//		return ERROR_CODE;
//	}
//	Node* currentNode = tree->root;
//	int compResult;
//	//do a regular binary tree search using the given comp func
//	while(currentNode != NULL)
//	{
//		compResult = tree->compFunc(data, currentNode->data);
//		if(compResult == EQUAL)
//		{
//			return SUCCESS_CODE; //found the node with the given data
//		}
//		else if(compResult < EQUAL)
//		{
//			//current nod has a higher value so check left son
//			currentNode = currentNode->left;
//		}
//		else
//		{
//			//current nod has a lower value so check right son
//			currentNode = currentNode->right;
//		}
//	}
//	return ERROR_CODE;
}
/**
 * Left rotate the tree with the given node as the pivot point
 * @param tree: the tree to change.
 * @param currentNode: the node to left rotate around.
 */
static void leftRotateRBTree(RBTree *tree, Node* currentNode)
{
	Node* y = currentNode->right; //set y
	currentNode->right = y->left; //turn y's left subtree into currentNode's right subtree
	if(y->left != NULL)
	{
		y->left->parent = currentNode;
	}
	y->parent = currentNode->parent; //link currentNode's parent to y
	if(currentNode->parent == NULL)
	{
		tree->root = y;
	}
	else if(currentNode == currentNode->parent->left)
	{
		currentNode->parent->left = y;
	}
	else
	{
		currentNode->parent->right = y;
	}
	y->left = currentNode; //put currentNode on y's left
	currentNode->parent = y;
}
/**
 * Right rotate the tree with the given node as the pivot point
 * @param tree: the tree to change.
 * @param currentNode: the node to right rotate around.
 */
static void rightRotateRBTree(RBTree *tree, Node* currentNode)
{
	Node *y = currentNode->left; //set y
	currentNode->left = y->right; // turn y's right subtree into currentNode's left subtree

	if (y->right != NULL)
	{
		y->right->parent = currentNode;
	}
	y->parent = currentNode->parent; //link currentNode's parent to y
	if(currentNode->parent == NULL)
	{
		tree->root = y;
	}
	else if(currentNode == currentNode->parent->right)
	{
		currentNode->parent->right = y;
	}
	else
	{
		currentNode->parent->left = y;
	}
	y->right = currentNode; //put currentNode on y's right
	currentNode->parent = y;
}

/**
 * Fix tree after a new node was inserted
 * @param tree: the tree to fix.
 * @param newNode: the newly added node.
 */
void fixUpAfterInsertToRBTree(RBTree *tree, Node* newNode)
{
	Node    *currentNode = newNode,
			*parent = NULL, *grandParent = NULL, *uncle = NULL;
	//Fix the node's tree coloring so the red-black roles will still be valid
	while((currentNode != tree->root) && (currentNode->parent->color == RED)
		  && (currentNode->color == RED))
	{
		parent = currentNode->parent;
		grandParent = parent->parent;
		if(parent == grandParent->left)
		{
			uncle = grandParent->right;

			if(uncle != NULL && uncle->color == RED) // case 1
			{
				parent->color = BLACK;
				uncle->color = BLACK;
				grandParent->color = RED;
				currentNode = grandParent;
			}
			else
			{
				if(currentNode == parent->right) //case 2
				{
					leftRotateRBTree(tree, parent);
					currentNode = parent;
					parent = currentNode->parent;
				}
				rightRotateRBTree(tree, grandParent); // case 3
				parent->color = BLACK;
				grandParent->color = RED;
				currentNode = parent;
			}
		}
		else
		{
			uncle = grandParent->left;
			if(uncle != NULL && uncle->color == RED) // case 1
			{
				parent->color = BLACK;
				uncle->color = BLACK;
				grandParent->color = RED;
				currentNode = grandParent;
			}
			else
			{
				if(currentNode == parent->left) // case 2
				{
					rightRotateRBTree(tree, parent);
					currentNode = parent;
					parent = currentNode->parent;
				}
				leftRotateRBTree(tree, grandParent); // case 3
				parent->color = BLACK;
				grandParent->color = RED;
			}
		}
	}

	tree->root->color = BLACK;
}
/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data)
{
	if(tree == NULL || data == NULL) //check the given pointers are valid
	{
		return ERROR_CODE;
	}

	int compResult;
	Node *currentNode = NULL,
		 *parent = NULL;

	//Check if the node already exist inside the tree, else find it's right place
	currentNode = tree->root;
	parent = NULL;
	while (currentNode != NULL)
	{
		parent = currentNode;
		compResult = tree->compFunc(data, currentNode->data);
		if (compResult == EQUAL)
		{
			return ERROR_CODE; //there's already a node inside the tree with the given data
		}
		//Check whether the current node is greater/smaller then the new node
		else if(compResult > EQUAL)
		{
			currentNode  = currentNode->right; //Update the pointer to keep searching down the tree
		}
		else
		{
			currentNode  = currentNode->left; //Update the pointer to keep searching down the tree
		}
	}

	//If we reached this point then there isn't a node already in the tree with the given data
	Node* newNode = createNewNode(parent, data);
	if(newNode == NULL)
	{
		return ERROR_CODE; //the creation has failed
	}
	if (parent != NULL) //insert the new node, linked to the parent if such parent exist
	{
		if(compResult > EQUAL)
		{
			parent->right = newNode;
		}
		else
		{
			parent->left = newNode;
		}
	}
	else // the tree is currently empty, meaning the new node is the tree's root
	{
		tree->root = newNode;
	}
	tree->size++; //update tree size
	fixUpAfterInsertToRBTree(tree, newNode); //fix the tree so the coloring are still valid
	return SUCCESS_CODE;
}
/**
 * Get the min node inside the tree from the current node
 * @param data: node to start the search from, usually the root node.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
static Node* rBTreeMinimum(Node* node)
{
	if(node == NULL) //check node pointer is valid
	{
		return NULL;
	}
	//Loop through the left subtree
	for(; node->left != NULL; node = node->left)
	{
		return node;
	}
	return NULL;
}
/**
 * Fix tree after a node was removed
 * @param tree: the tree to fix.
 * @param x: the removed node.
 */
static void rbTreeDeleteFixUp(RBTree* tree, Node* x)
{
	Node* w;
	while( x != tree->root && x->color == BLACK)
	{
		if(x == x->parent->left)
		{
			w = x->parent->right;
			if(w->color == RED) // case 1
			{
				w->color = BLACK;
				x->parent->color = RED;
				leftRotateRBTree(tree, x->parent);
				w = x->parent->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK) // case 2
			{
				w->color = RED;
				x = x->parent;
			}
			else
			{
				if(w->right->color == BLACK) // case 3
				{
					w->left->color = BLACK;
					w->color = RED;
					rightRotateRBTree(tree, w);
					w = w->parent->right;
				}
				w->color = x->parent->color; // case 4
				x->parent->color = BLACK;
				w->right->color = BLACK;
				leftRotateRBTree(tree, x->parent);
				x = tree->root;
			}
		}
		else
		{
			w = x->parent->left;
			if(w->color == RED) // case 1
			{
				w->color = BLACK;
				x->parent->color = RED;
				rightRotateRBTree(tree, x->parent);
				w = x->parent->left;
			}
			if(w->right->color == BLACK && w->left->color == BLACK) // case 2
			{
				w->color = RED;
				x = x->parent;
			}
			else
			{
				if(w->left->color == BLACK) //case 3
				{
					w->right->color = BLACK;
					w->color = RED;
					leftRotateRBTree(tree, w);
					w = w->parent->left;
				}
				w->color = x->parent->color; // case 4
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rightRotateRBTree(tree, x->parent);
				x = tree->root;
			}
		}
	}
	x->color = BLACK;
}

/**
 * get the successor of the given node
 * @param tree: the tree to search in.
 * @param node: the node to search it's successor.
 * @return: The successor node. (if no such node exist than returning NULL).
 */
Node* treeSuccessor(Node *node)
{
	if(node->right != NULL) //check if node has a right subtree
	{
		return rBTreeMinimum(node);
	}
	Node* y = node->parent;
	while(y != NULL && node == y->right)
	{
		node = y;
		y = y->parent;
	}
	return y;
}
/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
	if(tree == NULL || data == NULL) //check pointers are valid
	{
		return ERROR_CODE;
	}

	Node* nodeToDelete = rBTreeSearch(tree, data); //check if such node exist
	if(nodeToDelete == NULL)
	{
		return ERROR_CODE; //no such node exist inside the given tree
	}
	Node* z = NULL;
	Node* y = NULL;

	if(nodeToDelete->left == NULL || nodeToDelete->right == NULL)
	{
		y = nodeToDelete;
	}
	else
	{
		y = treeSuccessor(nodeToDelete);
	}
	if(y->left != NULL)
	{
		z = y->left;
	}
	else
	{
		z = y->right;
	}
	z->parent = y->parent;
	if(y->parent == NULL)
	{
		tree->root = z;
	}
	else if(y == y->parent->left)
	{
		y->parent->left = z;
	}
	else
	{
		y->parent->right = z;
	}
	if(y != nodeToDelete)
	{
		nodeToDelete->data = y->data; //copy y's satellite data into z
	}
	if(y->color == BLACK)
	{
		//delete y from the tree and free the dynamic memory
		tree->freeFunc(y->data);
		free(y);
		tree->size--; //update tree's size
		y = NULL;
		rbTreeDeleteFixUp(tree, z); // fix nodes coloring after the deletion
	}
	return SUCCESS_CODE;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order.
 * if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function
 * (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
	if(tree == NULL || func == NULL) //check pointers are valid
	{
		return ERROR_CODE;
	}

	bool leftDone = false;
	Node* currentNode = tree->root;

	// Start traversal from root
	while (currentNode != NULL)
	{
		// If left child is not traversed, find the leftmost child
		if (leftDone == false)
		{
			while (currentNode->left)
			{
				currentNode = currentNode->left;
			}
		}

		// Call the given function
		if(func(currentNode->data, args) == ERROR_CODE)
		{
			return  ERROR_CODE; //an error occurred while calling the function
		}

		leftDone = true; // Mark the left subtree as done
		// Check If right child exists
		if (currentNode->right)
		{
			leftDone = false;
			currentNode = currentNode->right;
		}
		else if (currentNode->parent) // If right child doesn't exist, move to parent
		{
			// If this node is right child of its parent, visit parent's parent first
			while (currentNode->parent && currentNode == currentNode->parent->right)
			{
				currentNode = currentNode->parent;
			}
			if (currentNode->parent == NULL)
			{
				break;
			}

			currentNode = currentNode->parent;
		}
		else
		{
			break;
		}
	}
	return  SUCCESS_CODE;
}
/**
 * Delete the given tree and free it's dynamic memory
 * @param tree: the tree to delete
 * @param node: the current node in the deletion process
 */
static void destroyRBTree(RBTree **tree, Node** node)
{
	Node *l, *r;

	if(node == NULL || (*node) == NULL) //check given pointers are valid
	{
		return;
	}
	//Update son's pointers
	l = (*node)->left;
	r = (*node)->right;
	//Call the given free function
	(*tree)->freeFunc((*node)->data);
	free((*node));
	(*node) = NULL;
	//Continue down the subtrees until we are done
	destroyRBTree(tree, &l);
	destroyRBTree(tree, &r);
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
	if(tree == NULL || *tree == NULL) //check the given tree pointer is valid
	{
		return;
	}
	Node* root = (*tree)->root;
	destroyRBTree(tree, &root); //call the function who delete and free the dynamic memory
	free(*tree);
	*tree = NULL;
}

#endif // RB_TREE_C