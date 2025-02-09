/**
   Functions which are used in single list implementations
   used in aliasing and history tbd
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"


/**
   Create a new node
*/
Node* createNode(char *value) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    printf("Memory allocation failed!\n");
    return NULL;
  }

  newNode->value = strdup(value);  // Allocate memory for the string and copy it
  if (!newNode->value) {
    printf("String duplication failed!\n");
    free(newNode);  // Free memory allocated for the node
    return NULL;
  }

  newNode->next = NULL;
  return newNode;
}


/**
   Insert node at the end of a list
*/
void insertNode(Node **head, char *value) {
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
void deleteNode(Node **head, char *value) {
  if (*head == NULL) return;

  Node *temp = *head, *prev = NULL;

  // If head node itself holds the value
  if (temp != NULL && strcmp(temp->value, value) == 0) {
    *head = temp->next;
    free(temp->value);  // Free the value memory
    free(temp);         // Free the node memory
    return;
  }

  // Search for the node to delete
  while (temp != NULL && strcmp(temp->value, value) != 0) {
    prev = temp;
    temp = temp->next;
  }

  // If value not found
  if (temp == NULL) return;

  // Unlink the node and free memory
  prev->next = temp->next;
  free(temp->value);  // Free the value memory
  free(temp);         // Free the node memory
}


/**
   Print list
*/
void printList(Node *head) {
  Node *temp = head;
  while (temp != NULL) {
    printf("%s -> ", temp->value);
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
    free(temp->value);  // Free the value memory
    free(temp);         // Free the node memory
  }
}
