#include "../header/json_parser.h"

struct JSON_RESPONSE* new_jsonResponse()
{
    struct JSON_RESPONSE* json_in = malloc(sizeof(struct JSON_RESPONSE));

    json_in->state = 0;

    return json_in;
}

void free_json_response(struct JSON_RESPONSE* parser)
{
    free(parser);
}

char** match_json(char* source, char* regexString, int* done)
{
    char** result = calloc(3, sizeof(char*));

    for(size_t i = 0;i < 3; i++){
        result[i] = calloc(16, sizeof(char));
    }

    regex_t regexCompiled;
    regmatch_t groupArray[3];


    if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
        errx(EXIT_FAILURE, "Could not compile regular expression.\n");

    char* cursor = source;
    *done = 0;


    if (regexec(&regexCompiled, cursor, 3, groupArray, 0)){
        printf("Data corrupted, please reset the hardware and retry again !\n");
        return result;
    }

    for (unsigned int g = 0; g < 3; g++)
    {
        if (groupArray[g].rm_so == -1)
            break;

        char cursorCopy[strlen (cursor) + 1];
        strcpy (cursorCopy, cursor);
        cursorCopy[groupArray[g].rm_eo] = 0;

        strcpy(result[g], cursorCopy + groupArray[g].rm_so);
        *done = 1;
    }
    regfree(&regexCompiled);

    return result;
}


void json_response_fromstr(struct JSON_RESPONSE* parser, char* string)
{
    int done = 0;
    char** result =  match_json(string, "'([A-Z]+)':([0-9])", &done);

    if(!done){
        for(int i = 0; i <3; i++)
            free(result[i]);
        errx(EXIT_FAILURE, "JSON not valid");
    }
    //printf("G0: %s, G1: %s, G2: %s\n",result[0], result[1], result[2]);

    if(*result[1] != 'S'){
        for(int i = 0; i <3; i++)
            free(result[i]);
        errx(EXIT_FAILURE, "JSON didn't match the correct key");
    }

    parser->state = atoi(result[2]);

    for(int i = 0; i <3; i++)
        free(result[i]);
    free(result);
}



