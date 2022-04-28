/**
 * Ex3 - Bar Melinarskiy - 318189982
 * Exercise 3 - RB Tree implementation
 * @return success code - returning 0 if the program finished successfully
 */
#ifndef RB_TREE_C
#define RB_TREE_C

#include "RBTree.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define SUCCESS_CODE 1
#define ERROR_CODE 0
#define EQUAL (0)

/**
 * constructs a new RBTree with the given CompareFunc and freeFunc.
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
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to check an item in.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
	if(tree == NULL || data == NULL)
	{
		return ERROR_CODE;
	}
	Node* currentNode = tree->root;
	int compResult;
	while(currentNode != NULL)
	{
		compResult = tree->compFunc(data, currentNode->data);
		if(compResult == EQUAL)
		{
			return SUCCESS_CODE;
		}
		else if(compResult < EQUAL)
		{
			currentNode = currentNode->left;
		}
		else
		{
			currentNode = currentNode->right;
		}
	}
	return ERROR_CODE;
}


/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to check an item in.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
Node* rBTreeSearch(const RBTree *tree, const void *data)
{
	if(tree == NULL || data == NULL)
	{
		return NULL;
	}
	Node* currentNode = tree->root;

	while(currentNode != NULL && currentNode->data != NULL)
	{
		int compResult = tree->compFunc(currentNode->data, data);
		if(compResult == EQUAL)
		{
			return currentNode;
		}

		if(compResult < EQUAL)
		{
			currentNode = currentNode->right;
		}
		else
		{
			currentNode = currentNode->left;
		}
	}

	return NULL;
}

static void leftRotateRBTree(RBTree *tree, Node* currentNode)
{
	Node* y = currentNode->right;
	currentNode->right = y->left;
	if(y->left != NULL)
	{
		y->left->parent = currentNode;
	}
	y->parent = currentNode->parent;
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
	y->left = currentNode;
	currentNode->parent = y;

//	Node* rightNode = currentNode->right;
//	currentNode->right = rightNode->left;
//
//	if(rightNode->left != NULL)
//	{
//		rightNode->left->parent = currentNode;
//	}
//
//	rightNode->parent = currentNode->parent;
//
//	if(currentNode->parent == NULL)
//	{
//		tree->root = rightNode;
//	}
//	else
//	{
//		if(currentNode == currentNode->parent->left)
//		{
//			currentNode->parent->left = rightNode;
//		}
//		else
//		{
//			currentNode->parent->right = rightNode;
//		}
//	}
//	rightNode->left = currentNode;
//	currentNode->parent = rightNode;
}

static void rightRotateRBTree(RBTree *tree, Node* currentNode)
{
	Node *y = currentNode->left;

	currentNode->left = y->right;

	if (y->right != NULL)
	{
		y->right->parent = currentNode;
	}
	y->parent = currentNode->parent;
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
	y->right = currentNode;
	currentNode->parent = y;

//	Node* leftNode = currentNode->left;
//	currentNode->left = leftNode->right;
//
//	if(leftNode->right != NULL)
//	{
//		leftNode->right->parent = currentNode;
//	}
//
//	leftNode->parent = currentNode->parent;
//
//	if(currentNode->parent == NULL)
//	{
//		tree->root = leftNode;
//	}
//	else
//	{
//		if(currentNode == currentNode->parent->right)
//		{
//			currentNode->parent->right = leftNode;
//		}
//		else
//		{
//			currentNode->parent->left = leftNode;
//		}
//	}
//
//	leftNode->right = currentNode;
//	currentNode->parent = leftNode;
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

	while((currentNode != tree->root) && (currentNode->parent->color == RED) && (currentNode->color == RED))
	{
		parent = currentNode->parent;
		grandParent = parent->parent;
		if(parent == grandParent->left)
		{
			uncle = grandParent->right;

			if(uncle != NULL && uncle->color == RED)
			{
				parent->color = BLACK;
				uncle->color = BLACK;
				grandParent->color = RED;
				currentNode = grandParent;
			}
			else
			{
				if(currentNode == parent->right)
				{
					leftRotateRBTree(tree, parent);
					currentNode = parent;
					parent = currentNode->parent;
				}
				rightRotateRBTree(tree, grandParent);
				parent->color = BLACK;
				grandParent->color = RED;
				currentNode = parent;
			}
		}
		else
		{
			uncle = grandParent->left;
			if(uncle != NULL && uncle->color == RED)
			{
				parent->color = BLACK;
				uncle->color = BLACK;
				grandParent->color = RED;
				currentNode = grandParent;
			}
			else
			{
				if(currentNode == parent->left)
				{
					rightRotateRBTree(tree, parent);
					currentNode = parent;
					parent = currentNode->parent;
				}
				leftRotateRBTree(tree, grandParent);
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
		//Update the current pointer to keep searching down the tree
		//Check whether the current node is greater/smaller then the new node
		else if(compResult > EQUAL)
		{
			currentNode  = currentNode->right;
		}
		else
		{
			currentNode  = currentNode->left;
		}
	}

	//If we reached this point then there isn't a node already in the tree with the given data
	Node* newNode = createNewNode(parent, data);
	if(newNode == NULL)
	{
//		freeRBTree(&tree); //free the tree's dynamic memory before we exit
		return ERROR_CODE; //the creating has failed
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
	tree->size++;
	fixUpAfterInsertToRBTree(tree, newNode);
	return SUCCESS_CODE;
}

//static void rBTransplant(RBTree* tree, Node* u, Node* v)
//{
//	if(u->parent == NULL)
//	{
//		tree->root = v;
//	}
//	else
//	{
//		if(u == u->parent->left)
//		{
//			u->parent->left = v;
//		}
//		else
//		{
//			u->parent->right = v;
//		}
//	}
//	v->parent = u->parent;
//}

static Node* rBTreeMinimum(Node* z)
{
	for(; z->left != NULL; z = z->left);
	return z;
}

static void rbTreeDeleteFixUp(RBTree* tree, Node* x)
{
	Node* w;
	while( x != tree->root && x->color == BLACK)
	{
		if(x == x->parent->left)
		{
			w = x->parent->right;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				leftRotateRBTree(tree, x->parent);
				w = x->parent->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}
			else
			{
				if(w->right->color == BLACK)
				{
					w->left->color = BLACK;
					w->color = RED;
					rightRotateRBTree(tree, w);
					w = w->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				leftRotateRBTree(tree, x->parent);
				x = tree->root;
			}
		}
		else
		{
			w = x->parent->left;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				rightRotateRBTree(tree, x->parent);
				w = x->parent->left;
			}
			if(w->right->color == BLACK && w->left->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}
			else
			{
				if(w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color = RED;
					leftRotateRBTree(tree, w);
					w = w->parent->left;
				}
				w->color = x->parent->color;
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
	if(node->right != NULL)
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
	if(tree == NULL || data == NULL)
	{
		return ERROR_CODE;
	}

	Node* nodeToDelete = rBTreeSearch(tree, data);
	if(nodeToDelete == NULL)
	{
		return ERROR_CODE;
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
		nodeToDelete->data = y->data;
	}
	if(y->color == BLACK)
	{
		tree->freeFunc(y->data);
		free(y);
		tree->size--;
		y = NULL;
		rbTreeDeleteFixUp(tree, z);
	}
	return SUCCESS_CODE;

//	Node *y, *z, *x, *hold_node_to_delete;
//	Color y_original_color;
//
//	hold_node_to_delete = y = z = rBTreeSearch(tree, data);
//
//	if(y == NULL)
//	{
//		return ERROR_CODE; //Node doesn't exist in the tree
//	}
//
//	y_original_color = y->color;
//	if(z->left == NULL)
//	{
//		x = z->right;
//		rBTransplant(tree, z, z->right);
//	}
//	else
//	{
//		if(z->right == NULL)
//		{
//			x = z->left;
//			rBTransplant(tree, z, z->left);
//		}
//		else
//		{
//			y = rBTreeMinimum(z->right);
//			y_original_color = y->color;
//			x = y->right;
//			if(y->parent == z)
//			{
//				x->parent = y;
//			}
//			else
//			{
//				rBTransplant(tree, y, y->right);
//				y->right = z->right;
//				y->right->parent = y;
//			}
//			rBTransplant(tree, z, y);
//			y->left = z->left;
//			y->left->parent = y;
//			y->color = z->color;
//		}
//	}
//	if(y_original_color == BLACK)
//	{
//		rbTreeDeleteFixUp(tree, x);
//	}
//
//	free(hold_node_to_delete);
//	tree->size--; //update tree size
//	return SUCCESS_CODE;
}

int inOrder(const Node *node, forEachFunc func, void *args)
{
	if (node == NULL)
	{
		return SUCCESS_CODE;
	}
	/* first recur on left child */
	inOrder(node->left, func, args);
	/* then print the data of node */
	if(func(node->data, args) == ERROR_CODE)
	{
		return  ERROR_CODE;
	}

	/* now recur on right child */
	return inOrder(node->right, func, args);
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
	if(tree == NULL || func == NULL)
	{
		return ERROR_CODE;
	}

	bool leftdone = false;
	Node* root = tree->root;

	// Start traversal from root
	while (root)
	{
		// If left child is not traversed, find the
		// leftmost child
		if (!leftdone)
		{
			while (root->left)
				root = root->left;
		}

		// Print root's data
		if(func(root->data, args) == ERROR_CODE)
		{
			return  ERROR_CODE;
		}

		// Mark left as done
		leftdone = true;

		// If right child exists
		if (root->right)
		{
			leftdone = false;
			root = root->right;
		}

			// If right child doesn't exist, move to parent
		else if (root->parent)
		{
			// If this node is right child of its parent,
			// visit parent's parent first
			while (root->parent &&
				   root == root->parent->right)
				root = root->parent;
			if (!root->parent)
				break;
			root = root->parent;
		}
		else break;
	}

	return  SUCCESS_CODE;

