#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "../Preproccessor/mcro_expan.h"

int main(int argc, char const *argv[])
{
    int status = SUCCESS;
    int i;
    if(argc <= 1){
        status = ERROR;
        printf("Usage: ./assembler <file1.as> <file2.as> ...\n");
    }
    for (i = 1; i < argc && status == SUCCESS; i++)
    {
        char *fileName = (char *)malloc(MAX_LINE_LENGTH);
        strcpy(fileName, argv[i]);
        status = expand_macro(fileName);
        free(fileName);
    }
    
    return EXIT_SUCCESS;
}
