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

int expand_macro(char *fileName){
    int status = SUCCESS, isInMcro = STATE_OUT, err_code = 0;
    int mcroLinesCount = 0, lineCount = 0;
    char buff[MAX_LINE_LENGTH], *buff_copy;
    char *mcro_name, *no_prepost_spaces_str, *token;

    FILE *fp;
    fpos_t *mcro_start, *pos_copy;
    location *err_loc;

    macro_list = (hashMap *)malloc(sizeof(hashMap));
    initializeHashMap(macro_list);

    fp = fopen(fileName, "r");
    
    while(fgets(buff, MAX_LINE_LENGTH, fp) != NULL){
        lineCount++;
        buff_copy = (char *)malloc(strlen(buff) + 1);
        strcpy(buff_copy, buff);
        /*printf("Line: %s, count: %d\n", buff, lineCount);*/
        /*no_prepost_spaces_str = removeExtraSpaces(buff);*/
    
        /*if(strcmp(no_prepost_spaces_str, "\n") == 0)
            continue;*/
        /*if(is_tok_in_str(no_prepost_spaces_str, MACRO_END_TOK)){
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
                    save_mcro(mcro_name, mcro_start, fp, mcroLinesCount);
                    fsetpos(fp, pos_copy);
                    free(pos_copy);
                }
                isInMcro = STATE_OUT;
                mcroLinesCount = 0;
                free(mcro_start);
            }
        }*/
        token = strtok(buff_copy, "\n");
        if(strcmp(token, MACRO_END_TOK) == 0){
            if(isInMcro){
                if(isExtraText(buff_copy)){
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
                    save_mcro(mcro_name, mcro_start, fp, mcroLinesCount);
                    fsetpos(fp, pos_copy);
                    free(pos_copy);
                }
                isInMcro = STATE_OUT;
                mcroLinesCount = 0;
                free(mcro_start);
            }
        }
        /*else if(strcmp(getToken(no_prepost_spaces_str, " \t"),\
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
        }*/
        else if(strcmp(token, MCRO_DECL_TOK) == 0){
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
        fclose(fp);
        status = expan_and_remove_defs(fileName);
    }
    free(macro_list);
    return status;
}

int validate_mcro_name(char *mcro_name){

    if(mcro_name == NULL){
        return ERR_CODE_10;
    }

    mcro_name = removeExtraSpaces(mcro_name);
    strtok(mcro_name, "\n"); /*To remove new line char*/

    if(isCommand(mcro_name) || isInstruct(mcro_name) || isRegister(mcro_name)){
        return ERR_CODE_11;
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
    *(content+i) = '\0'; /*To avoid post-new_line*/
    printf("%s", content);
    insert(macro_list, name, content);
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
        strcpy(mcroend_str, MACRO_END_TOK);
        /*free(token);*/
        return mcroend_str;
    }
    return NULL;
}

char *exchange_if_mcro_name(char *line){
    char *token = getToken(line, "\n");
    char *content;
    char *content_copy;
    if(token == NULL)
        return NULL;
    if((content = search(macro_list, token)) != NULL){
        content_copy = malloc(strlen(content) + 1);
        strcpy(content_copy, content);
        /*free(token);*/
        return content_copy;
    }
    /*free(token);*/
    return NULL;
}

int expan_and_remove_defs(char *src_file_name){
    FILE *src, *dest;
    char *new_file_name = malloc(strlen(src_file_name) + 1);
    char *last_dot_pos;

    strcpy(new_file_name, src_file_name);
    /*Need to add support on filenames without extensions*/
    if((last_dot_pos = strrchr(new_file_name, '.')) != NULL)
    {
        last_dot_pos = ".am"; /* So we will get the same file name as source but .am extension */
    }

    src = fopen(src_file_name, "r");
    dest = fopen("example.am", "w");
    if(src == NULL){
        print_general_err(NULL, ERR_CODE_8);
        return ERROR;
    }
    if(dest == NULL){
        print_general_err(NULL, ERR_CODE_7);
    }

    copy_file(src, dest, skip_until_mcroend, exchange_if_mcro_name);

    return SUCCESS;
}