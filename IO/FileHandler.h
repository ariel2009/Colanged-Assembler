#include <stdio.h>
#define TMP_FILE_NAME "no_spaces_tmp.astmp"

int isFileNameValid(char *fullName, char *allowed_ext);
FILE *tryOpenFile(char *fileNameToOpen, char *exten, char *mode);
void copy_file(FILE *src, FILE *dest, char* (*skip_until)(char *), char* (*exchange)(char *));
int prepare_no_extra_spaces_file(char *filename, char *ext);