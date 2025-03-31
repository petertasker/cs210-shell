#include "singly_linked_list.h"

char *createBuffer();
char *saveInitialDirectory();
char *initialiseDirectory();
char *concatFilePath(char *);
void loadIntoHistory(DNode **, char **tokens);
void loadIntoAlias(SNode **, char **tokens);
char *saveInitialPath();
