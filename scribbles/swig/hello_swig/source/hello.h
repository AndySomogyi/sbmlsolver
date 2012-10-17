#ifndef HELLO_HEADER
#define HELLO_HEADER

typedef struct _Greeter {
    char* phrase;
    int   count;
} Greeter;

Greeter* create_greeter();
void     delete_greeter(Greeter* g);

int get_count(Greeter* g);

void  set_phrase(Greeter* g, char* phrase);
char* get_phrase(Greeter* g);

void print_greeting(Greeter* g, char* name);

#endif
