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
        if(!read_nums(inst, token)){
            return ERROR;
        }
    }

    return SUCCESS;
}

int read_nums(instruction **data, char *str){
    char *str_copy, *token;
    int i = 0;

    
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

    if(strstr(str_copy, ",")){
        token = strtok(str_copy, " ,");
        if(token == NULL){
            /* error comma before number */
            return ERROR;
        }
        /* TEST */
        printf("from read_nums: token is: %s\n", token);
        
        while(token != NULL && strchr(str + (token-str_copy), ',') != NULL){
            /* TEST */
            printf("from read_nums: number: %s\n", token);

            token = strtok(NULL, ", ");
            /*if(!is_valid_num(token)){
                return ERROR;
            }

            (*data)->nums[i] = atoi(token);*/
            i++;
        }
        token = strtok(NULL, " \n");
        if(token == NULL){
            /* error comma found after last number */
            return ERROR;
        }
    }
    return SUCCESS;
}