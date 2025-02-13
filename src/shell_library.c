/**
   A library of functions which are used inside the main shell
   script and the assisting files
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <linux/limits.h>
#include <unistd.h>
#include <wait.h>
#include <ctype.h>
#include <errno.h>

#include "doubly_linked_list.h"
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
    fprintf(stdout, "Failed to get working directory!");
  }
}


/**
   Set the current working directory
*/
void setWorkingDirectory(char *arg) {
  struct stat path_stat;

    // Check if the path exists and get its type
    if (stat(arg, &path_stat) != 0) {
        // Path does not exist
        fprintf(stdout, "%s: No such file or directory\n", arg);
        return;
    }

    // Check if the path is a directory
    if (!S_ISDIR(path_stat.st_mode)) {
        // Path is not a directory
        fprintf(stdout, "%s: Not a directory\n", arg);
        return;
    }

    // Change the working directory
    if (chdir(arg) != 0) {
        // chdir failed for some other reason
        fprintf(stdout, "%s: Failed to change directory\n", arg);
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
    fprintf(stdout, "Failed to duplicate input string");
    return NULL;
  }

  char **arguments = malloc((ARG_MAX + 1) * sizeof(char *));
  if (!arguments) {
    free(copy_str);
    fprintf(stdout, "Failed to allocate memory for arguments");
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
char **freeArguments(char **arguments) {
  // Early return if arguments is already NULL
  if (!arguments) {
    return NULL;
  }

  // Free each string
  for (int i = 0; arguments[i] != NULL; i++) {
    free(arguments[i]);
  }

  // Free the array itself
  free(arguments);

  return NULL;
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
    fprintf(stdout, "fork failed");
    return;
  }
  
  // pid = 0 is the child process
  if (pid == 0) {
     
    // Execute first argument as a command, with all other arguments
    // as *that* command's arguments
    if (execvp(args[0], args) == -1) {
      fprintf(stderr , "%s: command not found\n", args[0]);
      _exit(1);
      
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
DNode* addToHistory(DNode* head_history, char **tokens) {
  if (!tokens || !*tokens) {
    return head_history;
  }
    
  if (compareStrings(tokens[0], "exit")) {      
    return head_history;
  }

  if (compareStrings(tokens[0], "alias") && tokens[1] != NULL) {
    return head_history;
  }
  
  // Avoid duplicates
  if (head_history && compareStringArrays(head_history->arguments, tokens)) {
    return head_history;
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
    return head_history;
  }

  // Copy the command itself
  args[0] = strdup(tokens[0]);
  if (!args[0]) {
    fprintf(stderr, "Failed to allocate memory for command\n");
    free(args);
    return head_history;
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
      return head_history;
    }
  }

  args[arg_count] = NULL;  // NULL-terminate the array
  
  DNode *new_head = doubleListInsertNodeAtBeginning(head_history, args);
  
  // Delete oldest node if history exceeds MAX_NUM_HISTORY
  int size = 0;
  DNode *temp = new_head;
  while (temp) {
    size++;
    temp = temp->next;
  }

  if (size > MAX_NUM_HISTORY) {
    new_head = doubleListDeleteNodeAtPosition(new_head, MAX_NUM_HISTORY);
  }

  return new_head;
}


/**
   Returns 1 if arrays match 
*/
int compareStringArrays(char **a, char **b) {
  if (!a || !b) {
    return 0;
  }
  
  int i = 0;
  while (a[i] && b[i]) {
    // Mismatch found
    if (!compareStrings(a[i], b[i])) {
      return 0; 
    }
    i++;
  }
  // Both must end at NULL
  return a[i] == NULL && b[i] == NULL;
}


/**
   Invoke history from history list
*/
char **invokeHistory(DNode *head_history, char *user_command) {
  DNode *current = head_history;
  if (current == NULL) {
    printf("No previous command in history\n");
    return NULL;
  }

  // Handle !! command - return most recent command
  if (compareStrings(user_command, "!!")) {
    return duplicateArguments(head_history->arguments);
  }

  // Convert string to number, skipping the '!'
  char *endptr;
  int n = strtol(user_command + 1, &endptr, 10);

  // Check if conversion was successful and ensure !0 is not allowed
  if (*endptr != '\0' || n == 0) {
    printf("Invalid history index format\n");
    return NULL;
  }

  if (n > 0) {
    // Traverse forward from head (most recent) for positive n
    n--; // Adjust n since we start at most recent
    while (current != NULL && n > 0) {
      current = current->next;
      n--;
    }
  } else if (n < 0) {
    // Find tail for negative n (oldest commands)
    while (current && current->next != NULL) {
      current = current->next;
    }
    // Now current points to tail
    n = (-n) - 1; // Convert to positive and adjust
    while (current != NULL && n > 0) {
      current = current->previous;
      n--;
    }
  }

  // Check if we found a valid command
  if (current == NULL) {
    printf("History index out of range\n");
    return NULL;
  }

  return duplicateArguments(current->arguments);
}

/**
   Duplicate arguments so when invocing
   history, the arguments are new, and not
   pointing to the data in the linked list
*/
char **duplicateArguments(char **args) {
    if (args == NULL) return NULL;

    int count = 0;
    while (args[count] != NULL) count++;

    char **copy = malloc((count + 1) * sizeof(char *));
    if (!copy) return NULL;

    for (int i = 0; i < count; i++) {
        copy[i] = strdup(args[i]);  // Duplicate each argument
    }
    copy[count] = NULL; // NULL terminate the array

    return copy;
}

// Restores original path and prints
void printPathOnExit() {
    if (originalPath != NULL) {
        // Restore the original PATH
        if (setenv("PATH", originalPath, 1) == -1) {
            perror("setenv");
        } else {
            printf("Restored PATH: %s\n", originalPath);
        }

        // Free the allocated memory for originalPath
        free(originalPath);
    }
}

