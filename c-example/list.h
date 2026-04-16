// **really** simple dynamic list impl

typedef struct StringList {;
    size_t len;
    size_t count;
    char* *data;
} StringList ;

void StringList_init(StringList* list, size_t inital_capacity) {
    list-> len = 0;
    list-> count = inital_capacity;
    list-> data = calloc(sizeof(int) * inital_capacity, sizeof(char*));
}

void StringList_append(StringList* list, char* value) {
    StringList_reserve(list);
    printf("Hi! We got this: %s\n", value);
    list->data[list->len++] = value;
}

void StringList_reserve(StringList* list) {
    if(list-> len == list-> count) {
       size_t newcap =  list-> count*2;
       list-> count = newcap;
       list->data = realloc(list->data, newcap );
    }
}

char* StringList_get(StringList* list, int index) {
    return list-> data[index];
}