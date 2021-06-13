#ifndef JSON_H_
#define JSON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

struct JSON{
    char              name;
    char         direction;
    int             radius;
    int              speed;
};

struct JSON* new_json(); // Init the parser with empty values

void free_json(struct JSON* parser);

void json_fromstr(struct JSON* m1, struct JSON* m2, char* string);

#endif
