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

    
    /* Edge Cases */

    
    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputted EOF (<CTRL> + D)

    // BUG: if EOF is hit while there is text in the terminal input,
    // EOF has to be hit twice

    if (fgets(userInputBuffer, MAX_INPUT_LEN, stdin) == NULL) {
      printf("\n"); // Formatting
      break;
    }
    
    // Remove the last character of the input (\n)
    userInputBuffer[strcspn(userInputBuffer, "\n")] = '\0';

    //  Quit the program
    if (compareStrings(userInputBuffer, "exit")){
      break;
    }
    
    // If buffer is empty (user has only hit <RET>), do nothing
    if (compareStrings(userInputBuffer, "")) {
      continue;
    }

    
    /* Regular Cases */

    
    /*
     There are four categories of command:
         1. History invocation
         2. Alias
         3. In-built command
         4. External command
    */
    
    arguments = tokeniseUserInput(userInputBuffer);
    for (int i = 0; arguments[i] != NULL; i++) {
      printf("argument: %s\n", arguments[i]);
    }
    
    // Clear Terminal
    if (compareStrings(userInputBuffer, "clear") || compareStrings(userInputBuffer, "cls")) {
        clearTerminal();
	
    }

  }
  

  while (1);

  free(userInputBuffer);
  free(arguments);
  printf("Exiting... \n");
  return 0;
}
