#include "../header/main.h"
#include "../header/tools.h"


int initialisation_video(char* path)
{
    printf("Searching for compatible cameras ...\n\n");
    FILE *length_ls = popen("ls /dev/video* | wc -c", "r");
    FILE *ls_cmd = popen("ls /dev/video*", "r");

    if (ls_cmd == NULL || length_ls == NULL) {
        fprintf(stderr, "Error Code 1 : popen(3) error (main.c line 12)");
        exit(EXIT_FAILURE);
    }

    int size = 0;
    size_t n = 0;

    int index = 1;

    char buff[2];

    while ((n = fread(buff, 1, sizeof(buff)-1, length_ls)) > 0) {
        if (*buff < '0' || *buff > '9')
            continue;
        size = size*index + *buff - '0';
        index = index*10;
        buff[n] = '\0';
    }

    if(!size){
        printf("\n\n");
        errx(EXIT_FAILURE, "Error Code 5 :\
        No camera found please insert a camera !\n");
    }

    char* buffer = malloc(sizeof(char)*size+2);
    // Contient la liste des /dev/videoN

    n = 0;

    while((n = fread(buffer, 1, size, ls_cmd)) > 0){
        buffer[n] = '\0';
    }

    char* token = strtok(buffer, "\n");

    char** token_list = malloc(sizeof(char*)*(size/5));
    for(int i = 0; i < size /5; i++){
        token_list[i] = calloc(24, sizeof(char));
    }


    printf("Liste des camera compatible et disponibles : \n");
    printf(" ---------------------------------------------\n");
    int number_of_token = 0;
    int alone_token = 0;
    for(int i = 0;token != NULL;i++){
        if(vidtest_camCheck(token) > 0) {
            printf("| (%i) -----> %s is available !       |\n",i, token);
            strcpy(token_list[i], token);
            number_of_token++;
            alone_token = i;
        }
        else{
            printf("| (N) -----> %s is not available !   |\n", token);
            i--;
        }
        token = strtok(NULL, "\n");
    }
    printf(" ---------------------------------------------\n");
    printf("\n");
    if (number_of_token == 0)
        errx(EXIT_FAILURE, "Error Code 5 :\
        No camera was found please connect a camera !");

    if (number_of_token == 1){
        printf("Only compatible camera is %s, setting up...\n",\
                token_list[alone_token]);
        strcpy(path, token_list[alone_token]);
        printf("Success !\n \n ");
        goto token_jump;
    }


    printf("Select a camera (0-%i):", number_of_token-1);

    char rsp[10];
token_number:
    fflush( stdout );
    int scanf_state = scanf("%s", rsp);
    fgetc( stdin );

    int resp_int  = atoi(rsp);
    if (!(scanf_state || (resp_int > 0 && resp_int <= number_of_token)))
        goto token_number;


    strcpy(path, token_list[resp_int]);

token_jump:
    free(buffer);
    fclose(length_ls);
    fclose(ls_cmd);

    for(int i = 0; i < size/5; i++)
        free(token_list[i]);

    free(token_list);

    return 1;
}

void manual_command(int fd_in, int fd_out)
{
    while(1){
        printf(COLOR_WHITE"Turn motor 1 by : ");

        char rsp1[10];

        fflush( stdout );
        int scanf_state1 = scanf("%s", rsp1);
        fgetc( stdin );

        printf(COLOR_WHITE"Turn motor 2 by : ");

        char rsp2[10];

        fflush( stdout );
        int scanf_state2 = scanf("%s", rsp2);
        fgetc( stdin );


        if (scanf_state1 && scanf_state2){
            int radius1 = atoi(rsp1);
            int radius2 = atoi(rsp2);

            send_angle(fd_in, fd_out, radius1, radius2);
        }
        else{
            printf(COLOR_WHITE"Error: not a number. Please give a correct number.\n");
        }
    }

}


int software(int argc, char** argv, int fd_in, int fd_out)
{
    if(argc < 2)
        errx(EXIT_FAILURE, "Needs a mode !\n\
                \n0: Basic webcam V4L2\
                \n1: Skin detection noisy\
                \n2: Skin detection dilated and eroded\
                \n3: Robert edges detection grayscale\
                \n4: Real roberts edge detection\
                \n5: Hough Transform CircleDetection\
                \n6: Analyse square historgrame\
                \n7: Manual mode");
    int fd;


    char path[24];

    initialisation_video(path);

    int mode = atoi(argv[1]);

    if(mode == 7){
        manual_command(fd_in, fd_out);
        return 0;
    }

    fd = open(path, O_RDWR);
    if (fd == -1){
        perror("Error Code 1 : File not found : Opening video device");
        return 1;
    }

    if(print_caps(fd))
        return 1;

    sdlInit();

    if(init_mmap(fd))
        return 1;

    SDL_Event event;

    for(int i = 0; i != 10000; i++){
        if(capture_image(fd))
            return 1;
        sdlUpdate(mode,fd_in,fd_out);
        SDL_PollEvent(&event);
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        {
            printf("Closing process...\n");
            goto main_stop_jump;
        }
        usleep(3000);
    }


main_stop_jump:
    sdlStop();
    close(fd);
    return 0;
}

