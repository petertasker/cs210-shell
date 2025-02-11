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
  
  new_SNode->arguments = arguments;
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
*/
SNode* singleDeleteNodeAtPosition(SNode *head, int pos) {
  if (head == NULL || pos < 0) {
    printf("List is empty.\n");
    return NULL;
  }
    
  SNode *current = head;
  
  if (pos == 0) {
    head = current->next;
      
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
SNode* singleReadListFromFile(SNode* head, char *path) {
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
