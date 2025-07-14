
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
    int skip_state = STATE_OUT, in_regular_content = STATE_OUT;
    int is_skip = skip_until != NULL, is_exchanged = exchange != NULL;

    /*int lines_wait_count = 0;*/

    while(fgets(buff, MAX_LINE_LENGTH, src)){

        strcpy(dest_buff, buff);
        
        if(*dest_buff != '\n'){ /*skip blank lines*/
            if(skip_state == STATE_OUT){
                if(is_skip && (end_skip_str = skip_until(dest_buff)) != NULL){
                    skip_state = STATE_IN;
                }
                else {
                    if(in_regular_content == STATE_IN){
                        putc('\n', dest);
                    }

                    if(is_exchanged &&(exchanged_str = exchange(dest_buff)) != NULL){
                        fprintf(dest, exchanged_str, "%s");
                    }
                    else{

                        fprintf(dest, dest_buff, "%s");
                    }

                    in_regular_content = STATE_IN;
                }
            }
            else if(strcmp(dest_buff, end_skip_str) == 0){
                in_regular_content = STATE_OUT;
                skip_state = STATE_OUT;
            }
        }
    }
    fclose(src);
    fclose(dest);
}

int prepare_no_extra_spaces_file(char *filename, char *ext){
    FILE *src, *tmp;
    src = tryOpenFile(filename, ext, "r");
    tmp = fopen(TMP_FILE_NAME, "w");

    if(src == NULL){
        print_general_err(NULL, ERR_CODE_8);
        return ERROR;
    }
    if(tmp == NULL){
        print_general_err(NULL, ERR_CODE_7);
        return ERROR;
    }

    copy_file(src, tmp, removeExtraSpaces, NULL);
    fclose(src);
    fclose(tmp);

    return SUCCESS;
}