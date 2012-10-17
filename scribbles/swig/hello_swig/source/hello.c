#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hello.h"

Greeter* create_greeter() {
    Greeter* g = malloc(sizeof(Greeter));
    g->phrase = NULL;
    g->count = 0;
    set_phrase(g, "Hello");
    return g;
}

void delete_greeter(Greeter* g) {
    if (g) {
        if (g->phrase)
            free(g->phrase);
        free(g);
    }
}

int get_count(Greeter* g) {
    if (g)
        return g->count;
    else
        return -1;
}

void set_phrase(Greeter* g, char* p) {
    if (g && p) {
        size_t s = sizeof(char)*(strlen(p)+1);
        g->phrase = realloc(g->phrase, s);
        strcpy(g->phrase, p);
    }
}

char* get_phrase(Greeter* g) {
    if (g) {
        size_t s = sizeof(char)*(strlen(g->phrase)+1);
        char* p = malloc(s);
        strcpy(p, g->phrase);
        return p;
    } else
        return NULL;
}

void print_greeting(Greeter* g, char* name) {
    if (g && name) {
        char* p = get_phrase(g);
        printf("%s %s\n", p, name);
        g->count++;
        free(p);
    }
}
