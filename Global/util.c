#include <stdio.h>

#include "util.h"
#include "defines.h"

char *removeExtraSpaces(char *str){
    char *out_str;
    int text_only_len = 0, pre_spaces_count = 0, textOnlyCount = 0;
    int isInText = STATE_OUT;
    int ch, i = 0;
    
    /* Loop while non-space text ended and put \0 there to sign end of string*/
    while ((ch = *(str + i++)) != '\0')
    {
        if(ch != ' ' && ch != '\t'){
            if(isInText == STATE_OUT){
                str += pre_spaces_count;
                i = 0;
            }

            isInText = STATE_IN;
        }
        else{
            if(isInText == STATE_IN){
                text_only_len = i;
            }
            else{
                pre_spaces_count++;
            }
            isInText = STATE_OUT;
        }
    }
    if(!text_only_len){
        return '\n';
    }

    *(str + i) = '\0';
    out_str = malloc(text_only_len);
    strcpy(out_str, str);
    
    return out_str;
}