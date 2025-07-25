#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mcro_expan.h"
#include "../IO/file_handler.h"
#include "../Global/defines.h"
#include "../Global/util.h"
#include "../Global/Data/tables.h"
#include "../ErrorHandling/Errors.h"

hashMap *macro_list;

char *expand_macro(char *src_file_name){
    int status = SUCCESS, isInMcro = STATE_OUT, err_code = 0;
    int mcroLinesCount = 0, lineCount = 0;
    char buff[MAX_LINE_LENGTH], *buff_copy;
    char *mcro_name, *token;
    char *expanded_file_name;

    FILE *src;
    fpos_t *mcro_start, *pos_copy;
    location *err_loc;

    src = fopen(src_file_name, "r");
    if(src == NULL){
        print_general_err(NULL, ERR_CODE_8);
        return NULL;
    }
    
    macro_list = (hashMap *)malloc(sizeof(hashMap));
    initializeHashMap(macro_list);
    
    while(fgets(buff, MAX_LINE_LENGTH, src) != NULL){
        /*lineCount += count_spaces_lines(src) + 1; */
        lineCount++;
        buff_copy = remove_extra_spaces_copy(buff);

        if(buff_copy != NULL && strcmp(buff_copy, "\0") != 0){
            token = strtok(buff_copy, " \t\n");
            if(strcmp(token, MACRO_END_TOK) == 0){
                if(isInMcro){
                    if(isExtraText(buff)){
                        err_loc = (location*)malloc(sizeof(location));
                        err_loc->file_name = src_file_name;
                        err_loc->line = lineCount;
                        print_mcro_err(err_loc, ERR_CODE_12);
                        free(err_loc);
                        status = ERROR;
                    }
                    else{
                        pos_copy = (fpos_t *)malloc(sizeof(fpos_t));
                        fgetpos(src, pos_copy);
                        save_mcro(mcro_name, mcro_start, src, mcroLinesCount);
                        fsetpos(src, pos_copy);
                        free(pos_copy);
                    }
                    isInMcro = STATE_OUT;
                    mcroLinesCount = 0;
                    free(mcro_start);
                }
            }
            else{
                buff_copy = remove_extra_spaces_copy(buff);
                token = strtok(buff_copy, " \t\n");

                if(strcmp(token, MCRO_DECL_TOK) == 0){
                    mcro_name = strtok(NULL, "\n");
                    if((err_code = validate_mcro_name(mcro_name)) != -1){
                            err_loc = (location*)malloc(sizeof(location));
                            err_loc->file_name = src_file_name;
                            err_loc->line = lineCount;
                            print_mcro_err(err_loc, err_code);
                            free(err_loc);
                            status = ERROR;
                    }
                    else{
                        isInMcro = STATE_IN;
                        mcro_start = (fpos_t *)malloc(sizeof(fpos_t));
                        fgetpos(src, mcro_start);
                    }
                }
                else if(isInMcro){
                    mcroLinesCount++;
                }
            }
        }
    }
    
    if(status == SUCCESS){
        if((expanded_file_name =expan_and_remove_defs(src_file_name)) == NULL)
            return NULL;
        return expanded_file_name;
    }
    fclose(src);
    free(macro_list);

    return NULL;
}

int validate_mcro_name(char *mcro_name){

    if(mcro_name == NULL){
        return ERR_CODE_10;
    }

    if(isCommand(mcro_name) || isInstruct(mcro_name) || isRegister(mcro_name)){
        return ERR_CODE_11;
    }

    /* Validate Like Label */

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
    content = (char *)malloc(lineCount*MAX_LINE_LENGTH);

    fsetpos(src, mcro_start);
    for (i = 0; lines_passed<lineCount; i++)
    {
        if((*(content + i) = getc(src)) == '\n'){
            lines_passed++;
        }
    }
    *(content+i-1) = '\0'; /*To avoid post-new_line*/
    insert(macro_list, name, content);
    free(content);
}

char *skip_until_mcroend(char *line){
    char *line_copy = malloc(strlen(line) + 1);
    char *token;

    strcpy(line_copy, line);
    token = strtok(line_copy, " ");

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

char *expan_and_remove_defs(char *src_file_name){
    char *new_file_name;

    new_file_name = malloc(strlen(src_file_name) + 1);
    strcpy(new_file_name, src_file_name);

    strtok(new_file_name, ".");
    strcat(new_file_name, ".am");

    if(!copy_file(src_file_name, new_file_name, skip_until_mcroend, exchange_if_mcro_name)){
        return NULL;
    }

    return new_file_name;
}