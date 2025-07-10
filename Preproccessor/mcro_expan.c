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
        if(strstr(no_prepost_spaces_str, MACRO_END_TOK) != NULL){
            if(isInMcro){
                printf("%s", no_prepost_spaces_str);
                if(isExtraText(no_prepost_spaces_str, MACRO_END_TOK)){
                    err_loc = (location*)malloc(sizeof(location));
                    err_loc->file_name = fileName;
                    err_loc->line = lineCount;
                    print_mcro_err(err_loc, ERR_CODE_12);
                    free(err_loc);
                    free(mcro_start);
                    free(mcro_name);
                    status = ERROR;
                }
                else{
                    printf("naem: %s, lines: %d", mcro_name, mcroLinesCount);
                    save_mcro(mcro_name, mcro_start, fp, mcroLinesCount, &macros);
                    free(mcro_name);
                }
                isInMcro = STATE_OUT;
            }
        }
        else if(strstr(no_prepost_spaces_str, MCRO_DECL_TOK) != NULL){
                
            mcro_name = malloc(MAX_LINE_LENGTH - sizeof(MCRO_DECL_TOK));
            if(!validate_mcro_decl(no_prepost_spaces_str, mcro_name)){
                printf("%s", mcro_name);
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
        else if(isInMcro){
            mcroLinesCount++;
        }
    }
    fclose(fp);
    return status;
}

int validate_mcro_decl(char *str, char *mcro_name){
    char *str_copy = malloc(sizeof(str));

    strcpy(str_copy, str);
    strtok(str_copy, " \t");
    if(isExtraText(str_copy, MCRO_DECL_TOK)){
        free(str_copy);
        return ERROR;
    }

    strcpy(mcro_name, strtok(NULL, "\n"));
    free(str_copy);
    if(mcro_name == NULL){
        return ERROR;
    }

    if(isCommand(mcro_name) || isInstruct(mcro_name) || isRegister(mcro_name)){
        return ERROR;
    }

    if(strchr(mcro_name, '\t') != NULL || strchr(mcro_name, ' ') != NULL){
        return ERROR;
    }
    return SUCCESS;
}

void save_mcro(char *name, fpos_t *mcro_start, FILE *src, int lineCount, hashMap **macro_list){
    char *next_c, *content;
    int i;

    content = malloc(lineCount*MAX_LINE_LENGTH);
    next_c = content;

    fsetpos(src, mcro_start);
    /*Loop missing some characters, must handle it*/
    for (i = 0; i < sizeof(content); i++)
    {
        *(next_c + i) = getc(src);
        printf("%c", *(next_c + i));
    }
    /*Not working - memory issue
    insert(*macro_list, name, content);
    */
}