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
Node* createNode(char *command, char **arguments) {
  Node* new_node = 
    (Node*)malloc(sizeof(Node));
  new_node->command = command;
  new_node->arguments = arguments;
  new_node->previous = NULL;
  new_node->next = NULL;
  return new_node;
}


/**
   Insert a node at the beginning of a list
*/
Node* insertNodeAtBeginning(Node* head, char *command, char **arguments) {
    
  // Create a new node
  Node* new_node = createNode(command, arguments);
    
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
  if (head == NULL) {
    return NULL;
  }
  printf("Empty list: head freed\n");
  Node *current = head;
  // If pos is 0, delete head
  if (pos == 0) {
    head = current->next;
    if (head) {
      head->previous = NULL;
    }
    printf("Beginning freeing of string\n");
    // Free the strings
    free(current->command);
    printf("Command freed\n");
    if (current->arguments) {
      printf("Crashing here\n");
      for (int i = 0; current->arguments[i] != NULL; i++) {
	printf("Freeing %i\n", i);
	if (!current->arguments[i]) {
	  free(current->arguments[i]);
	}
	
      }
      free(current->arguments);
    }
    free(current);
    return head;
  }
  printf("traversal beginning\n");
  // Traverse to the node at the given position
  for (int i = 0; i < pos && current != NULL; i++) {
    current = current->next;
  }

  // If the position is out of range
  if (current == NULL)
    return head;

  // Update the previous node's next pointer
  if (current->previous)
    current->previous->next = current->next;

  // Update the next node's prev pointer
  if (current->next)
    current->next->previous = current->previous;

  // Free the strings
  free(current->command);
  if (current->arguments) {
    for (int i = 0; current->arguments[i] != NULL; i++) {
      free(current->arguments[i]);
    }
    free(current->arguments);
  }
  free(current);

  return head;
}


/**
   Print the value of each node of a linked list
*/
void printList(Node *head) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    Node *current = head;
    while (current != NULL) {
        // Print command
        printf("%s", current->command ? current->command : "(null)");
        
        // Print each argument
        if (current->arguments) {
            for (int i = 0; current->arguments[i] != NULL; i++) {
                printf(" %s", current->arguments[i]);
            }
        }
        printf("\n");
        current = current->next;
    }
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
