#include "tables.h"

command commands[] = {
    {"mov",  2},
    {"cmp",  2},
    {"add",  2},
    {"sub",  2},
    {"lea",  2},
    {"clr",  1},
    {"not",  1},
    {"inc",  1},
    {"dec",  1},
    {"jmp",  1},
    {"bne",  1},
    {"jsr",  1},
    {"red",  1},
    {"prn",  1},
    {"rts",  0},
    {"stop", 0}
};

char *instructions[] = {".data", ".string", ".mat", ".entry", ".extern"};