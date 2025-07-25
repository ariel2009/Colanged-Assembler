#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables.h"
#include "structures.h"
#include "../defines.h"

command_def commands[] = {
    {"mov", 0, 2},
    {"cmp", 1, 2},
    {"add", 2, 2},
    {"sub", 3, 2},
    {"lea", 4, 2},
    {"clr", 5, 1},
    {"not", 6, 1},
    {"inc", 7, 1},
    {"dec", 8, 1},
    {"jmp", 9, 1},
    {"bne", 10, 1},
    {"jsr", 11, 1},
    {"red", 12, 1},
    {"prn", 13, 1},
    {"rts", 14, 0},
    {"stop", 15, 0}
};

char *instructions[] = 
{".data", ".string", ".mat", ".entry", ".extern"};

char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

instruction *entries, *externs;
label *label_table;

int externs_count = 0, entries_count = 0, labels_count = 0;

int isInstruct(char *possibleInst){
    int i;
    for ( i = 0; i < INST_COUNT; i++){
        if(strcmp(possibleInst, instructions[i]) == 0)
            return SUCCESS;
    }
    return ERROR;
}

int isCommand(char *possibleCmd){
    int i;
    for ( i = 0; i < CMD_COUNT; i++){
        if(strcmp(possibleCmd, commands[i].name) == 0)
            return SUCCESS;
    }
    return ERROR;
}

int isRegister(char *possibleReg){
    int i;
    for ( i = 0; i < REG_COUNT; i++){
        if(strcmp(possibleReg, registers[i]) == 0)
            return SUCCESS;
    }
    return ERROR;
}

/*struct hashMap *create_map(){
    struct hashMap *macros;
    macros = (struct hashMap*)malloc(sizeof(struct hashMap));
    return macros;
}*/

int add_extern_or_entry(instruction *inst_data){
    if (inst_data->is_extern)
    {
        externs = (instruction *)realloc(externs, (externs_count + 1)*sizeof(instruction));
        /*Check if NULL*/
        
        memcpy(externs + externs_count++, inst_data, sizeof(instruction));
        free(inst_data);

        return SUCCESS;
    }
    entries = (instruction *)realloc(entries, (entries_count + 1)*sizeof(instruction));
    /*Check if NULL*/

    memcpy(entries + entries_count++, inst_data, sizeof(instruction));
    free(inst_data);
        
    return SUCCESS;
}

int add_label(label *input_label){
    label_table = (label *)realloc(label_table, (labels_count + 1)*sizeof(instruction));
    if(label_table == NULL){
        /* print cannot allocat error */
        return ERROR;
    }

    memcpy(label_table + labels_count++, input_label, sizeof(label));
    free(input_label);
    
    /* TEST */
    printf("from add_label: label name:%s, address: %d", (label_table+labels_count-1)->name, (label_table+labels_count-1)->address);

    return SUCCESS;
}