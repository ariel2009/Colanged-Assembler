#include <stdio.h>

int isFileNameValid(char *fullName, char *allowed_ext);
FILE *tryOpenFile(char *fileNameToOpen, char *exten, char *mode);
void copy_file(FILE *src, FILE *dest, char* (*skip_until)(char *), char* (*exchange)(char *));