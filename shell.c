/**
   The main loop of the shell
*/


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
  Node *head_history = NULL;
  
  // Find .hist.list
  char *file_path_history = concatFilePath(HISTORY_FILE);
  
  // Find .aliases
  char *file_path_alias = concatFilePath(ALIAS_FILE);

  // Load local history
  //readHistoryFromFile(history, file_path_history);
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
	break;
      }
    }

    
    // Trim leading whitespace and NULL terminator
    trimString(buffer_user_input);
    
    // Edge case: user has inputted nothing
    if (compareStrings(buffer_user_input, "")) {
      continue;
    }

    
    arguments = tokeniseString(buffer_user_input);
    
    // Add command to history list
    head_history = addToHistory(head_history, arguments);
    
    // Tokenise the arguments into an array of strings
    // either from history or from the input buffer
    if (buffer_user_input[0] == '!') { 
      arguments = invokeHistory(head_history, buffer_user_input);	
      // If arguments is NULL an error has been thrown
      if (arguments == NULL) {
	continue;
      }
    }


    /**
       Internal Commands:
    */
    // Exit the program
    if (compareStrings(arguments[0], "exit")) {
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
      printHistory(head_history);
    }
    
    // Erase History
    // else if (compareStrings(arguments[0], "delhist")) {
    // deleteHistory(history);
    //}

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

  }
  while (1);


  // Replenish directory
  setWorkingDirectory(directory_initial);

  // Save session history to file
  //writeHistoryToFile(head_history, file_path_history);

  // Free malloc'd variables
  clearHistory(head_history);
  free(directory_current);
  free(directory_initial);
  free(file_path_history);
  free(file_path_alias);
  free(buffer_user_input_ptr); 

  printf("Exiting...\n");
  return 0;
  
  
}
