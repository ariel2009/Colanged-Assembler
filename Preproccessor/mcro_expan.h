#include <stdio.h>
#include "../Global/Data/structures.h"

#define MCRO_DECL_TOK "mcro"
#define MACRO_END_TOK "mcroend"
#define MAX_MCRO_LINES 200

extern hashMap *macro_list;

void save_mcro(char *name, fpos_t *mcro_start, FILE *src, int lineCount);
char *expand_macro(char *src_file_name);
int validate_mcro_name(char *mcro_name);
char *expan_and_remove_defs(char *src_file_name);
char *skip_until_mcroend(char *line);
char *exchange_if_mcro_name(char *line);