#ifndef BUILT_IN_COMMANDS_H
#define BUILT_IN_COMMANDS_H

#include "linked_list.h"  // For Node type

/**
   Built in shell commands
*/
void echo(char **args);
void pwd(char *cwd, char **args);
void cd(char **args);

/**
   Alias management
*/
void printAliases(void);
void bindAlias(char **aliasNames, char **aliasCommands, char **args);
void unbindAlias(char **aliasNames, char **aliasCommands, char **args);

#endif 
