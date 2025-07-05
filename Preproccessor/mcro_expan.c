#include <stdio.h>
#include <string.h>

#include "mcro_expan.h"
#include "../IO/FileHandler.h"
#include "../Global/defines.h"
#include "../Global/util.h"
#include "../Global/tables.h"
#include "../ErrorHandling/Errors.h"

int expand_macro(char *fileName){
    int status = SUCCESS;
    FILE *fp;
    char *token;
    int isInMcro = STATE_OUT;
    location *err_loc = {fileName, 0};
    if(!(fp = tryOpenFile(fileName, "as", "r")))
        return ERROR;

    unsigned long lineNum = 0;
    char buff[MAX_LINE_LENGTH];
    while(fgets(buff, sizeof(buff), fp)){
        lineNum++;
        char *no_prepost_spaces_str = removeExtraSpaces(buff);

        if(!isInMcro){
            if(strstr(no_prepost_spaces_str, MCRO_DECL_TOK)){

                /* Validation */
                if(!validate_mcro_decl(no_prepost_spaces_str)){
                    err_loc->line = lineNum;
                    print_mcro_err(err_loc, ERR_CODE_11);
                    status = ERROR;
                }
                else{
                    isInMcro = STATE_IN;
                }
            }
        }
        else{
            if(strstr(no_prepost_spaces_str, MACRO_END_TOK)){
                char *after_end_tok;

                if(isExtraText(no_prepost_spaces_str, MCRO_DECL_TOK, PRE, after_end_tok) || \
                isExtraText(after_end_tok, MCRO_DECL_TOK, POST, NULL)){
                    err_loc->line = lineNum;
                    print_mcro_err(err_loc, ERR_CODE_12);
                    status = ERROR;
                }
                else{

                }
            }
            isInMcro = STATE_OUT;
        }
    }

    fclose(fp);
    return status;
}

int validate_mcro_decl(char *str){
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

    return SUCCESS;
}

int save_mcro(char *name, char *content){
    
}