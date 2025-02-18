/**
   Implementation of some built-in shell functions
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_library.h"
#include "built_in_commands.h"
#include "doubly_linked_list.h"
#include "singly_linked_list.h"
#include "constants.h"


/**
   Echo input
*/
void echo(char **args) {
  // Loop through every argument and print
  for (int i = 1; args[i] != NULL; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");
}


/**
   Print current path
*/
void getpath(char **args) {
    // Checks to see if parameters are present
    if (args[1] != NULL) {
    fprintf(stderr, "Error: getpath does not accept parameters\n");
    return;
  }
    // Get the PATH environment variable
    char *path = getenv("PATH");
    if (path == NULL) {
        fprintf(stderr, "PATH environment variable not found\n");
        return;
    }

    // Print the PATH
    printf("PATH: %s\n", path);
}


/**
   Change working directory
*/
void cd(char **args) {
  // Not enough args
  if (args[1] == NULL) {
    fprintf(stderr, "Failed to change directory: no arguments provided\n");
    return;
  }
  // Too many args
  else if (args[2] != NULL) {
    fprintf(stderr, "Failed to change directory: too many arguments provided\n");
    return;
  }
  setWorkingDirectory(args[1]);
}

/**
   Change current path
*/
void setpath(char **args) {
  // Checks if no parameters are present
  if (args[1] == NULL) {
    fprintf(stderr, "setpath: missing argument\n");
    return;
  }
  // Checks if too many parameters are present
  if (args[2] != NULL) {
    fprintf(stderr, "setpath: too many arguments\n");
    return;
  }
  // perror if setenv fails
  if (setenv("PATH", args[1], 1) == -1) {
    perror("setpath");
    return;
  }

  printf("PATH set to: %s\n", args[1]);
}



/**
   Add an alias to a singly linked list

   If an alias exists with the same alias name,
   overwrite it
*/
SNode *bindAlias(SNode *head, char **args) {

  if (!args || !args[2]) {
    fprintf(stderr, "Failed to bind alias: to few arguments provided\n");
    return head;
  }

  SNode *current = head;
  int index = 0;

  while (current != NULL) {
    if (compareStrings(current->alias_name, args[1])) {
      head = singleListDeleteNodeAtPosition(head, index);
      break;
    }

    current = current->next;
    index++;
  }
  return singleListInsertNodeAtBeginning(head, args);
}


/**
   Unbind an alias
*/
SNode *unbindAlias(SNode *head, char **args) {

  if (head == NULL){
    fprintf(stderr, "Failed to unbind alias: no aliases declared\n");
    return head;
  }

  if (args[2] != NULL) {
    fprintf(stderr, "Failed to unbind alias: too many arguments provided\n");
    return head;
  }
  
  return singleListDeleteNodeByKey(head, args[1]);
  
}

char **invokeAlias(SNode *head, char *input) {
  SNode *current;
  char **resolvedArgs = NULL;
  char **resolvedAliases = NULL; // To keep track alias calls for cycles
  int resolvedCount = 0;
  int capacity = 10; // Initial capacity for resolvedAliases array

  // Allocate memory for the resolvedAliases array
  resolvedAliases = malloc(capacity * sizeof(char *));
  if (resolvedAliases == NULL) {
    perror("malloc");
    return NULL;
  }

  while (input) {
    current = head;
    int found = 0;

    // Check if the current input is already in the resolved list
    for (int i = 0; i < resolvedCount; i++) {
      if (compareStrings(resolvedAliases[i], input)) {
	fprintf(stderr, "Error: Alias cycle detected for '%s'\n", input);
	free(resolvedAliases); // Free the memory allocated for resolvedAliases
	return NULL;
      }
    }

    // Add the current input to the resolvedAliases list
    if (resolvedCount >= capacity) {
      // If array is full resize
      capacity *= 2;
      resolvedAliases = realloc(resolvedAliases, capacity * sizeof(char *));
      if (resolvedAliases == NULL) {
	perror("realloc");
	return NULL;
      }
    }
    resolvedAliases[resolvedCount++] = input;

    // Search for the alias
    while (current != NULL) {
      if (compareStrings(current->alias_name, input)) {
	resolvedArgs = duplicateArguments(current->arguments);
	input = resolvedArgs[0]; // Check if the first argument is another alias
	found = 1;
	break;
      }
      current = current->next;
    }

    if (!found) {
      break; // No more alias resolution possible
    }
  }

  free(resolvedAliases); // Free the memory allocated for resolvedAliases
  return resolvedArgs;
}




