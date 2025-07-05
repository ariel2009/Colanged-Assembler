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
    location *err_loc = {fileName, 0};
    if(!(fp = tryOpenFile(fileName, "as", "r")))
        return ERROR;

    unsigned long lineNum = 0;
    char buff[MAX_LINE_LENGTH];
    while(fgets(buff, sizeof(buff), fp)){
        lineNum++;
        char *no_prepost_spaces_str = removeExtraSpaces(buff);

        if(strstr(no_prepost_spaces_str, MCRO_DECL_TOK)){
            no_prepost_spaces_str += sizeof(MCRO_DECL_TOK);
            no_prepost_spaces_str =  removeExtraSpaces(no_prepost_spaces_str);

            if(!validate_mcro_decl(no_prepost_spaces_str)){
                err_loc->line = lineNum;
                print_mcro_err(err_loc, ERR_CODE_11);
                status = ERROR;
            }
        }
    }

    fclose(fp);
    return status;
}

int validate_mcro_decl(char *after_mcro_tok){
    if(strcmp(after_mcro_tok, '\n') == 0)
        return ERROR;
    if(isCommand(after_mcro_tok) || isInstruct(after_mcro_tok) || isRegister(after_mcro_tok))
        return ERROR;
    if(strchr('\t', after_mcro_tok) || strchr(' ', after_mcro_tok))
        return ERROR;
    return SUCCESS;
}