/* Some internal functions which help out the main file */

#include <stdio.h>        // printf, perror
#include <stdlib.h>       // getenv, malloc, free
#include <string.h>       // strtok
#include <unistd.h>       // chdir, getcwd, getuid, execvp
#include <sys/types.h>    // getpwuid
#include <pwd.h>          // struct passwd, getpwuid
#include <linux/limits.h> // PATH_MAX, ARG_MAX
#include <unistd.h>
#include <wait.h>
#include <ctype.h>
#include <errno.h>

#include "shell_library.h"
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
  if (!copy) {
    perror("Failed to duplicate input string");
    return NULL;
  }

  char **arguments = malloc((ARG_MAX + 1) *sizeof(char *));
  if (!arguments) {
    free(copy);
    perror("Failed to allocate memory for arguments");
    return NULL;
  }
  
  int i = 0;
  char *token = strtok(copy, TOKEN_DELIMITERS);
  // Loop through the command and put each token in arguments
  while (token && i < ARG_MAX) {
    arguments[i] = strdup(token);
    token = strtok(NULL, TOKEN_DELIMITERS);
    i++;
  }
  
  arguments[i] = NULL;
  free(copy);
  return arguments;
}


void freeArguments(char **arguments) {
    // Early return if arguments is already NULL
    if (arguments == NULL) {
        return;
    }

    // Free each string
    for (int i = 0; arguments[i] != NULL; i++) {
        free(arguments[i]);
    }

    // Free the array itself
    free(arguments);
}


void freeHistory(char **history) {
  if (!history) {
    return;
  }
  
  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    if (history[i] != NULL) {
      // Free strings
      free(history[i]);
      history[i] = NULL;
    }
  }
  // Free empty array
  free(history); 
}

int compareStrings(char *input, char *arg) {
  return (strcmp(input, arg) == 0);  
}


void externalCommands(char **args) {  
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
     
    // Execute first argument as a command, with all other arguments
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
  // Don't have exit or invocations in history
  if (compareStrings(command, "exit") || command[0] == '!') {
    return;
  }
  // If command == most recent history entry, return
  if (history[0]) {
    if (compareStrings(command, history[0])) {
      return;
      }
  }
    

  // Erase memory of old command
  free(history[MAX_NUM_HISTORY - 1]);

  // Shift array to the right to make room for newest
  // (This array is recent ascending)
  for (int i = MAX_NUM_HISTORY - 1; i > 0; i--) {
    history[i] = history[i - 1];
  }
  
  // Add new command
  history[0] = strdup(command);
}

  
void writeHistoryToFile(char **history, char *path) {
  FILE *fptr;
  fptr = fopen(path, "w");
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


void readHistoryFromFile(char **history, char *path) {

    FILE *fptr = fopen(path, "r");
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


void deleteHistory(char **history){
  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    if (history[i]) {
      history[i][0] = '\0';
    }
  }
}


char **invokeHistory(char **history, char *command) {
  
  // Gets how many commands are in history
  int current_history_size = 0;
  for (int i = 0; i < MAX_NUM_HISTORY; i++) {
    if (history[i] != NULL && history[i][0] != '\0') {
      current_history_size++;
    } else {
      break; // Breaks out if reach empty location
    }
  }

  // If there are no entries in history
  if (!current_history_size) {
    printf("There are no entries in history\n");
    return NULL;
  }
  /*
    index = -1 -> index[0]
    index = -2 -> error
    index >= 0 -> good

   */
  int index = validHistoryInvocation(command, current_history_size);

  if (index == -1) {
    return tokeniseUserInput(history[0]);
  }
  if (index >= 0) {
    
    if (index >= current_history_size || history[index][0] == '\0') {
        fprintf(stderr, "Failed to invoke history: history %d doesn't exist!\n", index);
        return NULL;
    }

    // Else ..
    return tokeniseUserInput(history[index]);
  }

  return NULL;
}


int validHistoryInvocation(char *command, int current_history_size) {
  // Skip the first character (!)
  char *command_substring = command + 1;

  // If command is "!!"
  if (command_substring[0] == '!' && command_substring[1] == '\0') {
    return -1;
  }

  // If command is "!-<no>"
  if (command_substring[0] == '-') {
    // !--<n> not allowed
    if (!isdigit(command_substring[1])) {
      fprintf(stderr, "Failed to invoke history: !-- not accepted\n");
      return -2;
    }

    if (command_substring[1] == '0') {
      fprintf(stderr, "Failed to invoke history: cannot invoke -0\n");
      return -2;
    }
    
    int inputIndex = atoi(command_substring + 1);
    if (inputIndex <= -1 || inputIndex >= current_history_size) {
      fprintf(stderr, "Failed to invoke history: invalid history index\n");
      return -2;
    }
    return current_history_size - inputIndex;
  }


  // Return the index of history
  return getHistoryIndexForInvocation(command_substring , current_history_size);
}

int getHistoryIndexForInvocation(char *command, int current_history_size) {
  int historyIndex = 0;

  // Check if each character is a digit
  for (int i = 0; command[i] != '\0'; i++) {
    if (!isdigit(command[i])) {
      fprintf(stderr, "You must invoke history with either !! or !<n>\n");
      return -2;
    }
    // Shift digits
    historyIndex = historyIndex * 10 + (command[i] - '0');
  }

  // Number out of range
  if (historyIndex < 1 || historyIndex > current_history_size) {
    fprintf(stderr, "Failed to invoke history: out of range. Valid range is 1 to %d.\n", current_history_size);
    return -2;
  }

  return historyIndex - 1;
}

