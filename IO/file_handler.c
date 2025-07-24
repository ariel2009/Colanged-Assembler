#include <string.h>
#include <stdlib.h>

#include "../ErrorHandling/Errors.h"
#include "file_handler.h"
#include "../Global/defines.h"
#include "../Global/util.h"

int is_extension_valid(char *fullName, char *allowed_ext){

    char *last_dot_pos = strrchr(fullName, '.');

    if(last_dot_pos == NULL ||  \
            strcmp(strtok(last_dot_pos, "\0"), allowed_ext) != 0){
        print_general_err(NULL, ERR_CODE_2);
        return ERROR;
    }

    return SUCCESS;
}

int copy_file(char *src_file_name, char *dest_file_name, char* (*skip_until)(char *), char* (*exchange)(char *)){
    FILE *src, *dest;
    char buff[MAX_LINE_LENGTH], *buff_copy;
    char *exchanged_str, *end_skip_str, *token;
    int skip_state = STATE_OUT, in_content = STATE_OUT;
    int is_skip = skip_until != NULL, is_exchanged = exchange != NULL;

    src = fopen(src_file_name, "r");
    dest = fopen(dest_file_name, "w");

    if(src == NULL){
        print_general_err(NULL, ERR_CODE_8);
        return ERROR;
    }
    if(dest == NULL){
        print_general_err(NULL, ERR_CODE_7);
        return ERROR;
    }

    while(fgets(buff, MAX_LINE_LENGTH, src)){
        buff_copy = remove_extra_spaces_copy(buff);  
        if(buff_copy != NULL && strcmp(buff_copy, "\0") != 0){
            if(skip_state == STATE_OUT){
                if(is_skip && (end_skip_str = skip_until(buff_copy)) != NULL){
                    skip_state = STATE_IN;
                }
                else {
                    if(in_content){
                        putc('\n', dest);
                    }
                    if(is_exchanged && (exchanged_str = exchange(buff_copy)) != NULL){
                        fprintf(dest, exchanged_str, "%s");
                    }
                    else{
                        fprintf(dest, buff_copy, "%s");
                    }
                    in_content = STATE_IN;
                }
            }
            else if(end_skip_str != NULL){
                token = strtok(buff_copy, "\n");
                if(strcmp(token, end_skip_str) == 0){
                    skip_state = STATE_OUT;
                }
            }
        }
    }
    fclose(src);
    fclose(dest);
    return SUCCESS;
}

int try_add_file(char *filename, char *exten){
    FILE *fp;
    char *name_copy;
    char *token;

    name_copy = malloc(strlen(filename) +1);
    strcpy(name_copy, filename);

    if(strchr(name_copy, '.') != NULL){
        strtok(name_copy, ".");;
        token = strtok(NULL, " \n");
        if(token == NULL || strcmp(token, exten) != 0){
            free(name_copy);
            print_general_err(NULL, ERR_CODE_2);
            return ERROR;
        }
    }
    else{
        strcat(filename, ".");
        strcat(filename, exten);
    }

    fp = fopen(filename, "r");
    if(fp == NULL){
        print_general_err(NULL, ERR_CODE_4);
        return ERROR;
    }

    free(name_copy);
    fclose(fp);
    return SUCCESS;
}