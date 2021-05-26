#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <gmodule.h>
#include <glib.h>
#include <poll.h>

#include "../../header/hardware/json_parser.h"

#define BUF_SIZE 256
#define MAX_MATCHES 10
#define TIMEOUT 1000

// Check if modemmanager not installed

char** match_groups(char* source, char* regexString, size_t maxMatches, \
        size_t maxGroups, int* done)
{
    char** result = malloc(maxMatches* sizeof(char*));

    for(size_t i = 0;i < maxMatches; i++){
        result[i] = calloc(256, sizeof(char));
    }

    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];


    if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
        errx(EXIT_FAILURE, "Could not compile regular expression.\n");

    char* cursor = source;
    int index = 0;
    *done = 0;

    for(; maxMatches > 0; maxMatches--)
    {
        if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
            break;  // No more matches

        char cursorCopy[strlen(cursor) + 1];
        strcpy(cursorCopy, cursor);
        cursorCopy[groupArray[0].rm_eo] = 0;

        strcpy(*(index + result), (cursorCopy + groupArray[0].rm_so));
        *done += 1;

        index ++;
        cursor += groupArray[0].rm_eo;
    }

    regfree(&regexCompiled);

    return result;
}


int verification(int fd, struct pollfd* fds){
    int r;
    char buffer[2];
    while(1){
        if (!poll(fds, 1, 100)){
            break;
        }
        r = read(fd, buffer, 1);
        if(r <= 0)
            perror("Verification : ERROR reading bytes");
        if(*buffer == '1')
            return 1;
    }
    return 0;
}


int write_string(int fd_in, int fd_out, char* msg, struct pollfd* fds)
{// {'Name':X}
    for(; *msg != '\0'; msg++)
    {
        char buffer[2];
        sprintf(buffer, "%c", *msg); // It doesn't accept char
        write(fd_in, buffer, 1); // Sending the byte
        if(!verification(fd_out, fds)){ // Wait for the response
            printf("Write_string: Hardware didn't respond\n");
            return 0;
        }
    }
    printf("Data transmitted succesfuly !\n");
    return 1;
}

int waiting_response(char* port);

int currentState(struct JSON_RESPONSE* json, char* port){
    if (!(1 & json->state)){
        printf("M1 X axis (phi) is not set\n");
    }

    if(!(2 & json->state)){
        printf("M2 Y axis (theta) is not set\n");
    }

    if(!(4 & json->state)){
        printf("GCC hardware need to be restarted\n");
        fflush( stdout );
        char* rsp = NULL;
        int scanf_state = scanf("%c", rsp);
        fgetc( stdin );
        if (scanf_state && *rsp == 'y'){
            waiting_response(port);
        }
        return 0;
    }
    return 1;
}


int waiting_response(char* port)
{
    struct pollfd* fds = malloc(sizeof(struct pollfd*));

    struct JSON_RESPONSE* json = new_jsonResponse();

    int fd_out = open(port, O_RDONLY), r;
    int fd_in = open(port, O_WRONLY);

    if (fd_in < 0 || fd_out < 0)
        errx(EXIT_FAILURE, "File descriptor corrupted");

    fds->fd = fd_out;
    fds->events  = 0;
    fds->events |= POLLIN;

    char buff[1024];
    GString* result = g_string_new(NULL);

    if(!write_string(fd_in, fd_out, "OKEY!+", fds)){
        printf("\nHardware isn't connected. Please connect it or press on reset!\n\
                Restart (y/n) ?");

        char rsp[10];

        fflush( stdout );
        int scanf_state = scanf("%c", rsp);
        fgetc( stdin );

        if (scanf_state && *rsp == 'y'){
                free_json_response(json);
                close(fd_in);
                close(fd_out);
                free(fds);
            return waiting_response(port);
        }
        else
            goto finish_wr;
    }

    while(1){
        if (!poll(fds, 1, TIMEOUT)){
            printf("Waiting Response TIMEOUT\n");
            break;
        }
        r = read(fd_out, buff, 1);
        if (r <= 0)
            break;
        if(*buff == ' ' || *buff == '\n' || *buff < 33)
            continue;
        printf("C : %s I : %d\n", buff, *(buff+1));
        g_string_append_c(result, *buff);
    }

    json_response_fromstr(json, result->str);

    if(currentState(json, port)){
         free_json_response(json);
        close(fd_in);
        close(fd_out);
        free(fds);
        return 1;
    }

finish_wr:
    free_json_response(json);
    close(fd_in);
    close(fd_out);
    free(fds);
    return 0;
}

