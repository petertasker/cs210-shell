char *getHomeDirectory();
void setWorkingDirectory(char *);
void getWorkingDirectory(char *);
char **tokeniseUserInput(char *);
int *compareStrings(char*, char*);
void externalCommands(char **);
void trimString(char *);
void addToHistory(char **, char*);
void freeHistory(char **);
