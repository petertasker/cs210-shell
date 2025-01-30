// Directory management
char *getHomeDirectory();
void setWorkingDirectory(char *);
void getWorkingDirectory(char *);

// Input Parsing
char **tokeniseUserInput(char *);
int *compareStrings(char *, char *);
void externalCommands(char **);
void trimString(char *);

// History management
void addToHistory(char **, char*);
void freeHistory(char **);
void writeHistoryToFile(char **, char*);
void readHistoryFromFile(char **, char*);
char **invokeHistory(char **, char *);
