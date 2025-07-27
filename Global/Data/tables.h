#define CMD_COUNT 16
#define REG_COUNT 8
#define INST_COUNT 5

typedef enum TABLE_TYPE{
    EXTERN_OR_ENTRY,
    LABEL
} TABLE_TYPE;

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

typedef struct label{
    char *name;
    int type;
    int address;
} label;

typedef struct segment{
    unsigned short num;
    label *associated_lbl; 
} segment;

int isRegister(char *possibleReg);
int isCommand(char *possibleCmd);
int isInstruct(char *possibleInst);
int add_extern_or_entry(instruction *inst_data);
int add_label(label *input_label);