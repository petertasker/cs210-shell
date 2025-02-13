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
  if (args[2] != NULL) {
    fprintf(stderr, "Failed to unbind alias: too many arguments provided\n");
    return head;
  }
  
  return singleListDeleteNodeByKey(head, args[1]);
  
}


char **invokeAlias(SNode *head, char *input) {
  SNode *current = head;
  while (current != NULL) {
    if (compareStrings(current->alias_name, input)) {
      printf("Invoking alias...\n");
      return duplicateArguments(current->arguments);
    }
    current = current->next;
  }
  return NULL;
}

/**
   Internal Commands:
*/
int runInternalCommands(char **arguments, DNode *head_history, SNode *head_alias) {
  // Echo the command
  if (compareStrings(arguments[0], "echo") ||		\
      compareStrings(arguments[0], "regurgitate")) {
    echo(arguments);
    return 1;
  }

  // Print path
  if (compareStrings(arguments[0], "getpath")) {
    getpath(arguments);
    return 1;
  }

   
  // Change path
  if (compareStrings(arguments[0], "setpath")) {
    setpath(arguments);
    return 1;
  }

  // Change directory
  if (compareStrings(arguments[0], "cd")) {
    cd(arguments);
    return 1;
  }

  // Print History
  if (compareStrings(arguments[0], "history")) {
    doubleListPrint(head_history);
    return 1;
  }

  // Erase History
  if (compareStrings(arguments[0], "delhist")) {
    head_history = doubleListFree(head_history);
    return 1;
  }

  // Bind alias / view aliases
  if (compareStrings(arguments[0], "alias")) {
    if (!arguments[1]) {
      singleListPrint(head_alias);
    }
    else {
      head_alias = bindAlias(head_alias, arguments);
    }
    return 1;
  }

  // Unbind alias
  if (compareStrings(arguments[0], "unalias")) {
    head_alias = unbindAlias(head_alias, arguments);
    return 1;
  }

  return 0;
}
