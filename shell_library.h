#include "linked_list.h"

#ifndef SHELL_LIBRARY_H
#define SHELL_LIBRARY_H

/**
   Directory management
*/
char *getHomeDirectory();
void setWorkingDirectory(char *);
void getWorkingDirectory(char *);

// Input parsing
char **tokeniseString(char *);
int compareStrings(char *, char *);
void externalCommands(char **);
void trimString(char *);
void freeArguments(char **);
char **duplicateArguments(char **);

// History management
Node* addToHistory(Node*, char**);
char **invokeHistory(Node*, char *);

#endif
