#ifndef _TOOLS_H
#define _TOOLS_H

char *mystrdup(const char* s1);

#ifndef strdup
#define strdup mystrdup
#endif
#endif
