/* Linked List node */
struct node {

    /* key is string */
    char* key;

    /* value is also string */
    char* value;
    struct node* next;
};

struct hashMap {

    /* Current number of elements in hashMap and capacity of hashMap */
    int numOfElements, capacity;

    /* hold base address array of linked list */
    struct node** arr;
};

void setNode(struct node* node, char* key, char* value);
void initializeHashMap(struct hashMap* mp);
int hashFunction(struct hashMap* mp, char* key);
void insert(struct hashMap* mp, char* key, char* value);
void deleteRecord (struct hashMap* mp, char* key);
char* search(struct hashMap* mp, char* key);