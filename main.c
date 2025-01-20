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

  // Set home and working directory
  char *homedir = getHomeDirectory();
  setWorkingDirectory(homedir);

  
  // char buffer for the incoming arguments
  char buffer[512];

  clearTerminal();
  // Continuously loop until the user decides to quit
  do {
    printf("%s $", homedir);

    // get line input of user
    fgets(buffer, sizeof(buffer), stdin);
    
    // remove the last character of the input (which is <RET>)
    buffer[strcspn(buffer, "\n")] = '\0';    
    
    // If buffer is empty (user has only hit <RET>), do nothing
    // This allows the prompt $ to show on the new line
    if (strcmp(buffer, "") == 0) {
      continue;
    }

    // Clear Terminal
    if (strcmp(buffer, "clear") == 0 || strcmp(buffer, "clr") == 0) {
        clearTerminal();
    }  
  }
  
  // Quit the program
  while (strcmp(buffer, "quit") != 0);

  
  return 0;
}
