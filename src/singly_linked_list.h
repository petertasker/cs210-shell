#ifndef SINGLE_LINKED_LIST_H
#define SINGLE_LINKED_LIST_H


typedef struct SNode {
  char *alias_name;
  char **arguments; 
  struct SNode *next;
} SNode;


/**
   Inserting into lists
*/
SNode *singleCreateNode(char **arguments);
SNode *singleInsertNodeAtBeginning(SNode* head, char **arguments);
SNode *singleInsertNodeAtEnd(SNode *head, char **arguments);
SNode *singleDeleteNodeAtPosition(SNode *head, int pos);


/**
   List adjacent functions
*/
SNode *singleClearList(SNode *head);
void singlePrintList(SNode *head);

/**
   I/O
 */
void singleWriteListToFile(SNode* head, char *path);
SNode *singleReadListFromFile(SNode* head, char *path);


#endif /* SINGLE_LINKED_LIST_H */
