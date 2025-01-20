// Main loop of the shell


/* ✓  Find the user home directory from the environment */ 
/* Set current working directory to user home directory */
/* Save the current path */
/* Load history */
/* Load aliases */
/* Do while shell has not terminated */
/* Display prompt */
/* Read and parse user input */
/* While the command is a history invocation or alias then replace it with the */
/* appropriate command from history or the aliased command respectively */
/* If command is built-in invoke appropriate function */
/* Else execute command as an external process */
/* End while */
/* Save history */
/* Save aliases */
/* Restore original path */
/* Exit */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "shfunc.h"
int main() {
  printf("Hello, world!!!\n");

  
  char *homedir = getHomeDirectory();
  setWorkingDirectory(homedir);

  return 0;
}
