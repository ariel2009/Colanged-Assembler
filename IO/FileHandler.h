#include <stdio.h>

int isFileNameValid(char *fullName, char *allowed_ext);
FILE *tryOpenFile(char *fileNameToOpen, char *exten, char *mode);