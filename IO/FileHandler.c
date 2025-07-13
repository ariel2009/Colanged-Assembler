
#include <string.h>
#include <stdlib.h>

#include "../ErrorHandling/Errors.h"
#include "FileHandler.h"
#include "../Global/defines.h"
#include "../Global/util.h"

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

void copy_file(FILE *src, FILE *dest, char* (*skip_until)(char *), char* (*exchange)(char *)){
    char buff[MAX_LINE_LENGTH];
    char *dest_buff = (char *)malloc(MAX_LINE_LENGTH);
    char *exchanged_str;
    char *end_skip_str;
    int skip_state = STATE_OUT;

    /*int lines_wait_count = 0;*/

    while(fgets(buff, MAX_LINE_LENGTH, src)){
        dest_buff = removeExtraSpaces(buff);
        /*
        if(lines_wait_count = need_write_lines(dest_buff)){
            for (i = 0; i < lines_wait_count && buff; i++){
                fgets(buff, MAX_LINE_LENGTH, src);
            }
        }
        */
        if(skip_state == STATE_OUT){
            if((end_skip_str = skip_until(dest_buff)) != NULL){
                skip_state = STATE_IN;
                free(end_skip_str);
            }
            else if((exchanged_str = exchange(dest_buff)) != NULL){
                fprintf(dest, exchanged_str, "%s");
                free(exchanged_str);
            }
            else
                fprintf(dest, dest_buff, "%s");
        }
        else if(strcmp(dest_buff, end_skip_str) == 0){
            skip_state = STATE_OUT;
        }
    }

    free(dest_buff);
}