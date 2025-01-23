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
#define TOKEN_DELIMITERS " \t|><,&;\n"
#define MAX_INPUT_LEN 512
#define MAX_NUM_ARGS 50


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

  // If this fails, return null
  return NULL;
}


char *getWorkingDirectory(void) {

  // Allocate memory for string of the working path  
  char *cwd = malloc(PATH_MAX);
  if (cwd == NULL) {
    perror("Malloc error!");
    return NULL;
  }
  
  if (getcwd(cwd, PATH_MAX) != NULL) {
    return cwd;
  }
  
  perror("Path error!");
  free(cwd);

  // If this fails, return null
  return NULL;
}


void setWorkingDirectory(char *arg) {
  // chdir CHanges the working DIRectory  
  if (chdir(arg) != 0) {
    perror("Failed to change directory");
  }
}


void clearTerminal(void) {
  // https://stackoverflow.com/questions/55672661/what-does-printf-033h-033j-do-in-c
  printf("\033[H\033[J");

}

char **tokeniseUserInput(char *s) {

  char **arguments = malloc(MAX_NUM_ARGS *sizeof(char *));

  if (!arguments) {
    perror("Failed to allocate memory for arguments");
    return NULL;
  }

  char *token = strtok(s, TOKEN_DELIMITERS);
  int i = 0;

  // Loop through each token and put it into memory, erasing the token afterwards
  // Should maybe be triple pointer later for history?
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
    // The child process does not have to exit, as it is replaced by the
    // external command
    if (execvp(args[0], args) == -1) {
      fprintf(stderr , "%s: command not found\n", args[0]);  
      exit(1);
    }
  }
    
    // Parent waits for the child to finish
  else {  
    int status;
    waitpid(pid, &status, 0);
  }	  
  
}

char* trimString(char *s) {
  char *original = s; // Store original pointer for later use

  while (isspace((unsigned char)*s)) {
    s++;
  }

  // Use memmove to shift the trimmed string to the start of the buffer
  memmove(original, s, strlen(s) + 1);

  return original;
}
