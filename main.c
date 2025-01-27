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
/* ✓ If command is built-in invoke appropriate function */
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
✓ setpath
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

// Memory issue: ending child process loses(?) some still reachable memory.
// Assumedly, this is all the memory that that child process used.
// This is not really a problem.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>     // PATH_MAX
#include "shfunc.h"           // some shell functions that we define
#include "internalCommands.h" // all internal commands

// If you decide to change these, make sure they are changed in shfunc.c
#define MAX_INPUT_LEN 511
#define MAX_NUM_HISTORY 20

int main() {

  system("clear");

  // Create input buffer and also a copy of the initial
  // pointer so the buffer can reset every cycle of the loop
  char *userInputBuffer = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  if (!userInputBuffer) {
    perror("Failed to allocate memory...");
   return 1;
  }
  
  char *userInputBufferCopy = userInputBuffer;
  

  // Working directory of the place which the shell was ran
  char *initialDirectory = malloc(PATH_MAX * sizeof(char));
  getWorkingDirectory(initialDirectory);
  if (!initialDirectory) {
    perror("Failed to allocate memory for cwd");
  }

  
  // Current working directory
  char *currentDirectory = malloc(PATH_MAX * sizeof(char));
  if (!currentDirectory) {
    perror("Failed to allocate memory for cwd");
  }

  // Initialise in the home directory
  setWorkingDirectory(getHomeDirectory());
  
  char **arguments = NULL;

  /* ------------- HISTORY AND ALIASES GO HERE!! ------------- */

  // Allocated fixed sized list of strings for history
  // malloc is evil and is not to be done in schfunc
  char **history = malloc(MAX_NUM_HISTORY * sizeof(char*));
  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    history[i] = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  }
  if (!history) {
    perror("Failed to allocate memory for history");
    exit(1);
  }
  
  
  // Main shell loop
  do {

    // Get current working directory
    getWorkingDirectory(currentDirectory);
    if (!currentDirectory) {
      perror("Error trying to get working directory");
      break;
    }

    
    // Display shell-like interface
    printf("%s $", currentDirectory);
    
    
    // Reset buffer pointer
    userInputBuffer = userInputBufferCopy; 

    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputted EOF (<CTRL> + D)
    if (fgets(userInputBuffer, MAX_INPUT_LEN, stdin) == NULL) {
      if (feof(stdin)) {
	// Formatting
	printf("\n");
	break;
      }
    }

    // Trim leading whitespace
    trimString(userInputBuffer);
    
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
      break;
    }

    /* By this point in the program, the user has inputted a command
       which should be saved. Note that in linux, invalid commands
       are also saved.
    */

  
    
    // Echo the command
    else if (compareStrings(arguments[0], "echo")) {
      echo(arguments);
    }
    // Print path
    else if (compareStrings(arguments[0], "pwd") ||	\
	     compareStrings(arguments[0], "getpath")) {
      pwd(currentDirectory);
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

    // Free arguments that is malloced in tokeniseUserInput()
    free(arguments);
    arguments = NULL;

    

  }
  while (1);

  // replenish directory
  setWorkingDirectory(initialDirectory);
  printf("Exiting...\n");


  if (arguments) {
    free(arguments);
  }
  
  free(currentDirectory);
  free(initialDirectory);
  free(userInputBufferCopy);
  
  return 0;

  
}
