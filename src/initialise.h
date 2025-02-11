char *createBuffer();
char *saveInitialDirectory();
char *initialiseDirectory();
char **initialiseAliasNames();
char **initialiseAliasCommands();
char *concatFilePath(char *);
void loadIntoHistory(Node**, char **tokens);