//	Node* x = tree->root;
//	Node* y = NULL;
//	do
//	{
//		if(y == x->parent)
//		{
//			if(x->left == NULL)
//			{
//				y = NULL;
//			}
//			else
//			{
//				y = x;
//				x = x->left;
//			}
//		}
//		else if(y == x->left)
//		{
//			if(func(x->data, args) == ERROR_CODE)
//			{
//				return  ERROR_CODE;
//			}
//
//			if(x->right == NULL)
//			{
//				y = NULL;
//			}
//			else
//			{
//				y = x;
//				x = x->right;
//			}
//		}
//		else if(y == x->right)
//		{
//			y = x;
//			x = x->parent;
//		}
//	}while(x != tree->root);
//
//	return  SUCCESS_CODE;












//	Node* root = tree->root;
//	if(root == NULL)
//	{
//		return ERROR_CODE;
//	}
//
//	Node *current, *pre;
//	current = root;
//	while (current != NULL && current->data != NULL)
//	{
//		if (current->left == NULL)
//		{
//			if(func(current->data, args) == ERROR_CODE)
//			{
//				return ERROR_CODE;
//			}
//			current = current->right;
//		}
//		else
//		{
//			/* Find the inorder predecessor of current */
//			pre = current->left;
//			while (pre->right != NULL && pre->right != current)
//			{
//				pre = pre->right;
//			}
//			/* Make current as the right child of its inorder
//			   predecessor */
//			if (pre->right == NULL)
//			{
//				pre->right = current;
//				current = current->left;
//			}
//				/* Revert the changes made in the 'if' part to restore
//				   the original tree i.e., fix the right child
//				   of predecessor */
//			else
//			{
//				pre->right = NULL;
//				if(func(current->data, args) == ERROR_CODE)
//				{
//					return  ERROR_CODE;
//				}
//				current = current->right;
//			} /* End of if condition pre->right == NULL */
//		} /* End of if condition current->left == NULL*/
//	} /* End of while */
//	return  SUCCESS_CODE;
	/* first recur on left child */
