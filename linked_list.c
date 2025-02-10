/**
   Functions which are used in single list implementations
   used in aliasing and history tbd

   https://www.geeksforgeeks.org/doubly-linked-list/
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "constants.h"
#include "shell_library.h"


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

  int index = 1;
  
  // Find the last node (tail)
  Node *current = head;
  while (current->next != NULL) {
    current = current->next;
    index++;
  }

  // Traverse backward and print each node
  while (current != NULL) {
    // Print command
    printf("%2d. %s", index, current->command);
    
    // Print each argument
    if (current->arguments) {
      for (int i = 0; current->arguments[i] != NULL; i++) {
        printf(" %s", current->arguments[i]);
      }
    }
    printf("\n");
    
    // Move to the previous node
    current = current->previous;
    index--;
  }
}


/**
   Free every node of a linked list
   make sure to set HEAD to null if you
   do not plan on using the list again
*/
Node *clearList(Node* head) {
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
  return head;
}


/**
   Write a doubly linked list's contents into a file
*/
void writeListToFile(Node* head, char *path) {
  if (head == NULL || path == NULL) {
    return;
  }

  FILE *file = fopen(path, "w");
  if (file == NULL) {
    fprintf(stderr, "Failed to open file %s\n", path);
    return;
  }
  
  Node *current = head;
  while (current != NULL) {
    // Write command (with NULL check)
    if (current->command != NULL) {
      fprintf(file, "%s", current->command);
    }

    // Write each argument (with NULL checks)
    if (current->arguments != NULL) {
      for (int i = 0; current->arguments[i] != NULL; i++) {
	fprintf(file, " %s", current->arguments[i]);
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
Node* readListFromFile(Node* head, char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "Failed to open file %s\n", path);
    return NULL;
  }

  char line[MAX_INPUT_LEN + 1];  // Buffer to store each line from the file
  while (fgets(line, sizeof(line), file)) {
    trimString(line);  // Clean up any extra spaces or newlines
    if (compareStrings(line, "")) {
      continue;  // Skip empty lines
    }

    char **args = tokeniseString(line);  // Tokenize the line into arguments
    if (args != NULL) {
      head = addToHistory(head, args);  // Add the tokens to the history list
      freeArguments(args);  // Free the argument array after use
    }
  }

  fclose(file);  // Close the file after reading
  return head;
}


