/**
   The main loop of the shell
*/

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "built_in_commands.h"
#include "constants.h"
#include "doubly_linked_list.h"
#include "initialise.h"
#include "shell_library.h"
#include "singly_linked_list.h"

char *originalPath = NULL;

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

  // Save pre-session PATH
  char *path_initial = saveInitialPath();
  if (path_initial == NULL) {
    return 1;
  }

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

    // Strip newline
    buffer_user_input[strcspn(buffer_user_input, "\n")] = '\0';

    // Edge case: user has inputted nothing
    if (isEmptyOrWhitespace(buffer_user_input)) {
      continue;
    }

    // Since string tokenise removes whitespace, and the tokenisation
    // is done after checking for invocation, the leading whitespace of
    // buffer_user_input must be manually trimmed so it can be checked
    // for being a history or alias invocation ie input is "     !!"
    char *buffer_user_input_trimmed = buffer_user_input;
    buffer_user_input_trimmed = trimWhitespace(buffer_user_input_trimmed);

    // Check for alias
    char **alias_arguments = invokeAlias(head_alias, buffer_user_input_trimmed);

    // Alias Found
    if (alias_arguments != NULL) {
      arguments = alias_arguments;
      char **alias_alias_name = tokeniseString(buffer_user_input);
      head_history = addToHistory(head_history, alias_alias_name);
      free(alias_alias_name);
    }

    // Alias not found
    else {
      // Tokenise user input
      arguments = tokeniseString(buffer_user_input);
      if (arguments == NULL || arguments[0] == NULL) {
        continue;
      }
      // Check for history invocation
      if (arguments[0][0] == '!') {
        arguments =
            invokeHistory(head_history, buffer_user_input_trimmed, head_alias);
        if (arguments == NULL) {
          continue;
        }
      }
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
    else if (compareStrings(arguments[0], "echo") ||
             compareStrings(arguments[0], "regurgitate")) {
      echo(arguments);
    }

    // Print path
    else if (compareStrings(arguments[0], "getpath")) {
      getpath(arguments);
    }

    // Change directory
    else if (compareStrings(arguments[0], "cd")) {
      cd(arguments);
    }

    // Change path
    else if (compareStrings(arguments[0], "setpath")) {
      setpath(arguments);
    }

    else if (compareStrings(arguments[0], "history")) {
      // Erase history
      if (compareStrings(arguments[1], "-d")) {
        head_history = doubleListFree(head_history);
      }
      // Print history
      else {
        doubleListPrint(head_history, arguments);
      }
    }

    // Bind alias / view aliases
    else if (compareStrings(arguments[0], "alias")) {
      if (!arguments[1]) {
        singleListPrint(head_alias);
      } else {
        head_alias = bindAlias(head_alias, arguments);
      }
    }

    // Unbind alias
    else if (compareStrings(arguments[0], "unalias")) {
      head_alias = unbindAlias(head_alias, arguments);
    }

    else {
      externalCommands(arguments);
    }

    arguments = freeArguments(arguments);

  } while (1);

  // Replenish directory to pre-session
  setWorkingDirectory(directory_initial);
  exitRestorePath(path_initial);
  path_initial = NULL;
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
