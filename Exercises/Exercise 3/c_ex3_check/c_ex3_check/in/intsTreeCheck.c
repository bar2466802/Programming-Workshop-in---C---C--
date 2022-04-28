//
// Created by amit.david on 02/12/2019.
//

#include "RBTree.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct intStruct
{
    int value;
} intStruct;

int intsCmp(const void *a, const void *b)
{
    int x = ((intStruct*)a)->value, y = ((intStruct*)b)->value;
    return x - y;
}

int printWithArg(const void *value, void *text)
{
    static int count = 1;
    int x = *((int *)value);
    char *line = (char*)text;
    printf("%d: %s %d\n", count++, line, x);

    return 1;
}

int main()
{
    intStruct *arr[102];
    int temp;
    for (int i = 0; i < 102; ++i)
    {
        arr[i] = (intStruct*) malloc(sizeof(intStruct));
        temp = i % 7 ? i : -i;
        temp = temp % 9 ? temp : temp + 10;
        arr[i]->value = temp;
    }

    arr[100]->value = 3000;
    arr[101]->value = -2000;

    RBTree *tree = newRBTree(&intsCmp, &free);

    for (int i = 0; i < 98; i++)
    {
        printf("add the number %d to the tree\n", arr[i]->value);
        printf("\tinsertToRBTree return code: %d\n", insertToRBTree(tree, arr[i]));
        printf("\n");
    }

    char *text;
    int code;

    printf("\nCheck if number is in tree:\n");
    for (int i = 0; i < 102; i++)
    {
        code = RBTreeContains(tree, arr[i]);
        text = code ? "%d is in the tree\n" : "%d not in the tree\n";
        printf(text, arr[i]->value);
        if(code == 0)
        {
            free(arr[i]);
        }
    }

    printf("\nThe number of ints in the tree is %d.\n\n", tree->size);
    forEachRBTree(tree, &printWithArg, "Number stored in node is:");

    freeRBTree(&tree);

    free(arr[10]);
    free(arr[19]);
    free(arr[37]);
    free(arr[46]);
    free(arr[55]);
    free(arr[64]);
    free(arr[82]);

    printf("\nTest done\n");
}





