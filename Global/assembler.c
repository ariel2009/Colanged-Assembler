#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "../first_pass/main_pass.h"
#include "../Preproccessor/mcro_expan.h"
#include "../IO/file_handler.h"

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
        strcpy(fileName, argv[i]);

        if(try_add_file(fileName, SRC_FILE_EXT)){
            status = expand_macro(fileName);
        }
        free(fileName);
    }
    
    return EXIT_SUCCESS;
}
