#include <stdlib.h>
#include <string.h>
#include "tools.h"

char *mystrdup(const char* s1) {
    char* a = NULL;
    a = malloc(strlen(s1) + 1);
    if(a == NULL) {
        return NULL;
    }
    strcpy(a, s1);
    return a;
}
