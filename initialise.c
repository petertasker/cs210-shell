/* Initalise some variables used in the main loop */
#include <stdio.h>
#include "shfunc.h"
#include <stdlib.h>
#include <linux/limits.h>


#define MAX_INPUT_LEN 511
#define MAX_NUM_HISTORY 20

char *createBuffer() {
  char *buf = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  if (!buf) {
    perror("Failed to allocate memory...");
    return NULL;
  }
  return buf;
}


char *saveInitialDirectory() {  

  // Working directory of the place which the shell was ran
  char *dir = malloc(PATH_MAX * sizeof(char));
  getWorkingDirectory(dir);
  if (!dir) {
    perror("Failed to allocate memory for cwd");
    return NULL;
  }
  return dir;
}


char *initialiseDirectory() {
  // Current working directory
  char *currentDirectory = malloc(PATH_MAX * sizeof(char));
  if (!currentDirectory) {
    perror("Failed to allocate memory for cwd");
  }
  // Initialise in the home directory
  setWorkingDirectory(getHomeDirectory());

  return currentDirectory;
}

char **initialiseHistory() {
  // Allocated fixed sized list of strings for history
  // malloc is evil and is only to be done in main.c
  char **history = malloc(MAX_NUM_HISTORY * sizeof(char*));


  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    history[i] = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
    if (!history[i]) {
      perror("Failed to allocate memory for history entry");
      // Free already allocated memory before returning
      for (int j = 0; j < i; j++) {
	free(history[j]);
      }
      free(history);
      return NULL;
    }
    history[i][0] = '\0'; // Initialize each string as empty
  }
  
  return history;
}
