#include "json_parser.h"

struct JSON* new_json()
{
    struct JSON* json_in = malloc(sizeof(struct JSON));

    json_in->name = '0';
    json_in->direction = 1;
    json_in->radius = 0;
    json_in->speed = 0;

    return json_in;
}

void free_json(struct JSON* parser)
{
    free(parser);
}


void json_fromstr(struct JSON* parser, char* string)
{
    char *start = string;

    char *key   = calloc(10, sizeof(char));
    char *remind_key = key;
    char *value = calloc(10, sizeof(char));
    char *remind_value = value;
    char is_key = 0;

    for(; *start != '\0'; start++)
    {
        if (start == string && *start != '{')
            send_string("JSON string not valid !");

        if (*start == 58 || *start == 32 || *start == 123)
            continue;

        if (*start == 39){ // '
            is_key = !is_key; // first time => it's a key else it's a value
            continue;
        }

        if(is_key){
            *key = *start;
            key++;
        }
        else{
            if (*start == ',' || *start == 125)
            {
                if (!strcmp(key, "Name")){
                    parser->name = *(remind_value); // Since it's just a char
                }
                else if (!strcmp(key, "Direction")){
                    parser->direction = (int)(*(remind_value)) - 48; // It's a bit
                }
                else if (!strcmp(key, "Speed")){
                    parser->speed = (int)atoi(remind_value);
                }
                else if (!strcmp(key, "Radius")){
                    parser->radius = (int)atoi(remind_value);
                }
                else{
                    send_string("JSON invalid key");
                }
                value = remind_value; // Back to the first add
                memset(value,0,strlen(value));
                memset(key,0,strlen(key));
            }
            else{
                *value = *start;
                value++;
            }
            key = remind_key;
        }
    }
    free(value);
    free(key);
}

