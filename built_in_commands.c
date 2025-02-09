/**
   Implementation of some built-in shell functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_library.h"
#include "built_in_commands.h"
#include "linked_list.h"
#include "constants.h"

void echo(char **args) {
  // Loop through every argument and print
  for (int i = 1; args[i] != NULL; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");
}


void pwd(char *path, char **args) {
  // Only one argument
  if (args[1] != NULL) {
    fprintf(stdout, "Failed to print directory: too many arguments provided\n");
    return;
  }
  
  printf("%s\n", path);
}


void cd(char **args) {
  // In Linux shells, typing "cd" takes you to %HOME%
  if (args[1] == NULL) {
    fprintf(stdout, "Failed to change directory: no arguments provided\n");
    return;
  }
  
  // Too many args
  else if (args[2] != NULL) {
    fprintf(stdout, "Failed to change directory: too many arguments provided\n");
    return;
  }
  else {
    setWorkingDirectory(args[1]);
  }
}


void printHistory(char **history) {
  printf("History:\n");
  // Print all slots that are not NULL
  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    if (history[i] != NULL && *history[i] != '\0') {
      printf("%d: %s\n", i+1, history[i]);
    }
  }
}


void bindAlias(Node **head, char **args) {
  
}


void unbindAlias(Node **head, char **args) {
  
}
