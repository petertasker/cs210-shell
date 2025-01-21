#include <stdio.h>        // printf, perror
#include <stdlib.h>       // getenv, malloc, free
#include <string.h>       // strtok
#include <unistd.h>       // chdir, getcwd, getuid
#include <sys/types.h>    // getpwuid
#include <pwd.h>          // struct passwd, getpwuid
#include <linux/limits.h> // PATH_MAX


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


void setWorkingDirectory(char *homedir) {

  // chdir CHanges the working DIRectory
  // We change it to the user's home directory
  chdir(homedir);
  
  if (chdir(homedir) != 0) {
    perror("Failed to change directory");
  }
}


void clearTerminal(void) {
  // https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
  printf("\e[1;1H\e[2J");
}


void tokeniseUserInput(char *s) {
  char *token = strtok(s, " ");
  while (token) {
    printf("token: %s\n", token);
    token = strtok(NULL, " ");
  }
}
