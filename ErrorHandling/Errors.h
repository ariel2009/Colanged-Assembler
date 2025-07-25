typedef struct location{
    char *file_name;
    unsigned long line;
} location;

typedef struct Error{
    int err_code;
    char *err_msg;
} Error;

typedef enum GENERAL_ERR{
    ERR_CODE_1 = 0,
    ERR_CODE_2,
    ERR_CODE_3,
    ERR_CODE_4,
    ERR_CODE_5,
    ERR_CODE_6,
    ERR_CODE_7,
    ERR_CODE_8,
    ERR_CODE_9
} GENERAL_ERR;

typedef enum MCRO_ERR{
    ERR_CODE_10 = 0,
    ERR_CODE_11,
    ERR_CODE_12,
    ERR_CODE_13,
    ERR_CODE_14,
    ERR_CODE_15,
    ERR_CODE_16
} MCRO_ERR;

void print_mcro_err(location *loc, unsigned short err_code);
void print_general_err(location *loc, unsigned short err_code);

void print_independed_err(int err_code, Error *type, int offset);
void print_depended_err(location loc, int err_code, Error *type, int offset);
