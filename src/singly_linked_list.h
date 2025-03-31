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
SNode *singleListCreateNode(char **arguments);
SNode *singleListInsertNodeAtBeginning(SNode *head, char **arguments);
SNode *singleListInsertNodeAtEnd(SNode *head, char **arguments);
SNode *singleListDeleteNodeAtPosition(SNode *head, int pos);
SNode *singleListDeleteNodeByKey(SNode *head, char *key);

/**
   List adjacent functions
*/
SNode *singleListFree(SNode *head);
void singleListPrint(SNode *head);

/**
   I/O
 */
void singleListWriteToFile(SNode *head, char *path);
SNode *singleListReadFromFile(SNode *head, char *path);

#endif /* SINGLE_LINKED_LIST_H */
