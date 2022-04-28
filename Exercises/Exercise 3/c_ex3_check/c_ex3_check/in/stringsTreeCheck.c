//
// Created by amit.david on 02/12/2019.
//

#include "RBTree.h"
#include "Structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int printString(const void *text, void *null)
{
    if (null != NULL)
    {
        return 0;
    }

    printf("%s", (char*)text);
    return 1;
}

void testFreeString() {}

int main()
{
    char *arr[] = {"a", "a", "e", "i", "f", "c", "h", "d", "e", "b", "i", "g", "f", "a"};
    int size = 14;
    RBTree *tree = newRBTree(&stringCompare, &testFreeString);

    for (int i = 0; i < size; i++)
    {
        printf("Try to add the letter %s\n", arr[i]);
        printf("\tinsertToRBTree return code: %d\n", insertToRBTree(tree, arr[i]));
        printf("\n\tforEachRBTree return code: %d\n", forEachRBTree(tree, &printString, NULL));
        printf("\n");
    }
    printf("Try to add NULL into the tree\n");

    char *text;
    for (int i = 0; i < size; i++)
    {
        text = RBTreeContains(tree, arr[i]) ? "%s is in the tree\n" : "%s not in the tree\n";
        printf(text, arr[i]);
    }

    text = RBTreeContains(tree, "s") ? "%s is in the tree\n" : "%s not in the tree\n";
    printf(text, "s");

    text = RBTreeContains(tree, "aa") ? "%s is in the tree\n" : "%s not in the tree\n";
    printf(text, "aa");

    text = RBTreeContains(tree, "A") ? "%s is in the tree\n" : "%s not in the tree\n";
    printf(text, "A");

    text = RBTreeContains(tree, "") ? "%s is in the tree\n" : "%s not in the tree\n";
    printf(text, "Empty string");

    printf("\nThe number of strings in the tree is %d.\n\n", tree->size);

    char testString[500] = "All the letters in the tree are: ";

    // forEachRBTree(tree, &concatenate, NULL); // School solution doesnt support it :)
    forEachRBTree(tree, &concatenate, testString);
    printf("%s", testString);

    freeRBTree(&tree);
    printf("\nTest done\n");
}


