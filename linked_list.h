#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
  char* value;        
  struct Node *previous;
  struct Node *next;
} Node;

Node *createNode(char *value);
Node* insertNodeAtBeginning(Node* head, char *value);
Node* deleteNodeAtPosition(Node *head, int pos);
void printList(Node *head);
void freeList(Node *head);
#endif /* LINKED_LIST_H */
