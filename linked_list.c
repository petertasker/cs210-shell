/**
   Functions which are used in single list implementations
   used in aliasing and history tbd
*/

#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    int data;
    struct Node *next;
} Node;


/**
   Create a new node
*/
Node* createNode(String value) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    printf("Memory allocation failed!\n");
    return NULL;
  }
  newNode->data = value;
  newNode->next = NULL;
  return newNode;
}


/**
   Insert node at the end of a list
*/
void insertNode(Node **head, String value) {
  Node *newNode = createNode(value);
  if (!newNode) return;

  if (*head == NULL) {
    *head = newNode;
    return;
  }

  Node *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = newNode;
}


/**
   Delete a node by value
*/
void deleteNode(Node **head, String value) {
  if (*head == NULL) return;

  Node *temp = *head, *prev = NULL;

  // If head node itself holds the value
  if (temp != NULL && temp->data == value) {
    *head = temp->next;
    free(temp);
    return;
  }

  // Search for the node to delete
  while (temp != NULL && temp->data != value) {
    prev = temp;
    temp = temp->next;
  }

  // If value not found
  if (temp == NULL) return;

  // Unlink the node and free memory
  prev->next = temp->next;
  free(temp);
}


/**
   Print list
*/
void printList(Node *head) {
  Node *temp = head;
  while (temp != NULL) {
    printf("%d -> ", temp->data);
    temp = temp->next;
  }
  printf("NULL\n");
}


/**
   De-allocate memory for the list
*/
void freeList(Node **head) {
  Node *temp;
  while (*head) {
    temp = *head;
    *head = (*head)->next;
    free(temp);
  }
}
