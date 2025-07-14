#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mcro_expan.h"
#include "../IO/FileHandler.h"
#include "../Global/defines.h"
#include "../Global/util.h"
#include "../Global/Data/tables.h"
#include "../ErrorHandling/Errors.h"

int expand_macro(char *fileName){
    int status = SUCCESS, isInMcro = STATE_OUT, err_code = 0;
    int mcroLinesCount = 0, lineCount = 0;
    char buff[MAX_LINE_LENGTH];
    char *mcro_name, *no_prepost_spaces_str;

    FILE *fp;
    fpos_t *mcro_start, *pos_copy;
    location *err_loc;
    hashMap *macros;

    macros = (hashMap *)malloc(sizeof(hashMap));
    initializeHashMap(macros);

    if(!(fp = tryOpenFile(fileName, ".as", "r"))){
        return ERROR;
    }
    
    while(fgets(buff, MAX_LINE_LENGTH, fp) != NULL){
        lineCount++;
        /*printf("Line: %s, count: %d\n", buff, lineCount);*/
        no_prepost_spaces_str = removeExtraSpaces(buff);
    
        if(strcmp(no_prepost_spaces_str, "\n") == 0)
            continue;
        if(is_tok_in_str(no_prepost_spaces_str, MACRO_END_TOK)){
            if(isInMcro){
                if(isExtraText(no_prepost_spaces_str)){
                    err_loc = (location*)malloc(sizeof(location));
                    err_loc->file_name = fileName;
                    err_loc->line = lineCount;
                    print_mcro_err(err_loc, ERR_CODE_12);
                    free(err_loc);
                    status = ERROR;
                }
                else{
                    pos_copy = (fpos_t *)malloc(sizeof(fpos_t));
                    fgetpos(fp, pos_copy);
                    save_mcro(mcro_name, mcro_start, fp, mcroLinesCount, &macros);
                    fsetpos(fp, pos_copy);
                    free(pos_copy);
                }
                isInMcro = STATE_OUT;
                mcroLinesCount = 0;
                free(mcro_start);
            }
        }
        else if(strcmp(getToken(no_prepost_spaces_str, " \t"),\
         MCRO_DECL_TOK) == 0){
            mcro_name = strtok(NULL, "\n");
            if((err_code = validate_mcro_name(mcro_name)) != -1){
                    err_loc = (location*)malloc(sizeof(location));
                    err_loc->file_name = fileName;
                    err_loc->line = lineCount;
                    print_mcro_err(err_loc, err_code);
                    free(err_loc);
                    status = ERROR;
            }
            else{
                isInMcro = STATE_IN;
                mcro_start = (fpos_t *)malloc(sizeof(fpos_t));
                fgetpos(fp, mcro_start);
            }
        }
        else if(isInMcro){
            mcroLinesCount++;
        }
    }

    if(status != ERROR){
        /*
            Need to copy the file to expanded one
        */
    }
    free(macros);
    fclose(fp);
    return status;
}

int validate_mcro_name(char *mcro_name){

    if(mcro_name == NULL){
        return ERR_CODE_10;
    }

    mcro_name = removeExtraSpaces(mcro_name);
    if(isCommand(mcro_name) || isInstruct(mcro_name) || isRegister(mcro_name)){
        return ERR_CODE_11;
    }

    if(isExtraText(mcro_name)){
        return ERR_CODE_16;
    }

    return -1;
}

void save_mcro(char *name, fpos_t *mcro_start, FILE *src, int lineCount, hashMap **macro_list){
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
    insert(*macro_list, name, content);
    free(content);
}

char *skip_until_mcroend(char *line){
    char *token = getToken(line, " \t");
    char *mcroend_str;
    if(token == NULL){
        return NULL;
    }
    if(strcmp(token, MCRO_DECL_TOK) == 0){
        mcroend_str = (char *)malloc(strlen(MCRO_DECL_TOK) + 1);
        strcpy(mcroend_str, MCRO_DECL_TOK);
        free(token);
        return mcroend_str;
    }
}

char *exchange_if_mcro_name(char *line, hashMap *macro_list){
    char *token = getToken(line, "\n");
    char *content;
    char *content_copy;
    if(token == NULL)
        return NULL;
    if((content = search(macro_list, token)) != NULL){
        content_copy = malloc(strlen(content) + 1);
        strcpy(content_copy, content);
        free(token);
        return content_copy;
    }
    free(token);
    return NULL;
}

int expan_and_remove_defs(hashMap *mcro_list, FILE *src, char *src_file_name){
    char *new_file_name = malloc(strlen(src_file_name) + 1);
    char *last_dot_pos;

    strcpy(new_file_name, src_file_name);
    last_dot_pos = strrchr(new_file_name, '.');
    last_dot_pos = '.am'; // So we will get the same file name as source but .am extension
    
    FILE *dest = fopen(new_file_name, "w");
    if(dest == NULL){
        print_general_err(NULL, ERR_CODE_7);
        return ERROR;
    }

    copy_file(src, dest, skip_until_mcroend, exchange_if_mcro_name);
    return SUCCESS;
}