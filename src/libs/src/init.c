#include "../header/init.h"


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
        errx(EXIT_FAILURE, COLOR_RED"H-Error code: 3002\n"COLOR_CYAN"Comment :Could not compile regular expression.\n");

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

int verification(int fd, char expected, struct pollfd* fds, int init){
    int r;
    char buffer[2];
    while(1){
        if (!poll(fds, 1, 100)){
            break;
        }
        r = read(fd, buffer, 1);
        if(r <= 0)
            perror(COLOR_RED"H-Error code: 3001\n"COLOR_CYAN"Comment: ERROR reading bytes");

        //printf(COLOR_WHITE"Confirmation !  received : %c  expected :%c \n", *buffer, expected);

        if(*buffer == expected)
            return 1;

        if(init && *buffer == '_')
            return 0;

    }
    return 0;
}

int write_string(int fd_in, int fd_out, char* msg, struct pollfd* fds)
{// {'Name':X}
    int resend = 1;
    if(!strcmp(msg, "OKEY!+")){
        resend = 0;
    }
    printf("RESEND : %i\n", resend);
    for(; *msg != '\0'; msg++)
    {
        char buffer[2];
        sprintf(buffer, "%c", *msg); // It doesn't accept char
        //printf(COLOR_WHITE"%s \n", buffer);
re_send:
        write(fd_in, buffer, 1); // Sending the byte
        if(!verification(fd_out, *buffer, fds, !resend)){ // Wait for the response
            printf(COLOR_WHITE"Write_string: Hardware didn't respond correctly \n");
            if(!resend)
                return 0;
            write(fd_in, "0", 1);
            goto re_send;
        }else{
            write(fd_in, "1", 1);
        }
    }
    printf(COLOR_WHITE"Data transmitted succesfuly !\n");
    return 1;
}

int waiting_response(char* port);

