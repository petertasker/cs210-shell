/* ✓  Find the user home directory from the environment */ 
/* ✓ Set current working directory to user home directory */
/* ✓ Save the current path */
/* Load history */
/* Load aliases */
/* ✓ Do while shell has not terminated */
/* ✓ Display prompt */
/* ✓ Read and parse user input */
/* While the command is a history invocation or alias then replace it with the */
/* appropriate command from history or the aliased command respectively */
/* ? If command is built-in invoke appropriate function */
/* ✓ Else execute command as an external process */
/* ✓ End while */
/* Save history */
/* Save aliases */
/* ✓ Restore original path */
/* ✓ Exit */

/*
Internal Commands:
✓ cd
✓ getpath
x setpath
x history
x !!
x !<no>
x !<no> -
x alias
x alias <name> <command>
x unalias
*/


/*
Note the difference in use between perror() and fprintf().
fprintf() is to give the user public facing errors, while
perror() is for system'y errors, such as malloc
This is becuase perror suceeding prints "Success" to the screen
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>     // PATH_MAX
#include "shfunc.h"           // some shell functions that we define
#include "internalCommands.h" // all internal commands

#define MAX_INPUT_LEN 512

int main() {
  // Manually clear shell (this only works on UNIX based terminals)
  system("clear");
  
  int exitFlag = 0;
  char **arguments = NULL;

  // Create input buffer and also a copy of the initial
  // pointer so the buffer can reset every cycle of the loop
  char *userInputBuffer = malloc(MAX_INPUT_LEN);
  char *userInputBufferCopy = userInputBuffer;
  
  if (!userInputBuffer) {
    perror("Failed to allocate memory...");
    return 1;
  }

  // Get current working directory so it can be restored on exit
  char *homeDirectory = getHomeDirectory();
  char *initialDirectory = getWorkingDirectory();
  
  // Set initial working directory to the home directory
  setWorkingDirectory(homeDirectory);  

  /* ------------- HISTORY AND ALIASES GO HERE!! ------------- */

  
  do {
    // Get current working directory
    char *cwd = getWorkingDirectory();
    if (cwd == NULL) {
      perror("Error trying to get working directory");
      exitFlag = exitShell();
    }

    
    // Display shell-like interface
    printf("%s $", cwd);
    free(cwd);

    
    // Reset buffer pointer
    userInputBuffer = userInputBufferCopy; 

    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputted EOF (<CTRL> + D)
    if (fgets(userInputBuffer, MAX_INPUT_LEN, stdin) == NULL) {
      if (feof(stdin)) {
        exitFlag = 1;
      }
    }

    // trim leading whitespace
    userInputBuffer = trimString(userInputBuffer);
    
    // Remove the last character of the input (\n)
    userInputBuffer[strcspn(userInputBuffer, "\n")] = '\0';

  
    // Edge case: user has inputted nothing
    if (compareStrings(userInputBuffer, "")) {
      continue;
    }

    // Tokenise the arguments into an array of strings

    arguments = tokeniseUserInput(userInputBuffer);
    //for (int i = 0; arguments[i] != NULL; i++) {
    //  printf("argument: %s\n", arguments[i]);
    //}
    

    /* --------- Internal Commands ---------*/
    // NOTE: else-if is an antipattern but unless someone wants
    // to create a hash function thems the breaks

    
    // Exit the program
    if (compareStrings(arguments[0], "exit")) {
      exitFlag = 1;
    }    
  
    // Echo the command
    else if (compareStrings(arguments[0], "echo")) {
      echo(arguments);
    }

    // Print path
    else if (compareStrings(arguments[0], "pwd") ||	\
	     compareStrings(arguments[0], "getpath")) {
      pwd();
    }
    
    // Set path
    else if (compareStrings(arguments[0], "setpath")) {
	if (arguments[1] == NULL) {
	  fprintf(stdout, "Failed to set path: a path must be given\n");
	}
	else if (arguments[3] != NULL) {
	  fprintf(stdout, "Failed to set path: only one argument must be given\n");
	}
	else {
	  setWorkingDirectory(arguments[1]);
	}
    }

    // Change directory
    else if (compareStrings(arguments[0], "cd")) {
      cd(arguments);
    }
    
    // command isnt in the list of internals, therefore
    // it is either external or does not exist
    else {
      externalCommands(arguments);
    }

    // reset arguments in memory every time as to not leak any memory
    free(arguments);
  }
  while (!exitFlag);

  // replenish directory
  setWorkingDirectory(initialDirectory);
  printf("\nExiting...\n\n");

  free(userInputBufferCopy);
  
  return 0;


}
