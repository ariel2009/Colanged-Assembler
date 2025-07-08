
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../ErrorHandling/Errors.h"
#include "FileHandler.h"
#include "../Global/defines.h"

int isFileNameValid(char *fullName, char *ext){

    strtok(*fullName, '.');

    char *ext_from_name = strtok(NULL, '.');
    if(strcmp(*ext_from_name, *ext) != 0 || strcmp(strtok(NULL, '\n'),"\0") != 0){
        print_general_err(NULL, ERR_CODE_2);
        return ERROR;
    }

    FILE *fp;
    if((fp = fopen(*fullName, "w")) == NULL){
        print_general_err(NULL, ERR_CODE_4);
        return ERROR;
    }

    fclose(fp);

    return SUCCESS;
}

FILE *tryOpenFile(char *fileNameToOpen, char *exten, char *mode){
    FILE *fp;
    fp = (FILE *)malloc(sizeof(FILE));
    fp = fopen(fileNameToOpen, mode);
    if(fp == NULL || !isFileNameValid(fileNameToOpen, exten)){
        fclose(fp);
        return NULL;
    }
    return fp;
}