#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
  char *command; // first token
  char **arguments; // other tokens
  struct Node *previous;
  struct Node *next;
} Node;

Node *createNode(char *command, char **arguments);
Node* insertNodeAtBeginning(Node* head, char *command, char **arguments);
Node* deleteNodeAtPosition(Node *head, int pos);
void printList(Node *head);
void freeList(Node *head);
#endif /* LINKED_LIST_H */
