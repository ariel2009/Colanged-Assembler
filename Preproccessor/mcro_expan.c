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
    int status = SUCCESS, isInMcro = STATE_OUT;
    int mcroLinesCount = 0, lineCount = 0;
    char buff[MAX_LINE_LENGTH];
    char *mcro_name, *no_prepost_spaces_str;

    FILE *fp;
    fpos_t *mcro_start;
    location *err_loc;
    hashMap *macros;

    macros = (hashMap *)malloc(sizeof(hashMap));
    initializeHashMap(macros);

    if(!(fp = tryOpenFile(fileName, ".as", "r"))){
        return ERROR;
    }
    
    while(fgets(buff, MAX_LINE_LENGTH, fp) != NULL){
        lineCount++;
        no_prepost_spaces_str = removeExtraSpaces(buff);
        if(strcmp(no_prepost_spaces_str, "\n") == 0)
            continue;
        if(!isInMcro){
            if(strstr(no_prepost_spaces_str, MCRO_DECL_TOK) != NULL){
                
                mcro_name = malloc(MAX_LINE_LENGTH - strlen(MCRO_DECL_TOK));
                if(!validate_mcro_decl(no_prepost_spaces_str, mcro_name) == ERROR){
                    err_loc = (location*)malloc(sizeof(location));
                    err_loc->file_name = fileName;
                    err_loc->line = lineCount;
                    print_mcro_err(err_loc, ERR_CODE_11);
                    free(err_loc);
                    free(mcro_name);
                    status = ERROR;
                }
                else{
                    isInMcro = STATE_IN;

                    mcro_start = (fpos_t *)malloc(sizeof(fpos_t));
                    fgetpos(fp, mcro_start);
                }
            }
        }
        else{
            if(strstr(no_prepost_spaces_str, MACRO_END_TOK)){
                char *after_end_tok = malloc(MAX_LINE_LENGTH - strlen(MACRO_END_TOK));

                if(isExtraText(no_prepost_spaces_str, MCRO_DECL_TOK, PRE, after_end_tok) || \
                isExtraText(after_end_tok, MCRO_DECL_TOK, POST, NULL)){
                    err_loc = (location*)malloc(sizeof(location));
                    err_loc->file_name = fileName;
                    err_loc->line = lineCount;
                    print_mcro_err(err_loc, ERR_CODE_12);
                    free(err_loc);
                    free(mcro_start);
                    status = ERROR;
                }
                else{
                    printf("%s", mcro_name);
                    save_mcro(mcro_name, mcro_start, fp, mcroLinesCount, &macros);
                    free(mcro_name);
                }
                isInMcro = STATE_OUT;
                free(after_end_tok);
            }
            else{
                mcroLinesCount++;
            }
        }
    }
    fclose(fp);
    return status;
}

int validate_mcro_decl(char *str, char *out_mcro_name){
    char *mcro_name = malloc(MAX_LINE_LENGTH - sizeof(MCRO_DECL_TOK));
    int is_valid = 1;

    is_valid = !isExtraText(str, MCRO_DECL_TOK, PRE, mcro_name);
    is_valid = is_valid && strcmp(mcro_name, "\n") == 0;
    is_valid = is_valid && (isCommand(str) || isInstruct(str) || isRegister(str));
    is_valid = is_valid && (strchr(str, '\t') != NULL ||\
         strchr(str, ' ') != NULL);
    
    strcpy(out_mcro_name, mcro_name);
    free(mcro_name);

    return is_valid;
}

void save_mcro(char *name, fpos_t *mcro_start, FILE *src, unsigned long lineCount, hashMap **macro_list){
    char *next_c, *content;
    unsigned long i;

    content = (char *)malloc((lineCount-1)*MAX_LINE_LENGTH);
    next_c = content;

    fsetpos(src, mcro_start);
    for (i = 0; i < sizeof(content); i++)
    {
        *(next_c + i) = getc(src);
    }
    
    insert(*macro_list, name, content);
}