#ifndef SINGLE_LINKED_LIST_H
#define SINGLE_LINKED_LIST_H


typedef struct SNode {
  char **arguments; 
  struct SNode *next;
} SNode;


/**
   Inserting into lists
*/
SNode *singleCreateNode(char **arguments);
SNode *singleCinsertNodeAtBeginning(SNode* head, char **arguments);
SNode *singleinsertNodeAtEnd(SNode *head, char **arguments);
SNode *singleNodeAtPositionSingle(SNode *head, int pos);


/**
   List adjacent functions
*/
SNode *singleClearList(SNode *head);


/**
   I/O
 */
void singleWriteListToFile(SNode* head, char *path);
SNode *singleReadListFromFile(SNode* head, char *path);


#endif /* SINGLE_LINKED_LIST_H */
