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
Node* createNode(char **arguments) {
  Node* new_node = (Node*)malloc(sizeof(Node));
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
Node* insertNodeAtBeginning(Node* head, char **arguments) {
  if (arguments == NULL) {
    return head;
  }

  // Create a new node
  Node* new_node = createNode(arguments);
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
   Delete a node from the linked list at a
   specified position

   Not used in history but can be used in alias
*/
Node* deleteNodeAtPosition(Node *head, int pos) {
  if (head == NULL || pos < 0) {
    printf("List is empty.\n");
    return NULL;
  }
  
  Node *current = head;

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
void printList(Node *head) {
  if (head == NULL) {
    printf("List is empty\n");
    return;
  }

  int index = 1;
  
  // Find the last node (tail)
  Node *tail = head;
  while (tail->next != NULL) {
    tail = tail->next;
    index++;
  }

  // Traverse backward and print each node
  Node *current = tail;
  while (current != NULL) {
    // Print command
    printf("%2d. ", index);
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
Node* readListFromFile(Node* head, char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "Failed to open file %s\n", path);
    return NULL;
  }

  char line[MAX_INPUT_LEN + 1];
  while (fgets(line, sizeof(line), file)) {
    trimString(line);  
    if (compareStrings(line, "")) {
      continue;
    }

    char **args = tokeniseString(line); 
    if (args != NULL) {
      head = addToHistory(head, args);  
      freeArguments(args);  
    }
  }

  fclose(file);  // Close the file after reading
  return head;
}


