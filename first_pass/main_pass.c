#include <string.h>
#include <stdlib.h>

#include "../Global/Data/tables.h"
#include "main_pass.h"
#include "handle_instruction.h"
#include "../IO/file_handler.h"
#include "../Global/defines.h"
#include "../Global/util.h"

int scan_am_file(char *filename){
    int IC = 0, DC = 0;
    int status = 1;
    /*Use add_file from FileHandler to validate file name*/

    FILE *am_file = fopen(filename, "r");
    /*
        --Validate file is not null.
        --Validate there is no over-length line
    */

    char line[MAX_LINE_LENGTH];
    char line_copy[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, am_file) != NULL)
    {
        strcpy(line_copy, line);

        if(strstr(line_copy, ".extern") != NULL || strstr(line_copy, ".entry") != NULL){
            status = read_entry_or_extern(line_copy);
        }
        else if(strstr(line_copy, ".data") != NULL || strstr(line_copy, ".string") != NULL){
            read_str_data(line_copy, &DC);
        }
    }

    return status;
}

/*int insert_data_segment(int *DC, unsigned short *data){
    
}*/