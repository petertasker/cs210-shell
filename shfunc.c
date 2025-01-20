#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // chdir
#include <sys/types.h>
#include <pwd.h>

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

void setWorkingDirectory(char *homedir) {
  printf("Im setting the working directory to %s\n", homedir);
  // chdir CHanges the working DIRectory
  chdir(homedir);
  
}
