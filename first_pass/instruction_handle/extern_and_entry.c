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
    instruction *inst_to_add;

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

        label_tok = strtok(NULL, "\n");
        if(label_tok == NULL){
            /* print corresponding error */
            return ERROR;
        }

        /* Label validation */

        inst_to_add = (instruction *)malloc(sizeof(instruction));
        if (inst_to_add == NULL)
        {
            /*Print correspondeing error*/
            return ERROR;
        }
        
        inst_to_add->is_extern = type;
        inst_to_add->label = label_tok;
        inst_to_add->nums = NULL;
        inst_to_add->arg_label=NULL;
        inst_to_add->length = 0;
        
        add_to_table(inst_to_add, EXTERN_OR_ENTRY);
    }
    return SUCCESS;
}