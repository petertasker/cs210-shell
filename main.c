/* ✓ Find the user home directory from the environment */ 
/* ✓ Set current working directory to user home directory */
/* ✓ Save the current path */
/* ✓ Load history */
/* Load aliases */
/* ✓ Do while shell has not terminated */
/* ✓ Display prompt */
/* ✓ Read and parse user input */
/* While the command is a history invocation or alias then replace it with the */
/* appropriate command from history or the aliased command respectively */
/* ✓ If command is built-in invoke appropriate function */
/* ✓ Else execute command as an external process */
/* ✓ End while */
/* ✓ Save history */
/* Save aliases */
/* ✓ Restore original path */
/* ✓ Exit */

/*
Internal Commands:
✓ cd
✓ getpath
✓ setpath
✓ history
x !!
x !<no>
x !<no> -
x alias
x alias <name> <command>
x unalias
*/

// Memory Issue: ending child process loses(?) some still reachable memory.
// Assumedly, this is all the memory that that child process used.
// This is not really a problem.

// Weird Issue: Using an ANSI character such as up arrow overwrites the line
// that the shell input is taken. This is not a funtional problem

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>     // PATH_MAX
#include "shfunc.h"           // some shell functions that we define
#include "internalCommands.h" // all internal commands
#include "initialise.h"
#include "constants.h"

int main() {
  
  system("clear");

  // Create input buffer and also a copy of the initial
  // pointer so the buffer can reset every cycle of the loop
  char *userInputBuffer = createBuffer();
  char *userInputBufferCopy = userInputBuffer;
  
  // Directory of the place which the shell was ran
  char *initialDirectory = saveInitialDirectory();

  // Initialise working directory
  char *currentDirectory = initialiseDirectory();
  // Initialise arguments
  char **arguments = NULL;

  // Initialise history
  char **history = initialiseHistory();
  
  // Load local history
  readHistoryFromFile(history, initialDirectory);

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

    // History saving to go here
    addToHistory(history, userInputBuffer);
    
    // Tokenise the arguments into an array of strings
    arguments = tokeniseUserInput(userInputBuffer);

    // If history is invoked, it will be processed here
    // where the history becomes arguments
    if (arguments[0][0] == '!') {
      invokeHistory(history, arguments);
    }


    // Exit the program
    else if (compareStrings(arguments[0], "exit")) {
      break;
    }
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
    // Print History
    else if (compareStrings(arguments[0], "history")) {
      printHistory(history);
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

  // Save session history to file
  writeHistoryToFile(history, initialDirectory);

  
  if (arguments) {
    free(arguments);
  }
  
  freeHistory(history);
  free(currentDirectory);
  free(initialDirectory);
  free(userInputBufferCopy); // Important to free the copy as it points to userInputBuffer[0]


  printf("Exiting...\n");
  return 0;
  
  
}
