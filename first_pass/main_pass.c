#include <string.h>

#include "main_pass.h"
#include "./instruction_handle/extern_and_entry.h"
#include "../IO/FileHandler.h"
#include "../Global/defines.h"

int scan_am_file(char *filename){
    int success = 0;
    /*Use add_file from FileHandler to validatefile name*/

    FILE *am_file = fopen(filename, "r");
    /*
        Validate file is not null.
        Validate there is no over-length line
    */

    char line[MAX_LINE_LENGTH];
    char line_copy[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, am_file) != NULL)
    {
        strcpy(line_copy, line);
        success = handle_entry_or_extern(line_copy);
    }

    return success;
}