int currentState(struct JSON_RESPONSE* json, char* port){
    if (!(1 & json->state)){
        printf(COLOR_YELLOW"H-Warning ! : M1 X axis (phi) is not set\n");
    }

    if(!(2 & json->state)){
        printf(COLOR_YELLOW"H-Warning ! : M2 Y axis (theta) is not set\n");
    }

    if(!(4 & json->state)){
        printf(COLOR_RED"H-Error code : 3003\n"COLOR_CYAN"Comment : GCC hardware need to be restarted\n");
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
        errx(EXIT_FAILURE, COLOR_RED"H-Error code : 3004\n "COLOR_CYAN"Comment : File descriptor corrupted");

    fds->fd = fd_out;
    fds->events  = 0;
    fds->events |= POLLIN;

    char buff[1024];
    GString* result = g_string_new(NULL);

    if(!write_string(fd_in, fd_out, "OKEY!+", fds)){
        printf(COLOR_WHITE"\nHardware isn't connected. Please connect it or press on reset!\n\
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
        if (!poll(fds, 1, 30000)){
            printf(COLOR_WHITE"Waiting Response TIMEOUT\n");
            break;
        }
        r = read(fd_out, buff, 1);
        if (r <= 0)
            break;
        if(*buff == ' ' || *buff == '\n' || *buff < 33)
            continue;
        printf(COLOR_WHITE"C : %s I : %d\n", buff, *(buff+1));
        g_string_append_c(result, *buff);
        if(*buff == '}')
            break;
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

int send_angle(int fd_in, int fd_out, int angle1, int angle2)
{
    struct pollfd* fds = malloc(sizeof(struct pollfd*));

    fds->fd = fd_out;
    fds->events  = 0;
    fds->events |= POLLIN;

    int r = 0;
    char buffer[2];

    while(1)
    {

        if (!poll(fds, 1, 250)){
            return -1;
        }

        r = read(fd_out,buffer, 1);

        if(r <= 0){
            perror(COLOR_RED"H-Error code : 3001\n "COLOR_CYAN"Comment : verification, ERROR reading bytes, Hardware disconnected");
            free(fds);
            return 0;
        }

        //printf(COLOR_WHITE"Confirmation !  received : %c  expected : _ \n", *buffer);

        // Loop Back
        if(*buffer == '_'){
            write(fd_in, "_", 1);
            break;
        }
    }


    struct Motor* m1 = malloc(sizeof(struct Motor));
    m1->name = 'X';
    m1->direction = angle1 > 0;
    m1->radius = abs(angle1);
    m1->speed = 1;


    // Y axis
    struct Motor* m2 = malloc(sizeof(struct Motor));
    m2->name = 'Y';
    m2->direction = angle2 > 0;
    m2->radius = abs(angle2);
    m2->speed = 1;

    send_data2M(fd_in, fd_out, m1, m2);

    free(m1);
    free(m2);
    free(fds);

    return 1;
}



void launch_soft(int argc, char** argv,char* port){
    printf(COLOR_BLUE"H-Info : Launching Software on port %s\n"COLOR_WHITE, port);

    int fd_out = open(port, O_RDONLY);

    int fd_in = open(port, O_WRONLY);

    software(argc, argv, fd_in, fd_out);
}


int send_data2M(int fd_in, int fd_out, struct Motor* m1, struct Motor* m2)
{

    char* json_command = calloc(256, sizeof(char));

    sprintf(json_command, \
            "[{'N':%c,'D':%i,'R':%u};{'N':%c,'D':%i,'R':%i}]+", \
            m1->name, m1->direction, m1->radius,\
            m2->name, m2->direction,  m2->radius);

    printf(COLOR_WHITE"JSON COMMAND : %s\n", json_command);

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



int main(int argc, char** argv)
{

    char* regexCommand = "tty[A-RT-Z][A-Z]*[0-9]+";

    GString* ls = g_string_new(NULL);

    FILE *ls_cmd = popen("ls /dev", "r");
    if (ls_cmd == NULL) {
        fprintf(stderr, COLOR_RED"H-Error code : 3005\n"COLOR_CYAN"Comment : Command access corrupted !");
        exit(EXIT_FAILURE);
    }

    static char buff[BUF_SIZE];
    size_t n;

    while ((n = fread(buff, 1, sizeof(buff)-1, ls_cmd)) > 0) {
        buff[n] = '\0';
        ls = g_string_append(ls, buff);
    }

    if (pclose(ls_cmd) < 0)
        perror(COLOR_RED"H-Error code : 3006\n "COLOR_CYAN"Comment : Unable to close command !");

    size_t maxGroups = 1;

    int done;

    char** result = match_groups(ls->str, regexCommand, MAX_MATCHES, \
            maxGroups, &done);

    g_string_free(ls, TRUE);
    if (done){
        for(int i = 0; i <done; i++) printf(COLOR_WHITE"Port available: %s\n", result[i]);
    }
    else
        err(1,COLOR_RED"H-Error : 3007\n"COLOR_WHITE" "COLOR_CYAN"Comment: Connection failed !\n"COLOR_BLUE"H-Info: Unplug the device and retry !");

    for(int i = 0; i < done; i++){
        char* command = malloc(512* sizeof(char));

        sprintf(command, "stty -F /dev/%s cs8 9600 ignbrk -brkint -imaxbel \
                -opost -onlcr -isig -icanon -iexten -echo -echoe -echok \
                -echoctl -echoke noflsh -ixon -crtscts",result[i]);

        FILE* STTY = popen(command, "r");

        if(STTY == NULL){
            fprintf(stderr, COLOR_RED"H-Error code : 3005\n"COLOR_CYAN"Comment : Command access corrupted !");
            exit(EXIT_FAILURE);
        }

        char buff[BUF_SIZE];

        while ((n = fread(buff, 1, sizeof(buff)-1, STTY)) > 0) {
            buff[n] = '\0';
            printf(COLOR_WHITE"%s", buff);
        }

        if(!strcmp(buff,"\0")){
            printf(COLOR_RED"H-Error : 3008\n "COLOR_CYAN"Comment : An error occured while setting /dev/%s\n", result[i]);
            goto freeit;
        }

        sprintf(command, "/dev/%s", result[i]);

        if(waiting_response(command)) // It's the correct port
        {
            printf(COLOR_GREEN"H-Success : Hardware found and working ! Connection established !\n");
            // Lauch software
            launch_soft(argc, argv, command);
            free(command);
            break;
        }
freeit:
        if (pclose(STTY) < 0)
            perror(COLOR_RED"H-Error code : 3006\n "COLOR_CYAN"Comment : Unable to close command !");
        free(command);
    }

    for(unsigned i = 0; i < MAX_MATCHES; i++) free(result[i]);

    free(result);
    return 0;
}
