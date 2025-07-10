#include <stdio.h>
#include "../Global/Data/structures.h"

#define MCRO_DECL_TOK "mcro"
#define MACRO_END_TOK "mcroend"

int expand_macro(char *fileName);
int validate_mcro_decl(char *str, char *mcro_name);
void save_mcro(char *name, fpos_t *mcro_start, FILE *src, int lineCount, hashMap **macro_list);