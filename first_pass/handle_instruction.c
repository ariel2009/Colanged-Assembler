#include <string.h>
#include <stdlib.h>

#include "handle_instruction.h"
#include "../Global/defines.h"
#include "../Global/Data/tables.h"
#include "../Global/util.h"

int read_entry_or_extern(char *str){
    char *str_copy;
    char *token, *label_tok;
    int type;
    instruction *inst_to_add;

    str_copy = malloc(strlen(str) +1);
    strcpy(str_copy, str);

    token = strtok(str_copy, ": \n");

    if(token != NULL && *(token) != '.' && legal_label_or_mcro(token)){ /* So this is a label */
        /* Print warning */
        token = strtok(NULL, " \n");
    }

    if(token == NULL){
        /* print error of illegal .entry or .extern instruction */
        return ERROR;
    }
    
    if(strcmp(token, ".extern") == 0){
        type = EXTERN;
    }
    else if(strcmp(token, ".entry") == 0){
        type = ENTRY;
    }
    else{
        /* print error of illeagl .entry or .extern instruction*/
    }

    label_tok = strtok(NULL, " \n");
    if(label_tok == NULL){
        /* print corresponding error */
        return ERROR;
    }

    /* Label validation(such Length, Syntax) and check for extra text */
    /* Check if label already defined as entry/extern */

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
    
    add_extern_or_entry(inst_to_add);

    return SUCCESS;
}

int read_str_data(char *str, int *DC){
    char *str_copy, *token;
    label *data_label;

    str_copy = malloc(strlen(str) + 1);
    strcpy(str_copy, str);

    token = strtok(str_copy, " \n");

    token = strtok(NULL, ": \n");
    if(legal_label_or_mcro(token)){
        data_label = (label *)malloc(sizeof(label));
        data_label->name = token;
        data_label->type = DATA_SEG;
        data_label->address = (*DC)++;

        add_label(data_label);
        token = strtok(NULL, " \n");
    }
    return SUCCESS;
}