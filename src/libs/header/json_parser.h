#ifndef JSON_H_

#define JSON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <regex.h>

struct JSON_RESPONSE{
    unsigned char state : 3;
};

struct Motor{
    char        name;
    unsigned int direction : 1;
    unsigned int radius;
    unsigned int speed     : 4;
};



struct JSON_RESPONSE* new_jsonResponse();


void free_json_response(struct JSON_RESPONSE* parser);

void json_response_fromstr(struct JSON_RESPONSE* parser, char* string);
#endif
