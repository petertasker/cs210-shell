#ifndef LINKED_LIST_H
#define LINKED_LIST_H


typedef struct DNode {
  char **arguments; 
  struct DNode *previous;
  struct DNode *next;
} DNode;


/**
   Inserting into doubly-linked lists
*/
DNode *doubleCreateNode(char **arguments);
DNode *doubleInsertNodeAtBeginning(DNode* head, char **arguments);
DNode *doubleInsertNodeAtEnd(DNode *head, char **arguments);
DNode *doubleDeleteNodeAtPosition(DNode *head, int pos);


/**
   List adjacent functions
*/
void doublePrintList(DNode *head);
DNode *doubleClearList(DNode *head);


/**
   I/O
 */
void doubleWriteListToFile(DNode* head, char *path);
DNode *doubleReadListFromFile(DNode* head, char *path);


#endif /* LINKED_LIST_H */
