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
   Print working directory
*/
void pwd(char *path, char **args) {
  // Only one argument
  if (args[1] != NULL) {
    fprintf(stderr, "Failed to print directory: too many arguments provided\n");
    return;
  }
  
  printf("%s\n", path);
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
   Add an alias to a singly linked list

   If an alias exists with the same alias name,
   overwrite it
*/
SNode *bindAlias(SNode *head, char **args) {

  if (!args || !args[1]) {
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
void unbindAlias(SNode *head, char **args) {
  if (args[2] != NULL) {
    fprintf(stderr, "Failed to unbind alias: too many arguments provided\n");
    return;
  }

  singleListDeleteNodeByKey(head, args[1]);
}
