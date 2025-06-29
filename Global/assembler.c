#include <stdlib.h>
#include "defines.h"
#include "../Preproccessor//mcro_expan.h"

int main(int argc, char const *argv[])
{
    unsigned short status = VALID_STAT;
    if(argc <= 1){
        status = NON_VALID_STAT;
        printf("Usage: ./assembler <file1.as> <file2.as> ...");
    }
    for (int i = 1; i < argc && status == VALID_STAT; i++)
    {
        char *fileName = *argv[i];
        if(status = isFileNameValid(*fileName, "as"))
            expand_macro(*fileName);
    }
    
    return EXIT_SUCCESS;
}
