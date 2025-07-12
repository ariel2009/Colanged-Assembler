#define PRE 1
#define POST 0

char *removeExtraSpaces(char *str);
int isExtraText(char *str);
char *getToken(char *str, char *delim);
int is_tok_in_str(char *str, char *tok);