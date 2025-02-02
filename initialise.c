
/* Initalise some variables used in the main loop */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include "shellFunctions.h"
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
  char *currentDirectory = malloc(PATH_MAX * sizeof(char));
  if (!currentDirectory) {
    perror("Failed to allocate memory for cwd");
  }
  // Initialise in the home directory
  strcpy(currentDirectory, getHomeDirectory());
  setWorkingDirectory(currentDirectory);
  return currentDirectory;
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


char* concatHistoryFile(char *initialDirectory){
  // Concatenate file of where the executable was ran with
  // ".hist.list"
  // Note that (./.hist.list) does't work because the program
  // changes the user's directory to %HOME%, which isn't
  // where the file is to be saved or loaded
  size_t len = strlen(initialDirectory) + strlen(HISTORY_FILE) + 1;
  char *path = malloc(len);
  if (!path) {
    perror("Failed to allocate memory for history path");
  }
  // Concatenate
  snprintf(path, len, "%s%s", initialDirectory, HISTORY_FILE);
  return path;
}
