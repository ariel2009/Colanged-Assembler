#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"
#include "defines.h"


/* Fix bugs in this function */
char *removeExtraSpaces(char *str){
    char *str_copy;
    int isInText = STATE_OUT;
    char ch;
    int i =0, text_start = 0, text_end = 0;

    str_copy = malloc(sizeof(str));
    strcpy(str_copy, str);
    /* Loop while non-space text ended and put \0 there to sign end of string*/
    while(*(str_copy + i) == ' ' || *(str_copy + i) == '\t') i++;
    text_start = i;

    isInText = STATE_IN;
    for( ;i < MAX_LINE_LENGTH && (ch = *(str_copy + i)) != '\0'; i++){
        if(!isspace(ch)){
            if(isInText)
                text_end++;
            else
                text_end = i;
            isInText = STATE_OUT;
        }
        else
            isInText = STATE_IN;
    }

    *(str_copy + text_end + 1) = '\0';
    str_copy += text_start;
    return str_copy;
}

int isExtraText(char *str, char *token){
    return strcmp(str, token) !=0;
}