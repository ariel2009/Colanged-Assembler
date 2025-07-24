#include <stdio.h>

int is_extension_valid(char *fullName, char *allowed_ext);
int can_open_file(char *fileNameToOpen, char *exten, char *mode);
int prepare_no_extra_spaces_file(char *filename, char *ext);
int try_add_file(char *filename, char *exten);
int copy_file(char *src_file_name, char *dest_file_name, char* (*skip_until)(char *), char* (*exchange)(char *));