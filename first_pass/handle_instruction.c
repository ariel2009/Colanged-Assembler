#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

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

    if(strchr(str_copy, ':') != NULL){
        token = strtok(str_copy, ":");
        if(!legal_label_or_mcro(token))
            return ERROR;
        
        token = strtok(NULL, " \n");
    }
    else
        token = strtok(str_copy, " \n");

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

    label_tok = strtok(NULL, "\n");
    if(label_tok == NULL){
        /* print corresponding error */
        return ERROR;
    }
    if(!legal_label_or_mcro(label_tok)){
        return ERROR;
    }

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

/* Edge Cases */
int read_str_data(char *str, int *DC){
    char *str_copy, *token;
    label *data_label;
    instruction *inst;

    inst = (instruction *)malloc(sizeof(instruction));
    if(inst == NULL){
        /* error cannot malloc */
        return ERROR;
    }

    str_copy = malloc(strlen(str) + 1);
    strcpy(str_copy, str);

    if(strchr(str_copy, ':') != NULL){
        token = strtok(str_copy, ":");
        if(!legal_label_or_mcro(token))
            return ERROR;

        data_label = (label *)malloc(sizeof(label));
        data_label->name = token;
        data_label->type = DATA_SEG;
        data_label->address = *DC;

        inst->label = token;

        add_label(data_label);
    }

    token = strtok(NULL, " \n");

    /* TEST */
    printf("from  read_str_data: instruction is: %s\n", token);

    if(strcmp(token, ".data") == 0){
        token = strtok(NULL, "\n");
        if(!read_nums(&inst, token)){
            return ERROR;
        }
    }

    return SUCCESS;
}

/* Re-Check edge cases */
/* Syntax Issue: Connot declare this func in the header file */
int read_nums(instruction **data, char *str){
    char *str_copy, *token;
    int i = 0;
    short *dest_nums;
    
    if(str == NULL){
        /* No nums provided error */
        return ERROR;
    }
    str_copy = malloc(strlen(str) + 1);
    strcpy(str_copy, str);
    if(str_copy == NULL){
        /* print error memory cannot be allocated */
        return ERROR;
    }
    /* validate there is no pre-comma */

    if(strchr(str_copy, ',') != NULL){
        token = strtok(str_copy, ",");
        if(token == NULL){
            /* error comma before number */
            return ERROR;
        }
        dest_nums = malloc(sizeof(short));

        while(token != NULL){

            if(!is_valid_num(token)){
                return ERROR;
            }
            dest_nums = realloc(dest_nums, (i+1)*sizeof(short));
            (*data)->nums = dest_nums;
            *(dest_nums + i) = atoi(token);
            i++;
            token = strtok(NULL, ", \n");

            /* TEST */
            printf("from read_nums: nums + i: %d\n", (*data)->nums[i-1]);
        }
    }
    /* validate ther is no post-comma */
    return SUCCESS;
}