//	return inOrder(root, func, args);
}

//void loopOnTree(Node *node, Node** maxNode, CompareFunc compFunc)
//{
//	if (node == NULL)
//	{
//		return;
//	}
//	/* first recur on left child */
//	loopOnTree(node->left, maxNode, compFunc);
//
//	/* then print the data of node */
//	if(compFunc(maxNode, node) == LESS)
//	{
//		(*maxNode) = &(*node);
//	}
//
//	/* now recur on right child */
//	loopOnTree(node->right, maxNode, compFunc);
//}

///**
// * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
// * function returns 0, the process stops.
// * @param tree: the tree with all the items.
// * @param func: the function to activate on all items.
// * @param args: more optional arguments to the function (may be null if the given function support it).
// * @return: 0 on failure, other on success.
// */
//Node* getMaxNodeWithProp(const RBTree *tree, CompareFunc compFunc)
//{
//	if(tree == NULL || tree->root || compFunc == NULL)
//	{
//		return NULL;
//	}
//
//	Node* currentNode = tree->root;
//	Node* maxNode = tree->root;
//	loopOnTree(currentNode, &maxNode, compFunc);
//	return maxNode;
//}

static void destroyRBTree(RBTree **tree ,Node** node)
{
	Node *l, *r;

	if(node == NULL || (*node) == NULL)
	{
		return;
	}

	l = (*node)->left;
	r = (*node)->right;

	(*tree)->freeFunc((*node)->data);
	free((*node));
	(*node) = NULL;
	destroyRBTree(tree,&l);
	destroyRBTree(tree,&r);
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
	if(tree == NULL || *tree == NULL)
	{
		return;
	}
	Node* root = (*tree)->root;
	destroyRBTree(tree, &root);
	free(*tree);
	*tree = NULL;
}

#endif // RB_TREE_C