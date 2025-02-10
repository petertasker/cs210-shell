#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
  char **arguments; 
  struct Node *previous;
  struct Node *next;
} Node;

/* Inserting into lists */
Node *createNode(char **arguments);
Node *insertNodeAtBeginning(Node* head, char **arguments);
Node *deleteNodeAtPosition(Node *head, int pos);

/* List adjacent functions */
void printList(Node *head);
Node *clearList(Node *head);

/* I/O */
void writeListToFile(Node* head, char *path);
Node *readListFromFile(Node* head, char *path);


#endif /* LINKED_LIST_H */
