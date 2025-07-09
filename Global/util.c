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

    isInText = STATE_IN;
    text_start = i;
    for( ;i < MAX_LINE_LENGTH && (ch = *(str_copy + i)) != '\0'; i++){
        if(isspace(ch)){
            if(isInText == STATE_IN)
                text_end = i;
            isInText = STATE_OUT;
        }
        else{
            isInText = STATE_IN;
        }
    }
    if(text_end == 0){
        free(str_copy);
        return "\n";
    }

    *(str_copy + text_end) = '\0';
    str_copy += text_start;
    
    return str_copy;
}

int isExtraText(char *str, char *token, int preChck, char *afterTok){
    char *after_tok_temp = malloc(MAX_LINE_LENGTH);
    if(preChck){
        if((strcmp(strtok(str, " "), token)) != 0 && (strcmp(strtok(str, "\t"), token)) != 0){
                afterTok = strtok(NULL, "\n");
                return SUCCESS;
        }
        after_tok_temp = strtok(NULL, "\n");
        strcpy(afterTok, after_tok_temp);
        return ERROR;
    }
    return strcmp(strtok(str, "\n"), token) != 0;
}