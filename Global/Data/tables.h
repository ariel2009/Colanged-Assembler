typedef struct command_def{
    char *name;
    unsigned int opcode: 4;
    unsigned int paramCount: 2;
} command_def;

typedef struct command{
    command_def *def;
    char *label;
    char *src;
    char *dest;
} command;

typedef struct instruction{
    char *label;
    short *nums;
    char *arg_label;
    int length;
    unsigned int is_extern: 1;
} instruction;

int isRegister(char *possibleReg);
int isCommand(char *possibleCmd);
int isInstruct(char *possibleInst);