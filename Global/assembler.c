#include <stdlib.h>
#include "defines.h"
#include "../Preproccessor/mcro_expan.h"

int main(int argc, char const *argv[])
{
    unsigned short status = SUCCESS;
    if(argc <= 1){
        status = ERROR;
        printf("Usage: ./assembler <file1.as> <file2.as> ...");
    }
    for (int i = 1; i < argc && status == SUCCESS; i++)
    {
        char *fileName = *argv[i];
        status = expand_macro(*fileName);
    }
    
    return EXIT_SUCCESS;
}
