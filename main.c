// Main loop of the shell


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
    fflush(stdout);
    

    
    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputted EOF (<CTRL> + D)

    // BUG: if EOF is hit while there is text in the terminal input,
    // EOF has to be hit twice

    // Read user input
    if (fgets(userInputBuffer, MAX_INPUT_LEN, stdin) == NULL) {
      printf("\n"); // Formatting
      break;
    }


    // Remove the last line of the input (\n)
    userInputBuffer[strcspn(userInputBuffer, "\n")] = '\0';

    //  Quit the program
    if (compareStrings(userInputBuffer, "exit")){
      break;
    }
    
    // If buffer is empty (user has only hit <RET>), do nothing
    if (compareStrings(userInputBuffer, "")) {
      continue;
    }
    
    tokeniseUserInput(userInputBuffer);
    
    
    // Clear Terminal
    if (compareStrings(userInputBuffer, "clear") || compareStrings(userInputBuffer, "cls")) {
        clearTerminal();
	
    }

  }
  

  while (1);

  free(userInputBuffer);
  printf("Exiting... \n");
  return 0;
}
