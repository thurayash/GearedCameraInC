#include "json_parser.h"

struct JSON* new_json()
{
    struct JSON* json_in = malloc(sizeof(struct JSON));

    json_in->name = '0';
    json_in->direction = 0;
    json_in->radius = 0;
    json_in->speed = 0;

    return json_in;
}

void free_json(struct JSON* parser)
{
    free(parser);
}



// [{'N':X, 'D':1, 'S':1, 'R':360};{'N':Y, 'D':0, 'S':3, 'R':270000000}]
//
// La liste peut contenir qu'une seule information par exemple :
//                      [{'N':X, 'D':1, 'S':1, 'R':360}]
// Speed limit don't have to exceed 9
// The name needs to be first
void json_fromstr(struct JSON* parser1, struct JSON* parser2, char* string)
{
    char* start = string;

    struct JSON* changer = parser1;

    char key_val;
    short is_key = 0;
    short is_val = 0;

    char* rotation;
    int rotation_val = 0;
    for(;*start != ']'; start++)
    {
        if(*start == '[' || *start== '{' || *start == ' ')
            continue;

        if(*start == '\''){
            is_key = !is_key;
            continue;
        }

        if(is_key){
            key_val = *start;
            continue;
        }

        if(*start == ':'){
            is_val = !is_val;
            continue;
        }

        if(is_val){
            switch (key_val){
                case 'N':
                    // Name
                    changer =  (parser1->name == *start) ? parser1: parser2;
                    break;
                case 'D':
                    // Direction
                    changer->direction = *start - '0';
                    break;
                case 'S':
                    // Speed
                    changer->speed = *start - '0';
                    break;
                case 'R':
                    // Step
                    rotation = start;
                    for(; *rotation != ',' && *rotation != '}'; rotation++){
                        rotation_val = (rotation_val*10)+ (*rotation - '0');
                    }
                    start = rotation;
                    changer->radius = rotation_val;
                    rotation_val = 0;
                    break;
                default:
                    break;
            }
            is_val = !is_val;
        }

    }
}
