#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shfunc.h"

int exitShell(void) {
  // This is a bit verbose, but exiting is put
  // in a function like this so we can have
  // every single internal command in one place
  return 1;
}


void echo(char **arguments) {
  // Loop through every argument and print
  for (int i = 1; arguments[i] != NULL; i++) {
    printf("%s ", arguments[i]);
  }
}


void pwd(void) {
  printf("%s\n", getWorkingDirectory());
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
   
