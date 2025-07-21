#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "../IO/FileHandler.h"
#include "../Preproccessor/mcro_expan.h"

int main(int argc, char const *argv[])
{
    int status = SUCCESS;
    int i;
    if(argc <= 1){
        status = ERROR;
        printf("Usage: ./assembler file1 file2.as ...\n");
    }
    for (i = 1; i < argc; i++)
    {
        char *fileName = (char *)malloc(MAX_LINE_LENGTH);
        char *no_extra_spaces_f_name = TMP_FILE_NAME;
        strcpy(fileName, argv[i]);
        if(try_add_file(fileName, SRC_FILE_EXT) && prepare_no_extra_spaces_file(fileName, SRC_FILE_EXT)){
            status = expand_macro(fileName, no_extra_spaces_f_name);
        }
        free(fileName);
    }
    
    return EXIT_SUCCESS;
}
