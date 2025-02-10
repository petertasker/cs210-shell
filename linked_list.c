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
  if (head == NULL) {
    printf("List is empty.\n");
    return NULL;
  }

  Node *current = head;

  if (pos == 0) {
    head = current->next;
    if (head) {
      head->previous = NULL;
    }
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

  for (int i = 0; i < pos && current != NULL; i++) {
    current = current->next;
  }

  if (current == NULL)
    return head;

  if (current->previous)
    current->previous->next = current->next;

  if (current->next)
    current->next->previous = current->previous;

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
   Free every node of a linked list
*/
void clearList(Node* head) {
  Node *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    
    free(temp->command);

    if (temp->arguments) {
      for (int i = 0; temp->arguments[i] != NULL; i++) {
        free(temp->arguments[i]);
      }
      free(temp->arguments);
    }

    free(temp);
  }
}
