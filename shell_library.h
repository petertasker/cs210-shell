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
void addToHistory(char **, char*);
void writeHistoryToFile(char **, char*);
void readHistoryFromFile(char **, char*);
void deleteHistory(char**);
char **invokeHistory(char **, char *);
int validHistoryInvocation(char *, int);
int getHistoryIndexForInvocation(char *, int);

// Memory management
void freeArguments(char **);
void freeHistory(char **);

