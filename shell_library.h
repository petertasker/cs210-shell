#include "linked_list.h"

#ifndef SHELL_LIBRARY_H
#define SHELL_LIBRARY_H
// Directory management
char *getHomeDirectory();
void setWorkingDirectory(char *);
void getWorkingDirectory(char *);

// Input parsing
char **tokeniseString(char *);
int compareStrings(char *, char *);
void externalCommands(char **);
void trimString(char *);

// History management
Node* addToHistory(Node*, char**);
void freeArguments(char **);
void writeHistoryToFile(Node*, char*);
void readHistoryFromFile(Node*, char*);
void clearHistory(Node*);
char **invokeHistory(Node*, char *);


void printHistory(Node*);

#endif