//////
////// Created by yuval on 18/05/2020.
//////
////#include <stdlib.h>
////#include <stdio.h>
////#include "RBTree.h"
////
/////* Compute the "height" of a tree -- the number of
////    nodes along the longest path from the root node
////    down to the farthest leaf node.*/
////int height(struct Node *node)
////{
////    if (node == NULL)
////        return 0;
////    else
////    {
////        /* compute the height of each subtree */
////        int lheight = height(node->left);
////        int rheight = height(node->right);
////
////        /* use the larger one */
////        if (lheight > rheight)
////            return (lheight + 1);
////        else return (rheight + 1);
////    }
////}
////
/////* Print nodes at a given level */
////void printGivenLevel(struct Node *root, int level)
////{
////    if (root == NULL)
////        return;
////    if (level == 1)
////        printf("(%d, %d)", (*(int *) root->data), root->color);
////    else if (level > 1)
////    {
////        printGivenLevel(root->left, level - 1);
////        printGivenLevel(root->right, level - 1);
////    }
////}
////
////
/////* Function to print level order traversal a tree*/
////void printLevelOrder(struct Node *root)
////{
////    int h = height(root);
////    int i;
////    for (i = 1; i <= h; i++)
////    {
////        printGivenLevel(root, i);
////        printf("\n");
////    }
////}
////
////
////int compareInt(const void *a, const void *b)
////{
////    int *n1 = (int *) a;
////    int *n2 = (int *) b;
////    int num1 = *n1;
////    int num2 = *n2;
////
////    return num1 - num2;
////}
////
////
////void insert(RBTree *tree, int *n)
////{
////    printf("Insert- %d\n", *n);
////    int result = insertToRBTree(tree, n);
////    printf("returned value- %d\n", result);
////    printf("Tree-\n");
////    printLevelOrder(tree->root);
////    printf("\n");
////}
////
////void myDelete(RBTree *tree, int *n)
////{
////    printf("delete %d-\n", *n);
////    int result = deleteFromRBTree(tree, n);
////    printf("returned value- %d\n", result);
////    printf("Tree-\n");
////    printLevelOrder(tree->root);
////    printf("\n");
////}
////
////int myForEach(const void *object, void *args)
////{
////    int curNum = *(int *) object;
////    int arg = *(int *) args;
////    if (curNum >= arg)
////    {
////        return 0;
////    }
////    return 1;
////}
////
////int main()
////{
////    int *pNums[10];
////
////    for (int j = 0; j < 10; ++j)
////    {
////        pNums[j] = (int *) malloc(sizeof(int));
////        *(pNums[j]) = j;
////    }
////
////    RBTree *tree = newRBTree(compareInt, free);
////
////    printf("---TEST INSERT---\n");
////    //----Insert
////    insert(tree, pNums[5]);
////    insert(tree, pNums[7]);
////    insert(tree, pNums[3]);
////    insert(tree, pNums[2]);
////    insert(tree, pNums[6]);
////    insert(tree, pNums[8]);
////    insert(tree, pNums[9]);
////    insert(tree, pNums[1]);
////    insert(tree, pNums[0]);
////    insert(tree, pNums[4]);
////    printf("\n---TEST DUPLICATE INSERT---\n");
////    //Duplicate insert:
////    insert(tree, pNums[0]);
////    insert(tree, pNums[2]);
////    insert(tree, pNums[4]);
////
////    // test delete not in tree value
////    printf("--- TEST DELETE NON EXIST VALUE--\n");
////    int lala = 11;
////    int *plala = &lala;
////    myDelete(tree, plala);
////
////    printf("\n---TEST DELETE---\n");
////    //--Delete
////    myDelete(tree, pNums[5]);
////    myDelete(tree, pNums[1]);
////    myDelete(tree, pNums[4]);
////    myDelete(tree, pNums[8]);
////    myDelete(tree, pNums[0]);
////    myDelete(tree, pNums[2]);
////    myDelete(tree, pNums[9]);
////    myDelete(tree, pNums[6]);
////    myDelete(tree, pNums[7]);
////    myDelete(tree, pNums[3]);
////
////
////
////    for (int j = 0; j < 10; ++j)
////    {
////        pNums[j] = (int *) malloc(sizeof(int));
////        *(pNums[j]) = j;
////    }
////
////
////    // insert again
////    insert(tree, pNums[1]);
////    insert(tree, pNums[2]);
////    insert(tree, pNums[3]);
////    insert(tree, pNums[4]);
////    insert(tree, pNums[5]);
////    insert(tree, pNums[8]);
////    insert(tree, pNums[9]);
////    insert(tree, pNums[7]);
////    insert(tree, pNums[6]);
////    insert(tree, pNums[0]);
////
////
////    printf("\n---TEST FOREACH---\n");
////    int maxAllowed = 10;
////    int *pMaxAllowed = &maxAllowed;
////    printf("Test success in for each- returned value: %d\n", forEachRBTree(tree, myForEach, pMaxAllowed));
////    maxAllowed = 8;
////    printf("Test failure in for each- returned value: %d\n", forEachRBTree(tree, myForEach, pMaxAllowed));
////
////    printf("\n---TEST RBTreeContains---\n");
////    printf("Test exist element: %d\n", RBTreeContains(tree, pNums[0]));
////
////    printf("Test non exist element: %d\n", RBTreeContains(tree, plala));
////
////
////    printf("\n---TEST FREE_TREE---\n");
////    freeRBTree(&tree);
////
////
////    printf("TEST DONE!\n");
////}
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
//
//////
////// Created by odedw on 12-Oct-19.
//////
////
//#ifndef TA_EX3_PRODUCTEXAMPLE_C
//#define TA_EX3_PRODUCTEXAMPLE_C
//
//#include "RBTree.h"
//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//
//#define LESS (-1)
//#define EQUAL (0)
//#define GREATER (1)
//
//typedef struct ProductExample
//{
//    char *name;
//    double price;
//} ProductExample;
//
///**
// * Comparator for ProductExample
// * @param a ProductExample*
// * @param b ProductExample*
// * @return -1 if a<b, 0 if a==b, 1 if b<a
// */
//int productComparatorByName(const void *a, const void *b)
//{
//    ProductExample *first = (ProductExample *) a;
//    ProductExample *second = (ProductExample *) b;
//    double diff = strcmp(first->name, second->name);
//    if (diff < 0)
//    {
//        return LESS;
//    }
//    else if (diff > 0)
//    {
//        return GREATER;
//    }
//    else
//    {
//        return EQUAL;
//    }
//}
//
//void productFree(void *a)
//{
//    ProductExample *pProduct = (ProductExample *) a;
//    free(pProduct->name);
//    free(a);
//}
//
///**
// *
// * @param pProduct pointer to product to print
// * @param null required argument for typedef
// * @return
// */
//int printProduct(const void *pProduct, void *null)
//{
//    if (null != NULL)
//    {
//        return 0;
//    }
//    ProductExample *product = (ProductExample *) pProduct;
//    printf("Name: %s.\t\tPrice: %.2f\n", product->name, product->price);
//
//    return 1;
//}
//
///**
// *
// * @return products for tests
// */
//ProductExample **getProducts()
//{
//    char *name0 = (char *) malloc(sizeof(char) * (20));
//    char *name1 = (char *) malloc(sizeof(char) * (20));
//    char *name2 = (char *) malloc(sizeof(char) * (20));
//    char *name3 = (char *) malloc(sizeof(char) * (20));
//    char *name4 = (char *) malloc(sizeof(char) * (20));
//    char *name5 = (char *) malloc(sizeof(char) * (20));
//
//    strcpy(name0, "MacBook Pro");
//    strcpy(name1, "iPod");
//    strcpy(name2, "iPhone");
//    strcpy(name3, "iPad");
//    strcpy(name4, "Apple Watch");
//    strcpy(name5, "Apple TV");
//
//    ProductExample **products = (ProductExample **) malloc(sizeof(ProductExample *) * 6);
//
//    products[0] = (ProductExample *) malloc(sizeof(ProductExample));
//    products[1] = (ProductExample *) malloc(sizeof(ProductExample));
//    products[2] = (ProductExample *) malloc(sizeof(ProductExample));
//    products[3] = (ProductExample *) malloc(sizeof(ProductExample));
//    products[4] = (ProductExample *) malloc(sizeof(ProductExample));
//    products[5] = (ProductExample *) malloc(sizeof(ProductExample));
//
//    products[0]->name = name0;
//    products[0]->price = 1499;
//    products[1]->name = name1;
//    products[1]->price = 199;
//    products[2]->name = name2;
//    products[2]->price = 599;
//    products[3]->name = name3;
//    products[3]->price = 499;
//    products[4]->name = name4;
//    products[4]->price = 299;
//    products[5]->name = name5;
//    products[5]->price = 199;
//
//    return products;
//
//}
//
//void freeResources(RBTree **tree, ProductExample ***products)
//{
//    freeRBTree(tree);
//    productFree((*products)[1]);
//    productFree((*products)[5]);
//    free(*products);
//}
//
//void assertion(int passed, int assertion_num, char *msg)
//{
//    if (!passed)
//    {
//        printf("assertion %d failed: %s\n", assertion_num, msg);
//    }
//
//}
//
//int main()
//{
//    ProductExample **products = getProducts();
//    RBTree *tree = newRBTree(productComparatorByName, productFree);
//    insertToRBTree(tree, products[2]);
//    insertToRBTree(tree, products[3]);
//    insertToRBTree(tree, products[4]);
//    insertToRBTree(tree, products[0]);
//    int i = 0;
//    for (i = 0; i < 6; i++)
//    {
//        if (RBTreeContains(tree, products[i]))
//        {
//            printf("\"%s\" is in the tree.\n", products[i]->name);
//            if (i == 1 || i == 5)
//            {
//                printf(" This product should not be in the tree!\nTest failed, aborting");
//                freeResources(&tree, &products);
//                return 1;
//            }
//        }
//        else
//        {
//            printf("\"%s\" is not in the tree.\n", products[i]->name);
//            if (i != 1 && i != 5)
//            {
//                printf(" This product should be in the tree!\nTest failed, aborting");
//                freeResources(&tree, &products);
//                return 2;
//            }
//        }
//    }
//
//    printf("\nThe number of products in the tree is %lu.\n\n", tree->size);
//    forEachRBTree(tree, printProduct, NULL);
//    freeResources(&tree, &products);
//    printf("test passed\n");
//    return 0;
//}
//
//
//#endif //TA_EX3_PRODUCTEXAMPLE_C

