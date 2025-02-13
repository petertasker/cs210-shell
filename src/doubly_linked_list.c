/**
   Implementation of a doubly-linked list

   Many of these are from GeeksforGeeks tutorials
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doubly_linked_list.h"
#include "constants.h"
#include "shell_library.h"
#include "initialise.h"

/**
   Create a new node
*/
DNode* doubleListCreateNode(char **arguments) {
  DNode* new_node = (DNode*)malloc(sizeof(DNode));
  if (new_node == NULL) {
    fprintf(stderr, "Failed to allocate space for new node\n");
    return NULL;
  }

  new_node->arguments = arguments;
  new_node->previous = NULL;
  new_node->next = NULL;
  return new_node;
}


/**
   Insert a node at the beginning of a list
*/
DNode* doubleListInsertNodeAtBeginning(DNode* head, char **arguments) {
  if (arguments == NULL) {
    return head;
  }

  // Create a new node
  DNode* new_node = doubleListCreateNode(arguments);
  if (new_node == NULL) {
    return head;
  }

  // Make next of new node as head
  new_node->next = head;
  if (head != NULL) {
    head->previous = new_node;
  }
  return new_node;
}


/**
   Insert a node at the end of a linked list
*/
DNode* doubleListInsertNodeAtEnd(DNode* head, char **arguments) {
  if (arguments == NULL) {
    return head;
  }
  
  // Create a new node
  DNode *new_node = doubleListCreateNode(arguments);
  if (new_node == NULL) {
    return head;
  }
  
  if (head == NULL) {
    return new_node;
  }
  
  DNode *current = head;
   
  // Find end of linked list
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = new_node;
  new_node->previous = current;

  return head;
}


/**
   Delete a node from a specific position

   https://www.geeksforgeeks.org/delete-a-node-in-a-doubly-linked-list/
*/
DNode* doubleListDeleteNodeAtPosition(DNode *head, int pos) {
  if (head == NULL || pos < 0) {
    printf("List is empty.\n");
    return NULL;
  }
  
  DNode *current = head;

  if (pos == 0) {
    head = current->next;
    if (head) {
      head->previous = NULL;
    }
    
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

  if (current == NULL) {
    return head;
  }

  if (current->previous) {
    current->previous->next = current->next;
  }
  
  if (current->next) {
    current->next->previous = current->previous;
  }
  
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
void doubleListPrint(DNode *head) {
  if (head == NULL) {
    printf("List is empty\n");
    return;
  }

  int index = 1;
  DNode *current = head;

  // Traverse forward and print each node
  while (current != NULL) {
    // Print index
    printf("%2d. ", index);

    // Print each argument
    if (current->arguments) {
      for (int i = 0; current->arguments[i] != NULL; i++) {
        printf(" %s", current->arguments[i]);
      }
    }
    printf("\n");

    // Move to the next node
    current = current->next;
    index++;
  }
}


/**
   Clear and free a doubly-linked list
*/
DNode *doubleListFree(DNode* head) {
  DNode *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    
    if (temp->arguments) {
      for (int i = 0; temp->arguments[i] != NULL; i++) {
        free(temp->arguments[i]);
      }
      free(temp->arguments);
    }

    free(temp);
  }
  return NULL;
}


/**
   Write a doubly linked list's contents into a file
*/
void doubleListWriteToFile(DNode* head, char *path) {
  if (head == NULL || path == NULL) {
    return;
  }
  
  FILE *file = fopen(path, "w");
  if (file == NULL) {
    fprintf(stderr, "Failed to open file %s\n", path);
    return;
  }

  DNode *current = head;
  while (current != NULL) {
    // Write each argument
    if (current->arguments != NULL) {
      for (int i = 0; current->arguments[i] != NULL; i++) {
        fprintf(file, "%s ", current->arguments[i]);
      }
    }
    fprintf(file, "\n");
    current = current->next;
  }
  fclose(file);
}



/**
   Read a file's contents into a doubly linked list
 */
DNode* doubleListReadFromFile(DNode* head, char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "Failed to open file %s\n", path);
    return NULL;
  }

  // We are reading the 
  char line[MAX_INPUT_LEN + 1];
  while (fgets(line, sizeof(line), file)) {
    trimString(line);  
    if (compareStrings(line, "")) {
      continue;
    }

    char **args = tokeniseString(line); 
    if (args != NULL) {
      loadIntoHistory(&head, args);  
      freeArguments(args);  
    }
  }
  
  fclose(file);  // Close the file after reading
  return head;
}


