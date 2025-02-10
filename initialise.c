/**
   Variables that are initialised before the main loop
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "shell_library.h"
#include "constants.h"


/**
   Create buffer for stdin
*/
char *createBuffer() {
  // Create user input buffer
  char *buf = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  if (!buf) {
    perror("Failed to allocate memory...");
    return NULL;
  }
  return buf;
}


/**
   Save the working directory that the shell
   was ran in, to be restored later
*/
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


/**
   Set initial directory to home
*/
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


/**
   Concatenate the home path with a file name suffix
*/
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
