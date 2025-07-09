
#include <string.h>
#include <stdlib.h>

#include "../ErrorHandling/Errors.h"
#include "FileHandler.h"
#include "../Global/defines.h"

int isFileNameValid(char *fullName, char *allowed_ext){

    char *last_dot_pos = strrchr(fullName, '.');

    if(last_dot_pos == NULL ||  \
            strcmp(strtok(last_dot_pos, "\0"), allowed_ext) != 0){
        print_general_err(NULL, ERR_CODE_2);
        return ERROR;
    }

    return SUCCESS;
}

FILE *tryOpenFile(char *fileNameToOpen, char *exten, char *mode){
    FILE *fp;
    fp = fopen(fileNameToOpen, mode);
    if(!isFileNameValid(fileNameToOpen, exten)){
        return NULL;
    }
    if(fp == NULL){
        print_general_err(NULL, ERR_CODE_4);
        return NULL;
    }
    return fp;
}