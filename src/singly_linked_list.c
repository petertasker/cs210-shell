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



/**
   Create a new Single list node
*/
SNode* singleCreateNode(char **arguments) {
  SNode* new_SNode = (SNode*)malloc(sizeof(SNode));
  if (new_SNode == NULL) {
    fprintf(stderr, "Failed to allocate space for new SNode\n");
    return NULL;
  }
  
  new_SNode->alias_name = strdup(arguments[1]);
  if (!new_SNode->alias_name) {
    fprintf(stderr, "Failed to allocat space for alias_name\n");
    free(new_SNode);
    return NULL;
  }

  // Count arguments
  int num_args = 0;
  while (arguments[num_args + 2] != NULL) {
    num_args++;
  }

  new_SNode->arguments = malloc((num_args + 1) * sizeof(char*));
  if (!new_SNode->arguments) {
    fprintf(stderr, "Failed to allocate space for arguments array\n");
    free(new_SNode->alias_name);
    free(new_SNode);
    return NULL;
  }
  
  for (int i = 0; i < num_args; i++) {
    new_SNode->arguments[i] = strdup(arguments[i + 2]);
    if (!new_SNode->arguments[i]) {
      for (int j = 0; j < i; j++) {
	free(new_SNode->arguments[j]);
      }
      free(new_SNode->alias_name);
      free(new_SNode->arguments);
      free(new_SNode);
      return NULL;
    }
  }  
  new_SNode->arguments[num_args] = NULL;
  
  new_SNode->next = NULL;

  return new_SNode;
}


/**
   Insert a Node at the beginning of a singly-linked list
*/
SNode* singleInsertNodeAtBeginning(SNode* head, char **arguments) {
  if (arguments == NULL) {
    return head;
  }
  
  // Create a new SNode
  SNode* new_SNode = singleCreateNode(arguments);
  if (new_SNode == NULL) {
    return head;
  }
  
  // Make next of new SNode as head
  new_SNode->next = head;
  return new_SNode;
}


/**
   Insert a Node at the end of a singly-linked list
*/
SNode* singleInsertNodeAtEnd(SNode* head, char **arguments) {
  if (arguments == NULL) {
    return head;
  }
    
  // Create a new SNode
  SNode *new_SNode = singleCreateNode(arguments);
  if (new_SNode == NULL) {
    return head;
  }
    
  if (head == NULL) {
    return new_SNode;
  }
    
  SNode *current = head;
     
  // Find end of linked list
  while (current->next != NULL) {
    current = current->next;
  }
  
  current->next = new_SNode;
    
  
  return head;
}


/**
   Delete a Node from a singly-linked list at a
   specified position

   especially useful - https://www.geeksforgeeks.org/delete-a-linked-list-node-at-a-given-position/
*/
SNode* singleDeleteNodeAtPosition(SNode *head, int pos) {
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
SNode *singleClearList(SNode* head) {
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
void singleWriteListToFile(SNode* head, char *path) {
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
*/
SNode* singleReadListFromFile(SNode *head, char *path) {
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
      //loadIntoAlias(&head, args);  
      freeArguments(args);  
    }
  }
  
  fclose(file);  // Close the file after reading
  return head;
}


/**
   Print all nodes of a singly linked list
*/
void singlePrintList(SNode *head) {
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

