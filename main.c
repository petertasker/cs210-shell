// History is capped at 20, but that value can be modified in constants.c
// Aliases are malloced on the fly because there is no alias cap.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>     // PATH_MAX
#include "shellFunctions.h"   // Some shell functions that we define
#include "internalCommands.h" // All internal commands

#include "initialise.h"       // Variables created before do-while loop
#include "constants.h"        // Constants

int main() {
  
  system("clear");

  // Create input buffer and also a copy of the initial
  // pointer so the buffer can reset every cycle of the loop
  char *userInputBuffer = createBuffer();
  char *userInputBufferCopy = userInputBuffer;
  
  // Directory of the place which the shell was ran
  char *initialDirectory = saveInitialDirectory();
  // Initialise working directory
  char *currentDirectory = initialiseDirectory();

  // Initialise arguments
  char **arguments = NULL;
  // Initialise aliases
  char **aliases = NULL;
  
  // Initialise history
  char **history = initialiseHistory();
  
  // Find .hist.list
  char *historyFilePath = concatFilePath(HISTORY_FILE);
  
  // Find .aliases
  char *aliasFilePath = concatFilePath(ALIAS_FILE);

  // Load local history
  readHistoryFromFile(history, historyFilePath);
  // readAliasesFromFile
  
  // Main shell loop
  do {

    // Get current working directory
    getWorkingDirectory(currentDirectory);
    
    // Reset buffer pointer
    userInputBuffer = userInputBufferCopy; 

    // Display shell-like interface
    printf("%s $", currentDirectory);
    
    // Call fgets for user input and instantly check if it is NULL,
    // this means that the user inputted EOF (<CTRL> + D)
    if (fgets(userInputBuffer, MAX_INPUT_LEN, stdin) == NULL) {
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
    trimString(userInputBuffer);
    
    // Edge case: user has inputted nothing
    if (compareStrings(userInputBuffer, "")) {
      continue;
    }
    
    // Add command to history
    addToHistory(history, userInputBuffer);
    
    // Tokenise the arguments into an array of strings
    // either from history or from the input buffer
    if (userInputBuffer[0] == '!') { 
      arguments = invokeHistory(history, userInputBuffer);	
      // If arguments is NULL an error has been thrown
      if (arguments == NULL) {
	continue;
      }
    }

    // else if alias is triggered <----------
    // turn arguments into [1:] of the alias command
    else {  
      arguments = tokeniseUserInput(userInputBuffer);
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
      pwd(currentDirectory, arguments);
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
      bindAlias(aliases, arguments);
    }

    else if (compareStrings(arguments[0], "unalias")) {
      unbindAlias(aliases, arguments);
    }
    
    // Command isnt in the list of internals, therefore
    // it is either external or does not exist
    else {
      externalCommands(arguments);
    }

    // Free arguments that is malloced in tokeniseUserInput()
    freeArguments(arguments);   
    arguments = NULL;
  }
  while (1);


  // Replenish directory
  setWorkingDirectory(initialDirectory);

  // Save session history to file
  writeHistoryToFile(history, historyFilePath);

  // Free malloc'd variables
  freeHistory(history);
  free(currentDirectory);
  free(initialDirectory);
  free(historyFilePath);
  free(aliasFilePath);
  // Important to free the copy as it points to userInputBuffer[0]
  free(userInputBufferCopy); 

  printf("Exiting...\n");
  return 0;
  
  
}