void launch_soft(char* port){ /*LAUNCH SOFTWARE HERE !!! */
    printf("Launching Software on port %s\n", port);

    int fd_out = open(port, O_RDONLY);
    int fd_in = open(port, O_WRONLY);

    struct Motor* m1 = malloc(sizeof(struct Motor));
    m1->name = 'X';
    m1->direction = 1;
    m1->radius = 9;
    m1->speed = 1;
    printf("ON\n");
    send_data1M(fd_in, fd_out, m1);
    sleep(3);
    m1->direction = 0;
    printf("OFF\n");
    send_data1M(fd_in, fd_out, m1);
    sleep(3);
    printf("ON\n");
    m1->direction = 1;
    send_data1M(fd_in, fd_out, m1);
    free(m1);
}


int send_data2M(int fd_in, int fd_out, struct Motor* m1, struct Motor* m2)
{

    char* json_command = calloc(128, sizeof(char));

    sprintf(json_command, \
            "[{'N':%c,'D':%i,'S':%i,'R':%u};{'N':%c,'D':%c,'S':%i,'R':%u}]+", \
            m1->name, m1->direction, m1->speed, m1->radius,m2->name, \
            m2->direction, m2->speed, m1->radius);

    struct pollfd* fds = malloc(sizeof(struct pollfd*));

    fds->fd = fd_out;
    fds->events  = 0;
    fds->events |= POLLIN;

    int rsp = write_string(fd_in, fd_out, json_command, fds);

    free(fds);
    free(json_command);
    return rsp;
}


int send_data1M(int fd_in, int fd_out, struct Motor* m1)
{
    char* json_command = calloc(128, sizeof(char));

    sprintf(json_command, \
            "[{'N':%c,'D':%i,'R':%u,'S':%i}]+", \
            m1->name, m1->direction, m1->radius, m1->speed);

    struct pollfd* fds = malloc(sizeof(struct pollfd*));

    fds->fd = fd_out;
    fds->events  = 0;
    fds->events |= POLLIN;

    int rsp = write_string(fd_in, fd_out, json_command, fds);

    free(fds);
    free(json_command);
    return rsp;
}



int launch_start()
{

    char* regexCommand = "tty[A-RT-Z][A-Z]*[0-9]+";

    GString* ls = g_string_new(NULL);

    FILE *ls_cmd = popen("ls /dev", "r");
    if (ls_cmd == NULL) {
        fprintf(stderr, "popen(3) error");
        exit(EXIT_FAILURE);
    }

    static char buff[BUF_SIZE];
    size_t n;

    while ((n = fread(buff, 1, sizeof(buff)-1, ls_cmd)) > 0) {
        buff[n] = '\0';
        ls = g_string_append(ls, buff);
    }

    if (pclose(ls_cmd) < 0)
        perror("pclose(3) error");

    size_t maxGroups = 1;

    int done;

    char** result = match_groups(ls->str, regexCommand, MAX_MATCHES, \
            maxGroups, &done);

    g_string_free(ls, TRUE);
    if (done){
        for(int i = 0; i <done; i++) printf("Port available: %s\n", result[i]);
    }
    else
        err(1,"Connection failed ! Unplug the device and retry !");

    for(int i = 0; i < done; i++){
        char* command = malloc(512* sizeof(char));

        sprintf(command, "stty -F /dev/%s cs8 9600 ignbrk -brkint -imaxbel \
                -opost -onlcr -isig -icanon -iexten -echo -echoe -echok \
                -echoctl -echoke noflsh -ixon -crtscts",result[i]);

        FILE* STTY = popen(command, "r");

        if(STTY == NULL){
            fprintf(stderr, "popen(3) error");
            exit(EXIT_FAILURE);
        }

        char buff[BUF_SIZE];

        while ((n = fread(buff, 1, sizeof(buff)-1, STTY)) > 0) {
            buff[n] = '\0';
            printf("%s", buff);
        }

        if(!strcmp(buff,"\0")){
            printf("An error occured while setting /dev/%s\n", result[i]);
            goto freeit;
        }

        sprintf(command, "/dev/%s", result[i]);

        if(waiting_response(command)) // It's the correct port
        {
            printf("Hardware found and working ! Connection established !\n");
            // Lauch software
            launch_soft(command);
            free(command);
            break;
        }
freeit:
        if (pclose(STTY) < 0)
            perror("pclose(3) error");
        free(command);
    }

    for(unsigned i = 0; i < MAX_MATCHES; i++) free(result[i]);

    free(result);
    return 0;
}
