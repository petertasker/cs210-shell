#ifndef BUILT_IN_COMMANDS_H
#define BUILT_IN_COMMANDS_H

#include "linked_list.h"  // For Node type

// Basic shell commands
void echo(char **args);
void pwd(char *cwd, char **args);
void cd(char **args);
void printHistory(char **args);

// Alias management commands
void printAliases(void);
void bindAlias(Node **head, char **args);
void unbindAlias(Node **head, char **args);

#endif 
