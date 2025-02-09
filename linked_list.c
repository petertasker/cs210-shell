/**
   Functions which are used in single list implementations
 */

#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    int data;
    struct Node *next;
} Node;


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



