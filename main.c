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
  char *token; // used for tokenising user input
  
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
    printf("%s $", getWorkingDirectory());

    

    
    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputted EOF, (<CTRL> + D)
    
    if (fgets(userInputBuffer, MAX_INPUT_LEN, stdin) == NULL){ 
      printf("\n"); // formatting
      break;
    }

    // Remove the last line of the input (\n)
    // Note that this allows \n in the middle of commands so pasting is unaffected
    userInputBuffer[strcspn(userInputBuffer, "\n")] = '\0';

    //  also halt if the user types quit
    if (strcmp(userInputBuffer, "quit") == 0) {
      break;
    }
    
    // If buffer is empty (user has only hit <RET>), do nothing
    // This allows the prompt $ to show on the new line
    
    if (strcmp(userInputBuffer, "") == 0) {
      continue;
    }

    tokeniseUserInput(userInputBuffer);

    
    // (add more)

    // Edge Cases:
    // <CTRL + D>
    // <RET>
    // ...
    
    // Regular Cases:
    // quit
    // clear, clr
    // ...

    // TODO:
    // help
    // ...
    
    // TODO: tokenise the input using strtok() but only after it has been saved to the history
    //       ... this means we will be tokenising fly and will be making things much easier

    

    
    // Clear Terminal
    if (strcmp(userInputBuffer, "clear") == 0 || strcmp(userInputBuffer, "clr") == 0) {
        clearTerminal();
	
    }

    
  }
  

  while (1);

  free(userInputBuffer);
  printf("Exiting... \n");
  return 0;
}
