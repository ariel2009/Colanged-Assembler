#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mcro_expan.h"
#include "../IO/FileHandler.h"
#include "../Global/defines.h"
#include "../Global/util.h"
#include "../Global/Data/tables.h"
#include "../ErrorHandling/Errors.h"

hashMap *macro_list;

int expand_macro(char *src_file_name, char *input_file_name){
    int status = SUCCESS, isInMcro = STATE_OUT, err_code = 0;
    int mcroLinesCount = 0, lineCount = 0;
    char buff[MAX_LINE_LENGTH], *buff_copy;
    char *mcro_name, *token;

    FILE *input, *src;
    fpos_t *mcro_start, *pos_copy;
    location *err_loc;

    input = fopen(input_file_name, "r");
    src = fopen(src_file_name, "r");
    if(input == NULL || src == NULL){
        print_general_err(NULL, ERR_CODE_8);
        return ERROR;
    }
    
    macro_list = (hashMap *)malloc(sizeof(hashMap));
    initializeHashMap(macro_list);
    
    while(fgets(buff, MAX_LINE_LENGTH, input) != NULL){
        /*lineCount += count_spaces_lines(src) + 1; */
        lineCount++;
        buff_copy = (char *)malloc(strlen(buff) + 1);
        strcpy(buff_copy, buff);
    
        token = strtok(buff_copy, "\n");
        if(strcmp(token, MACRO_END_TOK) == 0){
            if(isInMcro){
                if(isExtraText(buff_copy)){
                    err_loc = (location*)malloc(sizeof(location));
                    /*err_loc->file_name = src_file_name;*/
                    err_loc->file_name = input_file_name;
                    err_loc->line = lineCount;
                    print_mcro_err(err_loc, ERR_CODE_12);
                    free(err_loc);
                    status = ERROR;
                }
                else{
                    pos_copy = (fpos_t *)malloc(sizeof(fpos_t));
                    fgetpos(input, pos_copy);
                    save_mcro(mcro_name, mcro_start, input, mcroLinesCount);
                    fsetpos(input, pos_copy);
                    free(pos_copy);
                }
                isInMcro = STATE_OUT;
                mcroLinesCount = 0;
                free(mcro_start);
            }
        }
        else{
            strcpy(buff_copy, buff);
            token = strtok(buff_copy, " \t\n");

            if(strcmp(token, MCRO_DECL_TOK) == 0){
                mcro_name = strtok(NULL, "\n");
                mcro_name = removeExtraSpaces(mcro_name);
                if((err_code = validate_mcro_name(mcro_name)) != -1){
                        err_loc = (location*)malloc(sizeof(location));
                        /*err_loc->file_name = src_file_name;*/
                        err_loc->file_name = input_file_name;
                        err_loc->line = lineCount;
                        print_mcro_err(err_loc, err_code);
                        free(err_loc);
                        status = ERROR;
                }
                else{
                    isInMcro = STATE_IN;
                    mcro_start = (fpos_t *)malloc(sizeof(fpos_t));
                    fgetpos(input, mcro_start);
                }
            }
            else if(isInMcro){
                mcroLinesCount++;
            }
        }
    }
    
    if(status != ERROR){
        status = expan_and_remove_defs(input_file_name);
    }
    fclose(input);
    fclose(src);
    free(macro_list);
    return status;
}

int validate_mcro_name(char *mcro_name){

    if(mcro_name == NULL){
        return ERR_CODE_10;
    }

    strtok(mcro_name, "\n"); /*To remove new line char*/

    if(isCommand(mcro_name) || isInstruct(mcro_name) || isRegister(mcro_name)){
        return ERR_CODE_11;
    }

    if(search(macro_list, mcro_name) != NULL){
        return ERR_CODE_13;
    }
    
    if(isExtraText(mcro_name)){
        return ERR_CODE_16;
    }

    return -1;
}

void save_mcro(char *name, fpos_t *mcro_start, FILE *src, int lineCount){
    char *content;
    int i, lines_passed = 0;
    printf("%s", name);
    content = (char *)malloc(lineCount*MAX_LINE_LENGTH);

    fsetpos(src, mcro_start);
    for (i = 0; lines_passed<lineCount; i++)
    {
        if((*(content + i) = getc(src)) == '\n'){
            lines_passed++;
        }
    }
    *(content+i-1) = '\0'; /*To avoid post-new_line*/
    printf("%s", content);
    insert(macro_list, name, content);
    free(content);
}

char *skip_until_mcroend(char *line){
    char *line_copy = malloc(strlen(line) + 1);
    char *token;

    strcpy(line_copy, line);
    token = strtok(line_copy, " \t");

    if(token == NULL){
        free(line_copy);
        return NULL;
    }

    if(strcmp(token, MCRO_DECL_TOK) == 0){
        free(line_copy);
        return MACRO_END_TOK;
    }

    free(line_copy);
    return NULL;
}

char *exchange_if_mcro_name(char *line){
    char *line_copy = malloc(strlen(line) + 1);
    char *token;
    char *content;
    char *content_copy;

    strcpy(line_copy, line);
    token = strtok(line_copy, "\n");

    if(token == NULL){
        free(line_copy);
        return NULL;
    }

    if((content = search(macro_list, token)) != NULL){
        content_copy = malloc(strlen(content) + 1);
        strcpy(content_copy, content);
        free(line_copy);
        return content_copy;
    }
    
    free(line_copy);
    return NULL;
}

int expan_and_remove_defs(char *src_file_name){
    char *new_file_name = malloc(strlen(src_file_name) + 1);

    strcpy(new_file_name, src_file_name);

    if(!copy_file(src_file_name, "example.am", skip_until_mcroend, exchange_if_mcro_name)){
        return ERROR;
    }

    return SUCCESS;
}