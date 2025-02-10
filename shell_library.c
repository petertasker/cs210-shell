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
char **tokeniseString(char *s) {
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
    char** arguments = malloc((arg_count + 1) * sizeof(char*));
    if (!arguments) {
        perror("Failed to allocate memory for arguments");
        free(command);
        return head_history;
    }

    // Copy each argument
    for (int i = 0; i < arg_count; i++) {
        arguments[i] = strdup(tokens[i]);
        if (!arguments[i]) {
            perror("Failed to allocate memory for an argument");
            for (int j = 0; j < i; j++) {
                free(arguments[j]);
            }
            free(arguments);
            free(command);
            return head_history;
        }
    }
    arguments[arg_count] = NULL;  // NULL-terminate the array

    // Insert into history
    head_history = insertNodeAtBeginning(head_history, command, arguments);

    return head_history;
}


/**
   Write history to file

   Destruction TBD
*/
void writeHistoryToFile(Node* head_history, char *path) {
}

/**
   Read history from file

   Destruction TBD
*/
void readHistoryFromFile(Node* head_history, char *path) {
}


/**
   Delete all indexes of history

   Destruction TBD
*/
void clearHistory(Node* head_history) {
  clearList(head_history);
}


/**
   Invoke history from history array
   
   Destruction TBD
*/

char **invokeHistory(Node *head_history, char *command) {
  return "Hello!";
}

void printHistory(Node *head_history) {
  printList(head_history);
}
