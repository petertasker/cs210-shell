

char *getHomeDirectory(void);

void setWorkingDirectory(char *);

char *getWorkingDirectory(void);

void clearTerminal(void);

char **tokeniseUserInput(char *);

int *compareStrings(char*, char*);

void externalCommands(char **);

char *trimWhitespace(char *s);
