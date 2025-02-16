#ifndef LINKED_LIST_H
#define LINKED_LIST_H


typedef struct DNode {
  char **arguments; 
  struct DNode *previous;
  struct DNode *next;
} DNode;


/**
   List manipulation
*/
DNode *doubleListCreateNode(char **arguments);
DNode *doubleListInsertNodeAtBeginning(DNode* head, char **arguments);
DNode *doubleListInsertNodeAtEnd(DNode *head, char **arguments);
DNode *doubleListDeleteNodeAtPosition(DNode *head, int pos);


/**
   List adjacent functions
*/
void doubleListPrint(DNode *head, char **arguments);
DNode *doubleListFree(DNode *head);


/**
   I/O
 */
void doubleListWriteToFile(DNode* head, char *path);
DNode *doubleListReadFromFile(DNode* head, char *path);


#endif /* LINKED_LIST_H */
