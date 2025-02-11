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
    fprintf(stdin, "Failed to allocate memory...");
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
    fprintf(stdin, "Failed to allocate memory for cwd");
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
    fprintf(stdin, "Failed to allocate memory for cwd");
  }
  // Initialise in the home directory
  strcpy(directory_current, getHomeDirectory());
  setWorkingDirectory(directory_current);
  return directory_current;
}

/**
  Old style initialise alias array
*/
char **initialiseAliasNames(){

  char **aliasNames = malloc(INITIAL_ALIAS_CAPACITY * sizeof(char*));
  if (!aliasNames) {
    perror("Failed to allocate memory for alias names array");
    return NULL;
  }

  // Initialize all pointers to NULL
  for (int i = 0; i < INITIAL_ALIAS_CAPACITY; i++) {
    aliasNames[i] = NULL;
  }

  // Allocate char arrays for each index of history
  for (int i = 0; i < INITIAL_ALIAS_CAPACITY; i++) {
    aliasNames[i] = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
    if (!aliasNames[i]) {

      // Clean up previously allocated memory
      for (int j = 0; j < i; j++) {
        free(aliasNames[j]);
      }

      free(aliasNames);
      perror("Failed to allocate memory for aliasNames");
      return NULL;
    }

    // Initialize each string as empty
    aliasNames[i][0] = '\0';
  }

  return aliasNames;
}

char **initialiseAliasCommands(){

  char **aliasCommands = malloc(INITIAL_ALIAS_CAPACITY * sizeof(char*));
  if (!aliasCommands) {
    perror("Failed to allocate memory for alias commands array");
    return NULL;
  }

  // Initialize all pointers to NULL
  for (int i = 0; i < INITIAL_ALIAS_CAPACITY; i++) {
    aliasCommands[i] = NULL;
  }

  // Allocate char arrays for each index of history
  for (int i = 0; i < INITIAL_ALIAS_CAPACITY; i++) {
    aliasCommands[i] = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
    if (!aliasCommands[i]) {

      // Clean up previously allocated memory
      for (int j = 0; j < i; j++) {
        free(aliasCommands[j]);
      }

      free(aliasCommands);
      perror("Failed to allocate memory for aliasCommands");
      return NULL;
    }

    // Initialize each string as empty
    aliasCommands[i][0] = '\0';
  }

  return aliasCommands;

}


/**
  Concatenate the home path with a file name suffix
*/
char* concatFilePath(char *fileName){
  // Concatenate file path with %HOME% prefix and filename suffix
  size_t len = strlen(getHomeDirectory()) + strlen(fileName) + 1;
  char *path = malloc(len);
  if (!path) {
    fprintf(stdin, "Failed to allocate memory for history path");
  }
  // Concatenate
  snprintf(path, len, "%s%s", getHomeDirectory(), fileName);
  return path;
}


/**
   Create a node of the newest command and add it to
   the beginning of history
*/
void loadIntoHistory(Node** head_history, char **tokens) {
  if (!tokens || !*tokens || !head_history) {
    return;
  }

  // Count number of arguments
  int arg_count = 0;
  while (tokens[arg_count]) {
    arg_count++;
  }

  // Allocate memory for arguments array
  char** args = malloc((arg_count + 1) * sizeof(char*));
  if (!args) {
    fprintf(stderr, "Failed to allocate memory for arguments\n");
    return;
  }

  // Copy the command itself
  args[0] = strdup(tokens[0]);
  if (!args[0]) {
    fprintf(stderr, "Failed to allocate memory for command\n");
    free(args);
    return;
  }

  // Copy additional arguments if any
  for (int i = 1; i < arg_count; i++) {
    args[i] = strdup(tokens[i]);
    if (!args[i]) {
      fprintf(stderr, "Failed to allocate memory for an argument\n");
      for (int j = 0; j < i; j++) {
	free(args[j]);
      }
      free(args);
      return;
    }
  }

  args[arg_count] = NULL;  // NULL-terminate the array
  
  *head_history = insertNodeAtEnd(*head_history, args);
  
}
