
#include <stdio.h>
#include <string.h>
#include "../ErrorHandling/Errors.h"

#include "../Global/defines.h"

int isFileNameValid(char *fullName, char *ext){

    strtok(*fullName, '.');

    char *ext_from_name = strtok(NULL, '.');
    if(strcmp(*ext_from_name, *ext) != 0 || strtok(NULL, '\n') != '\0'){
        print_general_err(NULL, ERR_CODE_2);
        return NON_VALID_STAT;
    }

    FILE *fp;
    if(fp = fopen(*fullName, "w") == NULL){
        print_general_err(NULL, ERR_CODE_4);
        return NON_VALID_STAT;
    }

    return VALID_STAT;
}