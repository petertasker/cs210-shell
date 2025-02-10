/**
   Functions which are used in single list implementations
   used in aliasing and history tbd

   https://www.geeksforgeeks.org/doubly-linked-list/
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"


/**
   Create a new node
*/
Node* createNode(char *value) {
  Node* new_node = 
    (Node*)malloc(sizeof(Node));
  new_node->value = value;
  new_node->previous = NULL;
  new_node->next = NULL;
  return new_node;
}


/**
   Insert a node at the beginning of a list
*/
Node* insertNodeAtBeginning(Node* head, char *value) {
    
  // Create a new node
  Node* new_node = createNode(value);
    
  // Make next of it as head
  new_node->next = head;
    
  // Set previous of head as new node
  if (head != NULL) {
    head->previous = new_node;
  }
    
  // Return new node as new head
  return new_node;
}


/**
   Delete a node from the linked list at a
   specified position
*/
Node* deleteNodeAtPosition(Node *head, int pos) {

  // If the list is empty
  if (head == NULL)
    return head;

  struct Node *current = head;

  // Traverse to the node at the given position
  for (int i = 1; current && i < pos; ++i) {
    current = current -> next;
  }

  // If the position is out of range
  if (current == NULL)
    return head;

  // Update the previous node's next pointer
  if (current -> previous)
    current -> previous -> next = current -> next;

  // Update the next node's prev pointer
  if (current -> next)
    current -> next -> previous = current -> previous;

  // If the node to be deleted is the head node
  if (head == current)
    head = current -> next;

  // Deallocate memory for the deleted node
  free(current);
  return head;
}


/**
   Print the value of each node of a linked list
*/
void printList(Node *head) {
  Node *current = head;
  while (current != NULL) {
    printf("%s ", current->value);
    current = current->next;
  }
  printf("\n");
}

/**
   Clear every node of a linked list
*/
void freeList(Node *head) {
  Node *current = head;
  while (current != NULL) {
    current = deleteNodeAtPosition(current, 0);
  }
}
