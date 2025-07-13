#include <stdio.h>
#include "../Global/Data/structures.h"

#define MCRO_DECL_TOK "mcro"
#define MACRO_END_TOK "mcroend"
#define MAX_MCRO_LINES 200

int expand_macro(char *fileName);
int validate_mcro_name(char *mcro_name);
void save_mcro(char *name, fpos_t *mcro_start, FILE *src, int lineCount, hashMap **macro_list);