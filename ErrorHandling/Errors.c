#include <stdio.h>
#include <stddef.h>

#include "Errors.h"

Error general_errors[] ={
    {ERR_CODE_1,  "Failed to dynamically allocate memory"},
    {ERR_CODE_2,  ".as files name is missing"},
    {ERR_CODE_3,  "The file name is too long"},
    {ERR_CODE_4,  "The provided file name is not exist"},
    {ERR_CODE_5,  ".am file is missing"},
    {ERR_CODE_6,  "The Line is too long"},
    {ERR_CODE_7,  "Failed to open new file for writing"},
    {ERR_CODE_8,  "Failed to open file for reading"},
    {ERR_CODE_9,  "Setting a position in a file failed"}
};

Error mcro_errors[] ={
    {ERR_CODE_10, "Error - macro without name definition"},
    {ERR_CODE_11, "Illegal name for a macro"},
    {ERR_CODE_12, "mcroend with extra text"},
    {ERR_CODE_13, "Macro has more than one definition"},
    {ERR_CODE_14, "Failed to copy file during macros expansion"},
    {ERR_CODE_15, "Macros expansion in an .as file failed"},
    {ERR_CODE_16, "Error - extra text after macro name definition"}
};

/* Add More Errors */

void print_mcro_err(location *loc, unsigned short err_code){
    if(loc == NULL)
        print_independed_err(err_code, mcro_errors, sizeof(general_errors) / sizeof(Error)+1);
    else
        print_depended_err(*loc, err_code, mcro_errors, sizeof(general_errors) / sizeof(Error)+1);
}

void print_general_err(location *loc, unsigned short err_code){
    if(loc == NULL)
        print_independed_err(err_code, general_errors, 0);
    else
        print_depended_err(*loc, err_code, general_errors, 0);
}

void print_independed_err(int err_code, Error *type, int offset){
    printf("~ERROR~: ID:%d~~ | %s\n", err_code + offset, type[err_code].err_msg);
}

void print_depended_err(location loc, int err_code, Error *type, int offset){
    printf("~ERROR~: ID:%d~~ in %s at line:%lu | there is error: %s\n", err_code + offset, \
    loc.file_name, loc.line, type[err_code].err_msg);
}