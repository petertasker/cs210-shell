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

// Memory issue: ending child process loses(?) some still reachable memory.
// Assumedly, this is all the memory that that child process used.
// This is not really a problem.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>     // PATH_MAX
#include "shfunc.h"           // some shell functions that we define
#include "internalCommands.h" // all internal commands
#include "initialise.h"

// If you decide to change these, make sure they are changed in shfunc.c
#define MAX_INPUT_LEN 511
#define MAX_NUM_HISTORY 20

int main() {
  
  system("clear");

  // Create input buffer and also a copy of the initial
  // pointer so the buffer can reset every cycle of the loop
  char *userInputBuffer = createBuffer();
  char *userInputBufferCopy = userInputBuffer;
  
  // Directory of the place which the shell was ran
  char *initialDirectory = saveInitialDirectory();

  // Working directory
  char *currentDirectory = initialiseDirectory();

  // Initialise arguments
  char **arguments = NULL;
  
  // Main shell loop
  do {

    // Get current working directory
    getWorkingDirectory(currentDirectory);
    
    // Reset buffer pointer
    userInputBuffer = userInputBufferCopy; 

    // Display shell-like interface
    printf("%s $", currentDirectory);
    
    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputted EOF (<CTRL> + D)
    if (fgets(userInputBuffer, MAX_INPUT_LEN, stdin) == NULL) {
      if (feof(stdin)) {
	printf("\n");
	break;
      }
    }

    // Trim leading whitespace and NULL terminator
    trimString(userInputBuffer);
    
    // Edge case: user has inputted nothing
    if (compareStrings(userInputBuffer, "")) {
      continue;
    }

    // Tokenise the arguments into an array of strings
    arguments = tokeniseUserInput(userInputBuffer);
    
 
    // Exit the program
    if (compareStrings(arguments[0], "exit")) {
      break;
    }

    /* By this point in the program, the user has inputted a command
       which should be saved. Note that in linux, invalid commands
       are also saved.

       If the most recent command (the first in history matches the
       current command, then do nothing)
    */
    
    // History saving to go here
    addToHistory(history, userInputBuffer);

    // Echo the command
    else if (compareStrings(arguments[0], "echo")) {
      echo(arguments);
    }
    // Print path
    else if ((compareStrings(arguments[0], "pwd")) ||	\
	     compareStrings(arguments[0], "getpath")) {
      pwd(currentDirectory);
    }
    // Change directory
  else if ((compareStrings(arguments[0], "cd")) ||	\
      compareStrings(arguments[0], "setpath")) {
      cd(arguments);
    }
    // Command isnt in the list of internals, therefore
    // it is either external or does not exist
    else {
      externalCommands(arguments);
    }

    // Free arguments that is malloced in tokeniseUserInput()
    free(arguments);
    arguments = NULL;    

  }
  while (1);

  // Replenish directory
  setWorkingDirectory(initialDirectory);

  if (arguments) {
    free(arguments);
  }
  
  free(currentDirectory);
  free(initialDirectory);
  free(userInputBufferCopy); // Important to free the copy as it points to userInputBuffer[0]


  printf("Exiting...\n");
  return 0;

  
}
