/**
   A library of functions which are used inside the main shell
   script and the assisting files
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <linux/limits.h>
#include <unistd.h>
#include <wait.h>
#include <ctype.h>
#include <errno.h>

#include "linked_list.h"
#include "shell_library.h"
#include "constants.h"


/**
   Get the home directory
*/
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


/**
   Get the current working directory
*/
void getWorkingDirectory(char *buffer) {
  // Replace the old path with the new path
  buffer = getcwd(buffer, PATH_MAX);
  if (!buffer) {
    perror("Failed to get working directory!");
  }
}


/**
   Set the current working directory
*/
void setWorkingDirectory(char *arg) {
  // chdir CHanges the working DIRectory  
  if (chdir(arg) != 0) {
    perror("Failed to change directory");
  }
}


/**
   Tokenise a string using a series of delimeters
   the string is either:
       Input from stdin,
       An invocation of history, or
       An invocation of an alias
*/
char **tokeniseString(char *str) {
  if (!str) {
    return NULL;
  }

  char *copy_str = strdup(str);
  if (!copy_str) {
    perror("Failed to duplicate input string");
    return NULL;
  }

  char **arguments = malloc((ARG_MAX + 1) * sizeof(char *));
  if (!arguments) {
    free(copy_str);
    perror("Failed to allocate memory for arguments");
    return NULL;
  }

  int i = 0;
  char *token = strtok(copy_str, TOKEN_DELIMITERS);

  // Loop through the command and put each token in arguments
  while (token && i < ARG_MAX) {
    arguments[i] = strdup(token);
    token = strtok(NULL, TOKEN_DELIMITERS);
    i++;
  }

  arguments[i] = NULL;
  free(copy_str);
  return arguments;
}


/**
   Free the arguments of the command input
*/
void freeArguments(char **arguments) {
    // Early return if arguments is already NULL
    if (!arguments) {
        return;
    }

    // Free each string
    for (int i = 0; arguments[i] != NULL; i++) {
        free(arguments[i]);
    }

    // Free the array itself
    free(arguments);
}



/**
   Returns 1 if strings match
*/
int compareStrings(char *input, char *arg) {
  return (strcmp(input, arg) == 0);  
}


/**
   Pipe external commands

   Forks into a child process, and sleeps
   until the child gets killed or kills
   itself

   "Command not found" handling is found here
*/
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


/**
   Trim leading and trailing whitespace from a string
   Also trims newline characters
*/
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



/**
   Create a node of the newest command and add it to
   the beginning of history
*/
Node* addToHistory(Node* head_history, char **tokens) {
    if (!tokens || !*tokens) {
        printf("No tokens to add to history.\n");
        return head_history;
    }

    // Deep copy command (first token)
    char* command = strdup(tokens[0]);
    if (!command) {
      perror("Failed to allocate memory for command");
      return head_history;
    }
    
    // Count number of arguments
    int arg_count = 0;
    while (tokens[arg_count]) {
      arg_count++;
    }

    // Allocate memory for arguments array
    char** args = NULL;

    if (arg_count > 1) {
      // Allocate memory for arguments array if there are arguments
      args = malloc((arg_count) * sizeof(char*));
      if (!args) {
	perror("Failed to allocate memory for arguments");
	free(command);
	return head_history;
      }

      // Copy each argument
      for (int i = 1; i < arg_count; i++) {
	args[i - 1] = strdup(tokens[i]);
	if (!args[i - 1]) {
	  perror("Failed to allocate memory for an argument");
	  for (int j = 0; j < (i - 1); j++) {
	    free(args[j]);
	  }
	  free(args);
	  free(command);
	  return head_history;
	}	
      }
      args[arg_count - 1] = NULL;  // NULL-terminate the array
    }

    // Insert into history
    return insertNodeAtBeginning(head_history, command, args);
}


/**
   Invoke history from history array
   
   Destruction TBD
*/
char **invokeHistory(Node *head_history, char *command) {
  return "Hello!";
}
