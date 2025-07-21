
#include <string.h>
#include <stdlib.h>

#include "../ErrorHandling/Errors.h"
#include "FileHandler.h"
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
    char buff[MAX_LINE_LENGTH];
    char *dest_buff;
    char *exchanged_str, *end_skip_str, *token;
    int skip_state = STATE_OUT, in_regular_content = STATE_OUT;
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
        if(skip_state == STATE_OUT){
            if(is_skip && (end_skip_str = skip_until(buff)) != NULL){
                skip_state = STATE_IN;
            }
            else {
                if(is_exchanged && (exchanged_str = exchange(buff)) != NULL){
                    if(in_regular_content == STATE_IN){
                        putc('\n', dest);
                    }
                    fprintf(dest, exchanged_str, "%s");
                }
                else{
                    dest_buff = removeExtraSpaces(buff);
                    if(dest_buff != NULL){
                        if(in_regular_content == STATE_IN){
                            putc('\n', dest);
                        }
                        fprintf(dest, dest_buff, "%s");
                    }
                }
                in_regular_content = STATE_IN;
            }
        }
        else if(end_skip_str != NULL){
                token = strtok(buff, "\n");
                if(strcmp(token, end_skip_str) == 0){
                    in_regular_content = STATE_OUT;
                    skip_state = STATE_OUT;
                }
        }
    }
    fclose(src);
    fclose(dest);
    return SUCCESS;
}

int prepare_no_extra_spaces_file(char *filename, char *ext){

    /*if(is_extension_valid(filename, ext) &&\
        copy_file(filename, TMP_FILE_NAME, NULL, removeExtraSpaces)){
            return SUCCESS;
    }*/
    if(!copy_file(filename, TMP_FILE_NAME, NULL, removeExtraSpaces))
        return ERROR; 
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
/*int count_spaces_lines(FILE *file_with_spaces){
    char *buff_of_src;
    int space_line = STATE_IN, spaces_count = 0;
    buff_of_src = malloc(MAX_LINE_LENGTH);

    while(fgets(buff_of_src, MAX_LINE_LENGTH, file_with_spaces) && space_line){
        if(removeExtraSpaces(buff_of_src) != NULL){
            space_line = STATE_OUT;
        }
        spaces_count++;
    }
    return spaces_count;
    free(buff_of_src);
}*/