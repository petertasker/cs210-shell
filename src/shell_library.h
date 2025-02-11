#include "doubly_linked_list.h"
#include "singly_linked_list.h"

#ifndef SHELL_LIBRARY_H
#define SHELL_LIBRARY_H

/**
   Directory management
*/
char *getHomeDirectory();
void setWorkingDirectory(char *);
void getWorkingDirectory(char *);

/**
   Input parsing
*/
char **tokeniseString(char *);
void trimString(char *);


/**
   History management
*/
DNode* addToHistory(DNode*, char**);
char **invokeHistory(DNode*, char *);


/**
   Comparison functions
*/
int compareStrings(char *, char *);
int compareStringArrays(char **, char**);


/**
   Argument management
*/
void freeArguments(char **);
char **duplicateArguments(char **);


/**
   External command handling
*/
void externalCommands(char **);

#endif
