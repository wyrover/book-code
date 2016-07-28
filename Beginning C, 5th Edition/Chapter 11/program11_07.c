// Program 11.7 Sorting integers using a binary tree
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node Node;

// Defines a node in a binary tree sotring integers
struct Node {
    long item;                                     // The data item
    int count;                                     // Number of copies of item
    Node *pLeft;                                   // Pointer to left node
    Node *pRight;                                  // Pointer to right node
};

// Function prototypes
Node *create_node(long value);                   // Create a tree node
Node *add_node(long value, Node *pNode);         // Insert a new node
void list_nodes(Node *pNode);                    // List all nodes
void free_nodes(Node *pNode);                    // Release memory


// Function main - execution starts here
int main(void)
{
    long newvalue = 0;
    Node *pRoot = NULL;
    char answer = 'n';

    do {
        printf_s("Enter the node value: ");
        scanf_s(" %ld", &newvalue, sizeof(newvalue));

        if (!pRoot)
            pRoot = create_node(newvalue);
        else
            add_node(newvalue, pRoot);

        printf_s("Do you want to enter another (y or n)? ");
        scanf_s(" %c", &answer, sizeof(answer));
    } while (tolower(answer) == 'y');

    printf_s("The values in ascending sequence are:\n");
    list_nodes(pRoot);                             // Output the contents of the tree
    free_nodes(pRoot);                             // Release the heap memory
    return 0;
}

// Create a binary tree node
Node *create_node(long value)
{
    Node *pNode = (Node*)malloc(sizeof(Node));
    pNode->item = value;                           // Set the value
    pNode->count = 1;                              // Set the count
    pNode->pLeft = pNode->pRight = NULL;           // No left or right nodes
    return pNode;
}

// Add a new node to the tree
Node *add_node(long value, Node *pNode)
{
    if (!pNode)                                    // If there's no node
        return create_node(value);                   // ...create one and return it

    if (value == pNode->item) {
        // Value equals current node...
        ++pNode->count;                              // ...so increment count ...
        return pNode;                                // ...and return the same node
    }

    if (value < pNode->item) {
        // Less than current node...
        if (!pNode->pLeft) {                         // ...and no left node
            pNode->pLeft = create_node(value);         // ... so create a left node...
            return pNode->pLeft;                       // ...and return it.
        } else                                       // There is a left node...
            return add_node(value, pNode->pLeft);      // ...so add value via the left node
    } else {
        // Greater than current node...
        if (!pNode->pRight) {
            // ...but no right node...
            pNode-> pRight = create_node(value);       // ...so create one...
            return pNode-> pRight;                     // ...and return it.
        } else                                       // There is a right node...
            return add_node(value, pNode->pRight);     // ...so add to that.
    }
}

// List the node values in ascending sequence
void list_nodes(Node *pNode)
{
    if (pNode->pLeft)
        list_nodes(pNode->pLeft);

    printf_s("%10d x %10ld\n", pNode->count, pNode->item);

    if (pNode->pRight)
        list_nodes(pNode->pRight);
}

// Release memory allocated to nodes
void free_nodes(Node *pNode)
{
    if (!pNode)                                    // If there's no node...
        return;                                      // ...we are done.

    if (!pNode->pLeft)                             // If there's a left sub-tree...
        free_nodes(pNode->pLeft);                    // ...free memory for those nodes.

    if (pNode->pRight)                             // If there's a right sub-tree...
        free_nodes(pNode->pRight);                   // ...free memory for those nodes.

    free(pNode);                                   // Free current node memory
}

