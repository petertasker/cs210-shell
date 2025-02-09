// History is capped at 20, but that value can be modified in constants.c
// Aliases are malloced on the fly because there is no alias cap.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>     // PATH_MAX
#include "shell_library.h"   // Some shell functions that we define
#include "built_in_commands.h" // All internal commands
#include "linked_list.h"
#include "initialise.h"       // Variables created before do-while loop
#include "constants.h"        // Constants

int main() {
  
  system("clear");

  // Create input buffer and also a copy of the initial
  // pointer so the buffer can reset every cycle of the loop
  char *buffer_user_input = createBuffer();
  char *buffer_user_input_ptr = buffer_user_input;
  
  char *directory_initial = saveInitialDirectory();
  char *directory_current = initialiseDirectory();

  // Initialise arguments
  char **arguments = NULL;

  // Initialise aliases
  Node *head_aliases = NULL;
  
  // Initialise history
  char **history = initialiseHistory();
  
  // Find .hist.list
  char *file_path_history = concatFilePath(HISTORY_FILE);
  
  // Find .aliases
  char *file_path_alias = concatFilePath(ALIAS_FILE);

  // Load local history
  readHistoryFromFile(history, file_path_history);
  // readAliasesFromFile
  
  // Main shell loop
  do {

    // Get current working directory
    getWorkingDirectory(directory_current);
    
    // Reset buffer pointer
    buffer_user_input = buffer_user_input_ptr; 

    // Display shell-like interface
    printf("%s $", directory_current);
    
    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputted EOF (<CTRL> + D)
    if (fgets(buffer_user_input, MAX_INPUT_LEN, stdin) == NULL) {
      if (feof(stdin)) {
	printf("\n");
	// Free arguments once per loop
	if (arguments != NULL) {
	  freeArguments(arguments);
	  arguments = NULL;
	}
	
	break;
      }
    }
    
    // Trim leading whitespace and NULL terminator
    trimString(buffer_user_input);
    
    // Edge case: user has inputted nothing
    if (compareStrings(buffer_user_input, "")) {
      continue;
    }
    
    // Add command to history
    addToHistory(history, buffer_user_input);
    
    // Tokenise the arguments into an array of strings
    // either from history or from the input buffer
    if (buffer_user_input[0] == '!') { 
      arguments = invokeHistory(history, buffer_user_input);	
      // If arguments is NULL an error has been thrown
      if (arguments == NULL) {
	continue;
      }
    }

    // else if alias is triggered <----------
    // turn arguments into [1:] of the alias command
    else {  
      arguments = tokeniseString(buffer_user_input);
    }

    // Internal Commands:

    // Exit the program
    if (compareStrings(arguments[0], "exit")) {
      freeArguments(arguments);
      arguments = NULL;
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
      printHistory(history);
    }
    
    // Erase History
    else if (compareStrings(arguments[0], "delhist")) {
      deleteHistory(history);
    }

    // Bind/ View aliases <------------------------------ implement these two functions
    else if (compareStrings(arguments[0], "alias")) {
      bindAlias(&head_aliases, arguments);
    }

    else if (compareStrings(arguments[0], "unalias")) {
      unbindAlias(&head_aliases, arguments);
    }
    
    // Command isnt in the list of internals, therefore
    // it is either external or does not exist
    else {
      externalCommands(arguments);
    }

    // Free arguments that is malloced in tokeniseString()
    freeArguments(arguments);   
    arguments = NULL;
  }
  while (1);


  // Replenish directory
  setWorkingDirectory(directory_initial);

  // Save session history to file
  writeHistoryToFile(history, file_path_history);

  // Free malloc'd variables
  freeHistory(history);
  free(directory_current);
  free(directory_initial);
  free(file_path_history);
  free(file_path_alias);
  free(buffer_user_input_ptr); 

  printf("Exiting...\n");
  return 0;
  
  
}
