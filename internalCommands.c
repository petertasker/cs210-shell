/* Implementation of some built-in shell functions */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shellFunctions.h"
#include "constants.h"

void echo(char **arguments) {
  // Loop through every argument and print
  for (int i = 1; arguments[i] != NULL; i++) {
    printf("%s ", arguments[i]);
  }
  printf("\n");
}


void pwd(char *currentDirectory) {
  printf("%s\n", currentDirectory);
}


void cd(char **arguments) {
  // In Linux shells, typing "cd" takes you to %HOME%
  if (arguments[1] == NULL) {
    setWorkingDirectory(getHomeDirectory());
  }
  
  // Too many args
  else if (arguments[2] != NULL) {
    fprintf(stdout, "Failed to change directory: too many arguments provided\n");
    return;
  }
  else {
    setWorkingDirectory(arguments[1]);
  }
}

void printHistory(char **history) {
  printf("History:\n");
  // Print all slots that are not NULL
  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    if (history[i] != NULL && *history[i] != '\0') {
      printf("%d: %s\n", i, history[i]);
    }
  }
}
