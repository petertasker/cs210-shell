/**
   Implementation of some built-in shell functions
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_library.h"
#include "built_in_commands.h"
#include "doubly_linked_list.h"
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
    fprintf(stdout, "Failed to print directory: too many arguments provided\n");
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
    fprintf(stdout, "Failed to change directory: no arguments provided\n");
    return;
  }
  // Too many args
  else if (args[2] != NULL) {
    fprintf(stdout, "Failed to change directory: too many arguments provided\n");
    return;
  }
  setWorkingDirectory(args[1]);
}



void bindAlias(char **aliasNames, char **aliasCommands, char **args) {
  
}


void unbindAlias(char **aliasNames, char **aliasCommands, char **args) {
  
}
