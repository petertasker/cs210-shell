/* Initalise some variables used in the main loop */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include "shell_library.h"
#include "constants.h"


char *createBuffer() {
  // Create user input buffer
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
  if (!dir) {
    perror("Failed to allocate memory for cwd");
    return NULL;
  }
  getWorkingDirectory(dir);
  return dir;
}


char *initialiseDirectory() {
  // Current working directory
  char *directory_current = malloc(PATH_MAX * sizeof(char));
  if (!directory_current) {
    perror("Failed to allocate memory for cwd");
  }
  // Initialise in the home directory
  strcpy(directory_current, getHomeDirectory());
  setWorkingDirectory(directory_current);
  return directory_current;
}


char **initialiseHistory() {
  // Allocate array of pointers for history
  char **history = malloc(MAX_NUM_HISTORY * sizeof(char*));
  if (!history) {
    perror("Failed to allocate memory for history array");
    return NULL;
  }
  // Initialize all pointers to NULL
  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    history[i] = NULL;
  }

  // Allocate char arrays for each index of history
  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    history[i] = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
    if (!history[i]) {
      
      // Clean up previously allocated memory
      for (int j = 0; j < i; j++) {
	free(history[j]);
      }

      free(history);
      perror("Failed to allocate memory for history entry");
      return NULL;
    }

    // Initialize each string as empty
    history[i][0] = '\0';
  }

  return history;
}


char* concatFilePath(char *fileName){
  // Concatenate file path with %HOME% prefix and filename suffix
  size_t len = strlen(getHomeDirectory()) + strlen(fileName) + 1;
  char *path = malloc(len);
  if (!path) {
    perror("Failed to allocate memory for history path");
  }
  // Concatenate
  snprintf(path, len, "%s%s", getHomeDirectory(), fileName);
  return path;
}
