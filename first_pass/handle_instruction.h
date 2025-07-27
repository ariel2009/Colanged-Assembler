#define EXTERN 1
#define ENTRY 0

typedef enum address_type{
    CODE_SEG,
    DATA_SEG
} address_type;

int read_entry_or_extern(char *str);
int read_str_data(char *str, int *DC);
int read_nums(instruction **data, char *str);