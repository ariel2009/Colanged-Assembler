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
    /* not accurate, for example if a colon appears in a string of .string inst */
    if(strchr(str_copy, ':') != NULL){
        token = strtok(str_copy, ":");
        if(!legal_label_or_mcro(token)){
            printf("from read_str_data: illegal label");
            return ERROR;
        }

        data_label = (label *)malloc(sizeof(label));
        data_label->name = token;
        data_label->type = DATA_SEG;
        data_label->address = *DC;

        inst->label = token;

        add_label(data_label);
        token = strtok(NULL, " \n");
    }
    else
        token = strtok(str_copy, " \n");


    if(strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0){
        if(strcmp(token, ".data") == 0){
            token = strtok(NULL, "\n");
            if(!read_nums(&inst, token)){
                return ERROR;
            }
        }
        else{
            token = strtok(NULL, "\n");
            read_str(inst, token);
        }
    }
    else {
        /* Illegatl instruction */
        return ERROR;
    }

    return SUCCESS;
}

int read_str_data_copy(char *str, int *DC){
    char *str_copy, *token, *inst_start, *label_name, *after_label, *data;
    label *data_label;
    instruction *inst;

    inst = (instruction *)malloc(sizeof(instruction));
    if(inst == NULL){
        /* error cannot malloc */
        return ERROR;
    }

    str_copy = malloc(strlen(str) + 1);
    strcpy(str_copy, str);

    if((inst_start = strstr(str_copy, ".data")) != NULL || (inst_start = strstr(str_copy, ".string")) != NULL){
        token = strtok(inst_start, " ");
        if(strcmp(token, ".data") != 0 && strcmp(token, ".string") != 0){
            /* TEST */
            printf("from read_str_data_copy: illegal instruction\n");
            return ERROR;
        }
        data = strtok(NULL, "\n");
        if(str_copy != inst_start){ /* There is a potential label before */
            label_name = strtok(str_copy, ":");
            if(strlen(label_name) > (inst_start - str) || !legal_label_or_mcro(label_name)){ /* No semicolon or illegal label */
                
                /* TEST */
                printf("from read_str_data_copy: illegal label decl");

                return ERROR;
            }

            token = strtok(NULL, "\n");
            after_label = malloc(inst_start - token + 1);

            /* To look for Extra text after label decl by substring */
            strncpy(after_label, token, (inst_start - token));
            remove_extra_spaces(after_label);
            if(*(after_label) != '\0'){

                /* TEST */
                printf("from read_str_data_copy: extra text after label");
                return ERROR;
            }
            
            data_label = (label *)malloc(sizeof(label));
            data_label->name = label_name;
            data_label->type = DATA_SEG;
            data_label->address = *DC;

            inst->label = label_name;
            
            add_label(data_label);
        }

        if(strcmp(inst_start, ".data") == 0){
            if(!read_nums(&inst, data)){
                return ERROR;
            }
        }
    }
    return SUCCESS;
}

/* Re-Check edge cases */
/* Syntax Issue: Connot declare this func in the header file */
int read_nums(instruction **data, char *str){
    char *str_copy, *token;
    int i = 0, status = SUCCESS, err_code;
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

    dest_nums = malloc(sizeof(short));
    token = strtok(str_copy, ",");
    while(token != NULL){
        str_copy = strtok(NULL, "");
        remove_extra_spaces(token);
        if(isExtraText(token)){
            /* TEST */
            printf("from read_nums: extra text found");
            status = ERROR;
        }
        if(!is_valid_num(token)){
            status = ERROR;
        }
        dest_nums = realloc(dest_nums, (i+1)*sizeof(short));
        (*data)->nums = dest_nums;
        *(dest_nums + i) = atoi(token);
        i++;
        token = strtok(str_copy, ",");
        /* TEST */
    }
    if(count_chr_occur(str, ',') > i-1){
        /* TEST */
        printf("from read_nums: error! extra comma\n");
        return ERROR;
    }
    return status;
}

int read_str(instruction **inst, char *str){
    char *str_copy, *start;

    /* Validate correct quotes */
    if(str == NULL){
        /* string cannot be null */
        return ERROR;
    }

    str_copy = malloc(strlen(str) + 1);
    strcpy(str_copy, str);
    return SUCCESS;
}