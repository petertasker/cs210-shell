#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
  char* value;        
  struct Node *next;
} Node;

Node *createNode(char *value);
void insertNode(Node **head, char* value);  
void deleteNode(Node **head, char* value);
void printList(Node *head);
void freeList(Node **head);
#endif /* LINKED_LIST_H */
