#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"
#include "defines.h"


char *removeExtraSpaces(char *str){
    char *str_copy;
    char ch;
    int i =0, text_start = 0, text_end = 0;

    str_copy = (char *)malloc(strlen(str) + 1);
    strcpy(str_copy, str);
    /* Loop while non-space text ended and put \0 there to sign end of string*/
    while(*(str_copy + i) == ' ' || *(str_copy + i) == '\t') i++;
    text_start = i;

    for( ;i < MAX_LINE_LENGTH && (ch = *(str_copy + i)) != '\0'; i++){
        if(!isspace(ch)){
            text_end = i;
        }
    }

    *(str_copy + text_end + 1) = '\0';
    str_copy += text_start;
    return str_copy;
}

int isExtraText(char *str){
    return strchr(str, '\t') || strchr(str, ' ');
}

int is_tok_in_str(char *str, char *tok){
    char *str_copy = (char *)malloc(strlen(str) + 1);
    char *token_iter;
    
    strcpy(str_copy, str);
    token_iter = strtok(str_copy, " \t\n");

    while(token_iter != NULL){
        if(strcmp(token_iter, tok) == 0){
            free(str_copy);
            return SUCCESS;
        }
        token_iter = strtok(NULL, " \t\n");
    }

    free(str_copy);
    return ERROR;
}

char *getToken(char *str, char *delim){
    char *str_copy = (char *)malloc(strlen(str) + 1);
    char *token;

    strcpy(str_copy, str);
    token = strtok(str_copy, delim);

    return token;
}

int get_line_count(char *content){

    int i = 0, line_count = 1;
    char current_ch;
    char *content_copy  = (char *)malloc(strlen(content) + 1);

    if(content == NULL || strlen(content) <= 1){
        return ERROR;
    }

    strcpy(content_copy, content);

    while ((current_ch = *(content_copy + i)) != '\0'){
        if(current_ch == '\n'){
            line_count++;
        }
    }

    free(content_copy);
    return line_count;
}