#include <stdlib.h>
#include <string.h>

/* -- TEST --
#include <stdio.h>
*/

#include "extern_and_entry.h"
#include "../../Global/defines.h"
#include "../../Global/Data/tables.h"

int handle_entry_or_extern(char *str){
    char *str_copy;
    char *inst_type_tok, *label_tok;
    int type;

    str_copy = malloc(strlen(str) +1);
    strcpy(str_copy, str);

    if(strstr(str_copy, ".extern") != NULL || strstr(str_copy, ".entry") != NULL){
        inst_type_tok = strtok(str_copy, " \t\n");

        if(strcmp(inst_type_tok, ".extern") == 0){
            type = EXTERN;
        }
        else if(strcmp(inst_type_tok, ".entry") == 0){
            type = ENTRY;
        }
        else{
            /*print error of illegal instruction*/
            return ERROR;
        }

        label_tok = strtok(NULL, "n");
        if(label_tok == NULL){
            /* print corresponding error */
            return ERROR;
        }

        /* Label validation */
        /* Create instruction object and set values */
        /* Use "add_table" for this instruction */
    }
    return SUCCESS;
}