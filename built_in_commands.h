void echo(char **);
void pwd(char *, char **);
void cd(char **);   

void printHistory(char **);

// Alias management
void printAliases();
void bindAlias(Node*, char**);
void unbindAlias(Node*, char **);
