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

#define TOKEN_DELIMITERS " \t\n;&><|"
#define MAX_INPUT_LEN 512
#define MAX_NUM_ARGS 5
#define MAX_NUM_HISTORY 20

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
  char **arguments = malloc(MAX_NUM_ARGS *sizeof(char *));
  if (!arguments) {
    perror("Failed to allocate memory for arguments");
    return NULL;
  }

  char *token = strtok(s, TOKEN_DELIMITERS);
  int i = 0;
  // Loop through the command and put each token in arguments
  while (token && i < MAX_NUM_ARGS) {
    arguments[i] = token;
    token = strtok(NULL, TOKEN_DELIMITERS);
    i++;
  }
  
  arguments[i] = NULL;
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
  s[strcspn(s, "\n")] = '\0';
}


void addToHistory(char **history, char *command) {

  if (compareStrings(command, "history")) {
      return;
    }

  // Shift array to the right
  for (int i = MAX_INPUT_LEN - 1; i > 0; i--) {
    history[i] = history[i - 1];
  }
 
  // Add new command
  history[0] = strdup(command);
}

void freeHistory(char **history) {
  for (int i = MAX_INPUT_LEN - 1; i >= 0; i--) {
    free(history[i]);
  }
  free(history);
}
