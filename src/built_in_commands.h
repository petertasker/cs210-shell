#ifndef BUILT_IN_COMMANDS_H
#define BUILT_IN_COMMANDS_H

#include "doubly_linked_list.h"  // For Node type
#include "singly_linked_list.h"

/**
   Built in shell commands
*/
void echo(char **args);
void getpath(char **args);
void cd(char **args);
void setpath(char **args);


/**
   Alias management
*/
void printAliases(void);
SNode *bindAlias(SNode *, char **args);
SNode *unbindAlias(SNode *, char **args);
char **invokeAlias(SNode *head, char *input);
#endif 
