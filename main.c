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

int main() {
  
  printf("Hello, world!!!\n");

  // Set initial home and working directories
  char *homedir = getHomeDirectory();
  setWorkingDirectory(homedir);

  
  // char buffer for the incoming arguments
  char buffer[512];

  clearTerminal();

  // Continuously loop until the user decides to quit
  
  do {

    printf("%s $", homedir);

    
    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputed EOF, (<CTRL> + D), and halt
    // the program

    // This cannot be done in the while() condition alongside quit
    // as the program can achieve undefined results due to what may
    // be in the buffer previously

    // You could have them together if you sacrifice the do-while
    // structure, but it is too appropriate for this kind of task

    // Thems the breaks
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { 
      printf("\n"); // formatting
      break;
    }
    char *token = strtok(buffer, " ");
   
    printf("token: %s\n", token);
    token = strtok(NULL, " ");
    
    
    // Once the EOF edge case is dealt with, remove the last line of the input which is (<RET>)
    // Note that this allows <RET> in the middle of commands so pasting is unnafected

    buffer[strcspn(buffer, "\n")] = '\0';

    
    // If buffer is empty (user has only hit <RET>), do nothing
    // This allows the prompt $ to show on the new line
    
    if (strcmp(buffer, "") == 0) {
      continue;
    }

    
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
    if (strcmp(buffer, "clear") == 0 || strcmp(buffer, "clr") == 0) {
        clearTerminal();
	
    }

    
  }
  
  // Quit the program
  while (strcmp(buffer, "quit") != 0);

  printf("Exiting... \n");
  return 0;
}
