

/* ✓  Find the user home directory from the environment */ 
/* ? Set current working directory to user home directory */
/* Save the current path */
/* Load history */
/* Load aliases */
/* ✓ Do while shell has not terminated */
/* ✓ Display prompt */
/* ✓ Read and parse user input */
/* While the command is a history invocation or alias then replace it with the */
/* appropriate command from history or the aliased command respectively */
/* If command is built-in invoke appropriate function */
/* Else execute command as an external process */
/* ✓ End while */
/* Save history */
/* Save aliases */
/* Restore original path */
/* ✓ Exit */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shfunc.h"
#include <linux/limits.h> // PATH_MAX

#define MAX_INPUT_LEN 512

int main() {

  char **arguments;
  char *userInputBuffer = malloc(MAX_INPUT_LEN); // Used for fgets()
  
  if (!userInputBuffer) {
    perror("Failed to allocate memory...");
    return 1;
  }

  // Set initial working directory to the home directory
  char *homeDirectory = getHomeDirectory();
  setWorkingDirectory(homeDirectory);  
  clearTerminal();

  do {

    // Display shell-like interface
    printf("%s $ ", getWorkingDirectory());

    
    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputted EOF (<CTRL> + D)

    /*
      BUG: if EOF is hit while there is text in the terminal input,
      EOF has to be hit twice
    */

    if (fgets(userInputBuffer, MAX_INPUT_LEN, stdin) == NULL) {
      printf("\n");
      break;
    }


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
    
    // Clear terminal
    else if (compareStrings(arguments[0], "clear")) {
      clearTerminal();
    }

    // Echo the command
    else if (compareStrings(arguments[0], "echo")) {
      for (int i = 1; arguments[i] != NULL; i++) {
	printf("%s\n", arguments[i]);
      }
    }

    // command isnt in the list
    else {
      printf("%s: command not found\n", arguments[0]);
    }
  }
  
  while (1);

  free(userInputBuffer);
  free(arguments);
  printf("Exiting... \n");
  return 0;
}
