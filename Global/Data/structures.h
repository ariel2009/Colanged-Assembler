#define MAX_ERR_MSG_LEN 25

/* Linked List node */
typedef struct node {

    /* key is string */
    char* key;

    /* value is also string */
    char* value;
    struct node* next;
} node;

typedef struct hashMap {

    /* Current number of elements in hashMap and capacity of hashMap */
    int numOfElements, capacity;

    /* hold base address array of linked list */
    node** arr;
} hashMap;

void setNode(struct node* node, char* key, char* value);
void initializeHashMap(struct hashMap* mp);
int hashFunction(struct hashMap* mp, char* key);
void insert(struct hashMap* mp, char* key, char* value);
void deleteRecord (struct hashMap* mp, char* key);
char* search(struct hashMap* mp, char* key);