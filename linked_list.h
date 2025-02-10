#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
  char *command; // first token
  char **arguments; // other tokens
  struct Node *previous;
  struct Node *next;
} Node;

/* Inserting into lists */
Node *createNode(char *command, char **arguments);
Node* insertNodeAtBeginning(Node* head, char *command, char **arguments);
Node* deleteNodeAtPosition(Node *head, int pos);

/* List adjacent functions */
void printList(Node *head);
void clearList(Node *head);

/* I/O */
void writeListToFile(Node* head, char *path);
void readListFromFile(Node* head, char *path);


#endif /* LINKED_LIST_H */
