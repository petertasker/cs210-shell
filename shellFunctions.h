// Directory management
char *getHomeDirectory();
void setWorkingDirectory(char *);
void getWorkingDirectory(char *);

// Input parsing
char **tokeniseUserInput(char *);
int compareStrings(char *, char *);
void externalCommands(char **);
void trimString(char *);
void freeArguments(char **);

// History management
void addToHistory(char **, char*);
void freeHistory(char **);
void writeHistoryToFile(char **, char*);
void readHistoryFromFile(char **, char*);
char **invokeHistory(char **, char *);
int validHistoryInvocation(char *);
int getHistoryIndexForInvocation(char *);
