/* Some internal functions which help out the main file */
#include <stdio.h>        // printf, perror
#include <stdlib.h>       // getenv, malloc, free
#include <string.h>       // strtok
#include <unistd.h>       // chdir, getcwd, getuid, execvp
#include <sys/types.h>    // getpwuid
#include <pwd.h>          // struct passwd, getpwuid
#include <linux/limits.h> // PATH_MAX
#include <unistd.h>
#include <wait.h>
#include <ctype.h>
#include <errno.h>

#include "constants.h"

char* getHomeDirectory(void) {
  // Check if the HOME enviroment variable is set
  char *home = getenv("HOME");
  if (home) {
    return home;
  }

  // Otherwise, get the details of the current user by
  // accessing their user ID and password for their user ID
  struct passwd *pw = getpwuid(getuid());

  // pw contains the home directory of the current user
  if (pw) {
    return pw->pw_dir;
  }

  return NULL;
}


void getWorkingDirectory(char *buffer) {
  // Replace the old path with the new path
  buffer = getcwd(buffer, PATH_MAX);
  if (!buffer) {
    perror("Failed to get working directory!");
  }
}


void setWorkingDirectory(char *arg) {
  // chdir CHanges the working DIRectory  
  if (chdir(arg) != 0) {
    perror("Failed to change directory");
  }
}


char **tokeniseUserInput(char *s) {
  // Make a local copy so the base userInputBuffer
  // can go directly to addToHistory


  if (!s) {
    return NULL;
  }
  
  char *copy = strdup(s);
  char **arguments = malloc(MAX_NUM_ARGS *sizeof(char *));
  if (!arguments) {
    free(copy);
    perror("Failed to allocate memory for arguments");
    return NULL;
  }

  char *token = strtok(copy, TOKEN_DELIMITERS);
  int i = 0;
  // Loop through the command and put each token in arguments
  while (token && i < MAX_NUM_ARGS) {
    arguments[i] = strdup(token);
    token = strtok(NULL, TOKEN_DELIMITERS);
    i++;
  }
  
  arguments[i] = NULL;
  free(copy);
  return arguments;
}


int compareStrings(char *input, char *arg) {
  return strcmp(input, arg) == 0;  
}



void externalCommands(char **args) {  
  /* External Processes:
     1. fork
     2. exec
     3. wait
     4. exit
     
     Problem here(?) Exiting a child process potentially
     doesn't free all the memory it uses
  */

  // Create a new process with fork and give it an ID (child)
  // and a parent ID
  pid_t pid = fork();
  
  // Fork failed
  if (pid == -1) {  
    perror("fork failed");
    return;
  }
  
  // pid = 0 is the child process
  if (pid == 0) {
    // Execute first argument as a command, with all other arguments as that 
    // as *that* command's arguments
    if (execvp(args[0], args) == -1) {
      fprintf(stderr , "%s: command not found\n", args[0]);  
      // here?
      exit(1);
      
    }
  }
  // Parent waits for the child to finish (pid has changed state)
  else {  
    int status;
    waitpid(pid, &status, 0);
  }
}


void trimString(char *s) {
  // Create copy of original pointer address
  char *original = s;
  while (isspace((unsigned char)*s)) {
    s++;
  }
  // Use memmove to shift the trimmed string to the start of the buffer
  memmove(original, s, strlen(s) + 1);
  original[strcspn(s, "\n")] = '\0';
}


void addToHistory(char **history, char *command) {
  // Don't have exit in history
  if (compareStrings(command, "exit")) {
    return;
  }
  // Don'lt have history invocations in history
  if (command[0] == '!') {
    return;
  }
  // Shift array to the right to make room for newest
  // (This array is recent ascending)
  for (int i = MAX_NUM_HISTORY - 1; i > 0; i--) {
    history[i] = history[i - 1];
  }
 
  // Add new command
  history[0] = strdup(command);
}


void freeHistory(char **history) {
  // Free every indice of history and then history itself
  for (int i = MAX_NUM_HISTORY - 1; i >= 0; i--) {
    free(history[i]);
  }
  free(history);
}


void writeHistoryToFile(char **history, char *initialDirectory) {
  // Get the local .hist.list file
  char *f = strdup(initialDirectory);
  strcat(f, HISTORY_FILE);

  FILE *fptr;
  fptr = fopen(f, "w");
  if (fptr == NULL) {
    perror("Error opening file");
    // Print the error number
    printf("Error number: %d\n", errno);
    return; // Exit if the file can't be opened
  }
  // Reuse code from internalComamands
  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    if (history[i] != NULL && *history[i] != '\0') {
      fprintf(fptr, "%s\n",history[i]);
    }
  }   
  fclose(fptr);
}

void readHistoryFromFile(char **history, char *initialDirectory) {

    char *f = strdup(initialDirectory);
    strcat(f, HISTORY_FILE);

    FILE *fptr = fopen(f, "r");
    if (fptr == NULL) {
      perror("Error opening file");
      return;
    }
    char line[MAX_INPUT_LEN + 1];
    int i = 0;

    // Copy each line of the file to history
    while (i < MAX_NUM_HISTORY && fgets(line, sizeof(line), fptr)) {          
      trimString(line);        
      strcpy(history[i], line);
      i++;
    }
    fclose(fptr);
}


char **invokeHistory(char **history, char *command) {
  // This can either be done on the fly
  // (this implementation), or pre-parsed
  // (Brian's implementation)


  printf("starting invoking history\n");
  
  // Create [1:] substring of command to lop ! off 
  char commandSubstr[sizeof(command)];
  strcpy(commandSubstr, command + 1);

  // If command is "!!"
  if (commandSubstr[0] == '!' && commandSubstr[1] == '\0') {
    return tokeniseUserInput(history[1]);
  }
  
  printf("no match!\n");
  // Check substring contains only numbers
  if (!isdigit(commandSubstr)) {
    fprintf(stderr, "You must invoke history with either !! or !<n>");
    return tokeniseUserInput(NULL);
  }
  
  
  return tokeniseUserInput(NULL);
}

  
 
