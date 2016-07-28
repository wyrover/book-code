// Exercise 11.3 Using a linked list of structures representing a person's name.
/*
   You could link the PhoneRecord structures in a list by adding a pointer member.
   I chose to define a Node structure that is a node in a linked list. Each Node
   structure contains a pointer to a PhoneRecord structure and a pointer to
   the next Node structure. Memory for Node and PhoneRecord structures are allocated
   dynamically. You could extend this to allocate memory for names and numbers too.
*/
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define FIRST_NAME_LEN  31
#define SECOND_NAME_LEN 51
#define NUMBER_LEN      16


// Structure defining a name
typedef struct Name {
    char firstname[FIRST_NAME_LEN];
    char secondname[SECOND_NAME_LEN];
} Name;

// Structure defining a phone record
typedef struct PhoneRecord {
    Name name;
    char number[NUMBER_LEN];
} PhoneRecord;

// Structure defining a node in a linked list of PhoneRecord structures
typedef struct Node Node;
struct Node {
    PhoneRecord *pRecord;                                                             // Pointer to a PhoneRecord structure
    Node *pNext;                                                                      // Pointer to the next node in the list
};

Name read_name();                                                                   // Read a name from the keyboard
void show(PhoneRecord *pRecord);                                                    // Output a phone record
bool has_name(const PhoneRecord *pRecord, const Name *pName);                       // Test for a name
Node* create_node();                                                                // Create a new list node
PhoneRecord* create_record();                                                       // Create a new phone record
void insert_node(Node *pNode);                                                      // Insert a node in the list
int compare_records(const PhoneRecord *pFirst, const PhoneRecord *pSecond);         // Compare records
int compare_names(const Name *pFirst, const Name *pSecond);                         // Compare two names
void list_numbers(const Name *pName);                                               // List all numbers for a Name structure

Node *pStart = NULL;                                                                // Root node for the linked list

int main(void)
{
    char answer = 'n';
    Node *pNode = NULL;                                                               // Pointer to a list node

    // Read an arbitrary number of phone records from the keyboard
    do {
        insert_node(create_node());                                                     // Create and insert new node
        printf_s("Do you want to enter another(y or n)?: ");
        scanf_s(" %c", &answer, sizeof(answer));
    } while (tolower(answer) == 'y');

    // Search the list of phone records for a number
    Name name;
    bool first = true;

    do {
        if (first) {
            first = false;
            printf_s("\nDo you want to search for a name (y or n)?");
            scanf_s(" %c" , &answer, sizeof(answer));

            if (tolower(answer) == 'n') break;
        }

        name = read_name();
        list_numbers(&name);
        printf_s("Do you want to search for another (y or n)? ");
        scanf_s(" %c" , &answer, sizeof(answer));
    } while (tolower(answer) == 'y');

    // List all the records in the linked list
    printf_s("\nThe records we have are:\n");
    pNode = pStart;

    do {
        show(pNode->pRecord);
    } while ((pNode = pNode->pNext) != NULL);

    printf_s("\n");
    // Don't forget to free the memory!
    pNode = pStart;

    do {
        free(pNode->pRecord);                                                           // Free memory for the record from the current node
        pStart = pNode;                                                                 // Save current node address
        pNode = pNode->pNext;                                                           // Get next node address
        free(pStart);                                                                   // Free memory for the current node
    } while ((pNode = pNode->pNext) != NULL);

    return 0;
}

// Read a name from the keyboard and store in a structure
Name read_name(void)
{
    Name name;
    printf_s("\nEnter a first name: ");
    scanf_s(" %s", name.firstname, sizeof(name.firstname));
    printf_s("Enter a second name: ");
    scanf_s(" %s", name.secondname, sizeof(name.secondname));
    return name;
}

// Output a record */
void show(PhoneRecord *pRecord)
{
    printf_s("\n%s %s   %s", pRecord->name.firstname, pRecord->name.secondname, pRecord->number);
}

