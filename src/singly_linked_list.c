/**
   Implementation of a singly-linked list

   Many of these are from GeeksforGeeks tutorials
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "singly_linked_list.h"
#include "constants.h"
#include "shell_library.h"
#include "initialise.h"
#include "built_in_commands.h"


/**
   Create a new Single list node
*/
SNode* singleListCreateNode(char **arguments) {
  SNode* new_node = (SNode*)malloc(sizeof(SNode));
  if (new_node == NULL) {
    fprintf(stderr, "Failed to allocate space for new SNode\n");
    return NULL;
  }

  int num_args = 0;
  new_node->alias_name = strdup(arguments[1]);
  
  if (!new_node->alias_name) {
    fprintf(stderr, "Failed to allocat space for alias_name\n");
    free(new_node);
    return NULL;
  }
  
  // Count arguments
  while (arguments[num_args + 2] != NULL) {
    num_args++;
  }

  new_node->arguments = malloc((num_args + 1) * sizeof(char*));
  if (!new_node->arguments) {
    fprintf(stderr, "Failed to allocate space for arguments array\n");
    free(new_node->alias_name);
    free(new_node);
    return NULL;
  }
  
  for (int i = 0; i < num_args; i++) {
    new_node->arguments[i] = strdup(arguments[i + 2]);
    if (!new_node->arguments[i]) {
      for (int j = 0; j < i; j++) {
	free(new_node->arguments[j]);
      }
      free(new_node->alias_name);
      free(new_node->arguments);
      free(new_node);
      return NULL;
    }
  }  
  new_node->arguments[num_args] = NULL;
  
  new_node->next = NULL;

  return new_node;
}


/**
   Insert a Node at the beginning of a singly-linked list
*/
SNode* singleListInsertNodeAtBeginning(SNode* head, char **arguments) {
  if (arguments == NULL) {
    return head;
  }
  
  // Create a new SNode
  SNode* new_node = singleListCreateNode(arguments);
  if (new_node == NULL) {
    return head;
  }
  
  // Make next of new SNode as head
  new_node->next = head;
  return new_node;
}


/**
   Insert a Node at the end of a singly-linked list
*/
SNode* singleListInsertNodeAtEnd(SNode* head, char **arguments) {
  if (arguments == NULL) {
    return head;
  }
    
  // Create a new SNode
  SNode *new_node = singleListCreateNode(arguments);
  if (new_node == NULL) {
    return head;
  }
    
  if (head == NULL) {
    return new_node;
  }
    
  SNode *current = head;
     
  // Find end of linked list
  while (current->next != NULL) {
    current = current->next;
  }
  
  current->next = new_node;
    
  
  return head;
}



/**
   Delete a Node from a singly-linked list at a
   specified key (unbind by alias name)

   https://www.geeksforgeeks.org/c-program-for-deleting-a-node-in-a-linked-list/
 */
SNode *singleListDeleteNodeByKey(SNode *head, char *key) {
  if (head == NULL || key == NULL) {
    return head;
  }
  
  SNode *current = head;
  SNode *previous = NULL;
  
  if (compareStrings(current->alias_name, key)) {
    SNode *new_head = current->next;
    free(current->alias_name);
    for (int i = 0; current->arguments[i] != NULL; i++) {
      free(current->arguments[i]);
    }
    free(current->arguments);
    free(current);
    return new_head;
  }

  while (current != NULL && !(compareStrings(current->alias_name, key))) {
    previous = current;
    current = current->next;
  }

  if (current == NULL) {
    fprintf(stderr, "Failed to unbind alias: alias not found.\n");
    return head;
  }

  previous->next = current->next;
  free(current->alias_name);
  for (int i = 0; current->arguments[i] != NULL; i++) {
    free(current->arguments[i]);
  }
  free(current->arguments);
  free(current);

  return head;
}


/**
   Delete a Node from a singly-linked list at a
   specified position

   especially useful - https://www.geeksforgeeks.org/delete-a-linked-list-node-at-a-given-position/
*/
SNode* singleListDeleteNodeAtPosition(SNode *head, int pos) {
    if (head == NULL || pos < 0) {
        printf("List is empty or invalid position.\n");
        return NULL;
    }

    SNode *current = head;
    SNode *prev = NULL;
    
    // Special case: deleting first node
    if (pos == 0) {
        head = current->next;
        if (current->arguments) {
            for (int i = 0; current->arguments[i] != NULL; i++) {
                free(current->arguments[i]);
            }
            free(current->alias_name);
            free(current->arguments);
        }
        free(current);
        return head;
    }

    // Move to the node to be deleted
    for (int i = 0; i < pos && current != NULL; i++) {
        prev = current;
        current = current->next;
    }

    // If position is beyond list length
    if (current == NULL) {
        return head;
    }

    // Link the previous node to the next node, bypassing current
    prev->next = current->next;

    // Free the node and its contents
    if (current->arguments) {
        for (int i = 0; current->arguments[i] != NULL; i++) {
            free(current->arguments[i]);
        }
        free(current->alias_name);
        free(current->arguments);
    }
    free(current);

    return head;
}


/**
   Clear and free a singly-linked list
*/
SNode *singleListFree(SNode* head) {
  SNode *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
      
    if (temp->arguments) {
      for (int i = 0; temp->arguments[i] != NULL; i++) {
	free(temp->arguments[i]);
      }
      free(temp->alias_name);
      free(temp->arguments);
    }
  
    free(temp);
  }
  return NULL;
}


/**
   Write a Singly linked list contents into a file
*/
void singleListWriteToFile(SNode* head, char *path) {
  if (head == NULL || path == NULL) {
    return;
  }    
  FILE *file = fopen(path, "w");
  if (file == NULL) {
    fprintf(stderr, "Failed to open file %s\n", path);
    return;
  }
  
  SNode *current = head;
  while (current != NULL) {
    fprintf(file, "alias %s ", current->alias_name);
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
   Read a file's contents into a Singly linked list
   Creates the file if it doesn't exist
*/
SNode* singleListReadFromFile(SNode *head, char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    // File doesn't exist, create it
    file = fopen(path, "w");
    if (file == NULL) {
      fprintf(stderr, "Failed to create file %s\n", path);
      return NULL;
    }
    fclose(file);
    // Return existing head since the new file is empty
    return head; 
  }
  
  char line[MAX_INPUT_LEN + 1];
  while (fgets(line, sizeof(line), file)) {
    //trimString(line);
    if (compareStrings(line, "")) {
      continue;
    }
    char **args = tokeniseString(line);
    if (args != NULL) {
      head = bindAlias(head, args);
      freeArguments(args);
    }
  }
  fclose(file);
  return head;
}


/**
   Print all nodes of a singly linked list
*/
void singleListPrint(SNode *head) {

  if (head == NULL){
     fprintf(stderr, "No aliases defined\n");
     return;
  }

  SNode *current = head;
  printf("Aliases: \n");
  while (current != NULL) {
    printf("%s:", current->alias_name);
    for (int i = 0; current->arguments[i] != NULL; i++) {
      printf(" %s", current->arguments[i]);
    }
    printf("\n");
    current = current->next;
  }
}

