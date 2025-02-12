/**
   The main loop of the shell
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include "shell_library.h"
#include "built_in_commands.h"
#include "doubly_linked_list.h"
#include "singly_linked_list.h"
#include "initialise.h"
#include "constants.h"


int main() {
  
  system("clear");

  // Create input buffer and also a copy of the initial
  // pointer so the buffer can reset every cycle of the loop
  char *buffer_user_input = createBuffer();
  char *buffer_user_input_ptr = buffer_user_input;

  // String array used for tokenising the command input
  char **arguments = NULL;
  
  // Save pre-session directory and
  // set session directory to HOME
  char *directory_initial = saveInitialDirectory();
  char *directory_current = initialiseDirectory();

  // Set read / write file paths
  char *file_path_history = concatFilePath(HISTORY_FILE);
  char *file_path_alias = concatFilePath(ALIAS_FILE);
  
  // Load history into doubly-linked list
  DNode *head_history = NULL;
  head_history = doubleListReadFromFile(head_history, file_path_history);

  // Load aliases into singly-linked list
  SNode *head_alias = NULL;
  head_alias = singleListReadFromFile(head_alias, file_path_alias);
  

  // Main shell loop
  do {
    
    getWorkingDirectory(directory_current);
    
    // Reset buffer pointer
    buffer_user_input = buffer_user_input_ptr; 
    
    // Display shell-like interface
    printf("%s $", directory_current);
    
    // Check user input for EOF (<CTRL> + D) and escape if so
    if (fgets(buffer_user_input, MAX_INPUT_LEN, stdin) == NULL) {
      if (feof(stdin)) {
	if (arguments != NULL) {
	  freeArguments(arguments);
	  arguments = NULL;
	}
	printf("\n");
	break;
      }
    }

    // Trim leading whitespace and NULL terminator
    trimString(buffer_user_input);
    
    // Edge case: user has inputted nothing
    if (compareStrings(buffer_user_input, "")) {
      continue;
    }

    // Invoke history
    if (buffer_user_input[0] == '!') {
      arguments = invokeHistory(head_history, buffer_user_input);
      if (arguments == NULL) {
	continue;  
      }
    }
    else {
      // Take stdin as the command
      arguments = tokeniseString(buffer_user_input);
      head_history = addToHistory(head_history, arguments);
    
    }
    

    /**
       Internal Commands:
    */
    // Exit the program
    if (compareStrings(arguments[0], "exit")) {
      arguments = freeArguments(arguments);
      break;
    }
    
    // Echo the command
    else if (compareStrings(arguments[0], "echo") ||	\
	     compareStrings(arguments[0], "regurgitate")) {
      echo(arguments);
    }
    
    // Print path
    else if ((compareStrings(arguments[0], "pwd")) ||	\
	     compareStrings(arguments[0], "getpath")) {
      pwd(directory_current, arguments);
    }
    
    // Change directory
    else if ((compareStrings(arguments[0], "cd")) ||	\
	     compareStrings(arguments[0], "setpath")) {
      cd(arguments);
    }
    
    // Print History
    else if (compareStrings(arguments[0], "history")) {
      doubleListPrint(head_history);
    }
    
    // Erase History
    else if (compareStrings(arguments[0], "delhist")) {
      head_history = doubleListFree(head_history);
    }
    
    // Bind alias / view aliases
    else if (compareStrings(arguments[0], "alias")) {
      if (!arguments[1]) {
	singleListPrint(head_alias);
      }
      else {
	head_alias = bindAlias(head_alias, arguments);
      }
    }
    // Unbind alias
    //else if (compareStrings(arguments[0], "unalias")) {
    // unbindAlias(aliasNames, aliasCommands, arguments);
    //}
    
    else {
      externalCommands(arguments);
    }
    
    arguments = freeArguments(arguments);

  }
  while (1);
  
  // Replenish directory to pre-session
  setWorkingDirectory(directory_initial);

  // Save session lists to file
  doubleListWriteToFile(head_history, file_path_history);
  singleListWriteToFile(head_alias, file_path_alias);
	
  // Free lists
  head_history = doubleListFree(head_history);
  head_history = NULL;

  head_alias = singleListFree(head_alias);
  head_alias = NULL;

  // Free strings
  free(directory_current);
  free(directory_initial);
  free(file_path_history);
  free(file_path_alias);
  free(buffer_user_input_ptr); 

  printf("Exiting...\n");
  return 0;
  
  
}