bool has_name(const PhoneRecord *pRecord, const Name *pName)
{
    return (strcmp(pName->firstname, pRecord->name.firstname) == 0 &&
            strcmp(pName->secondname, pRecord->name.secondname) == 0);
}

// Create a new list node
Node* create_node(void)
{
    Node *pNode = NULL;                                                               // Pointer to the new node
    pNode = (Node*)malloc(sizeof(Node));                                              // Allocate memory for node
    pNode->pNext = NULL;                                                              // No next node yet
    pNode->pRecord = create_record();                                                 // Create record and store address in node
    return pNode;
}

// Create a new phone record
PhoneRecord* create_record(void)
{
    PhoneRecord *pRecord = NULL;                                                      // Pointer to the new record
    pRecord = (PhoneRecord*)malloc(sizeof(PhoneRecord));                              // Allocate memory
    pRecord->name = read_name();                                                      // Read the name
    // Get the number for the name
    printf_s("Enter the number for this name: ");
    scanf_s(" %[ 0123456789]", pRecord->number, sizeof(pRecord->number));             // Read the number - including spaces
    return pRecord;                                                                   // Return the address of the record
}

/*
  Compare two PhoneRecord structures
  Returns -1 if the name for the first is < name for the second
  Returns  0 if the name for the first is equal to the name for the second
  Returns +1 if the name for the first is > name for the second
*/
int compare_records(const PhoneRecord *pFirst, const PhoneRecord *pSecond)
{
    return compare_names(&(pFirst->name), &(pSecond->name));
}

/* Compare two names
  Returns -1 if the  first is < the second
  Returns  0 if the first is equal to tthe second
  Returns +1 if the first is >  the second

  The comparison is by second name. If second names are equal,
  first names are compared.
*/
int compare_names(const Name *pfirst, const Name *psecond)
{
    int result = 0;
    result = strcmp(pfirst->secondname, psecond->secondname);
    return (result != 0 ? result : strcmp(pfirst->firstname, psecond->firstname));
}

// Insert a node into the list
void insert_node(Node *pNode)
{
    Node *pCurrent = NULL;
    Node *pPrevious = NULL;

    // Check for empty list
    if (!pStart) {
        pStart = pNode;                                                                 // Store address of the node as the start node
        return;
    }

    // Find position to insert the new node
    pCurrent = pStart;

    while (pCurrent) {
        if (compare_records(pNode->pRecord, pCurrent->pRecord) <= 0) {
            // New node goes before current node
            pNode->pNext = pCurrent;                                                      // Set new node next pointer to current

            if (!pPrevious) {                                                             // check for no previous node
                // pCurrent should be the first node
                pNode->pNext = pStart;                                                      // New node next pointer points to current
                pStart = pNode;                                                             // New node is the first node
            } else {
                // Otherwise hook pCurrent up to previous node...
                pPrevious->pNext = pNode;                                                   // Previous node next pointer points to new node
            }

            return;
        }

        // pCurrent fits later in the list so move on...
        pPrevious = pCurrent;                                                           // Previous node will be the current node
        pCurrent = pCurrent->pNext;                                                     // Current node is now the next node
    }

    // If we reach here, add pNode to the end
    pPrevious->pNext = pNode;
}

// List the numbers for a name
void list_numbers(const Name *pName)
{
    Node *pNode = NULL;
    bool found = false;
    int result = 0;
    // Go through the list comparing names
    pNode = pStart;

    while (pNode) {
        if (0 == (result = compare_names(pName, &(pNode->pRecord->name)))) {
            if (!found) {                                                                 // If this is the first time
                found = true;                                                               // Reset found flag
                printf_s("The numbers for this name are:\n");                               // and output the heading
            }

            printf_s("%s\n", pNode->pRecord->number);                                     // Output the number for the name
        } else if (result < 0)                                                           // If name comes before current
            break;                                                                        // we are done

        pNode = pNode->pNext;                                                           // Otherwise move to next node
    }

    if (!found)                                                                       // If the name was not found
        printf_s("No numbers found for this name.\n");                                  // Say so
}



