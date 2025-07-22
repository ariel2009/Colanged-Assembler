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

instruction entries[], externs[];

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

/* Go ahead from here */
int add_to_table(instruction *inst_data, int type){
    switch (type)
    {
    case  EXTERN_OR_ENTRY:
        if (inst_data->is_extern)
        {
            /* Add to externs table */
            return SUCCESS;
        }
        /* Add to entries table */
        return SUCCESS;
        break;
    
    default:
        return SUCCESS;
        break;
    }

    return SUCCESS;
}