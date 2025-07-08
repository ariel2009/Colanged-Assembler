#include <string.h>
#include <stdlib.h>

#include "mcro_expan.h"
#include "../IO/FileHandler.h"
#include "../Global/defines.h"
#include "../Global/util.h"
#include "../Global/Data/tables.h"
#include "../ErrorHandling/Errors.h"

int expand_macro(char *fileName){
    int status = SUCCESS, isInMcro = STATE_OUT;
    unsigned long mcroLinesCount = 0;
    FILE *fp;
    fpos_t *mcro_start;
    char *mcro_name;
    location *err_loc;
    hashMap *macros;

    macros = (hashMap *)(sizeof(hashMap));
    initializeHashMap(macros);

    if(!(fp = tryOpenFile(fileName, "as", "r")))
        return ERROR;

    unsigned long lineCount = 0;
    char buff[MAX_LINE_LENGTH];

    /* Scanning for macros and saving in the hashMap */
    while(fgets(buff, sizeof(buff), fp)){
        lineCount++;
        char *no_prepost_spaces_str = removeExtraSpaces(buff);

        if(!isInMcro){
            if(strstr(no_prepost_spaces_str, MCRO_DECL_TOK)){

                /* Validation */
                if(!validate_mcro_decl(no_prepost_spaces_str, mcro_name)){
                    err_loc->line = lineCount;
                    print_mcro_err(err_loc, ERR_CODE_11);
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
                char *after_end_tok;

                if(isExtraText(no_prepost_spaces_str, MCRO_DECL_TOK, PRE, after_end_tok) || \
                isExtraText(after_end_tok, MCRO_DECL_TOK, POST, NULL)){
                    err_loc->line = lineCount;
                    print_mcro_err(err_loc, ERR_CODE_12);
                    free(mcro_start);
                    status = ERROR;
                }
                else{
                    save_mcro(mcro_name, mcro_start, fp, mcroLinesCount, &macros);
                }
                isInMcro = STATE_OUT;
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
    char *after_decl_tok;

    if(isExtraText(str, MCRO_DECL_TOK, PRE, after_decl_tok))
        return ERROR;

    after_decl_tok = removeExtraSpaces(after_decl_tok);
    if(strcmp(after_decl_tok, '\n') == 0)
        return ERROR;
    if(isCommand(str) || isInstruct(str) || isRegister(str))
        return ERROR;
    
    if(strchr('\t', str) || strchr(' ', str))
        return ERROR;
    strcpy(out_mcro_name, str);

    return SUCCESS;
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