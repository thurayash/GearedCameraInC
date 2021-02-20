#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define KNRM  "\x1B[0m"
#define K_ERROR  "\x1B[31m"
#define K_SUCCESS  "\x1B[32m"
#define K_WARNING  "\x1B[33m"
#define K_INFO  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define K_RESET  "\x1B[37m"


int main(void)
{
    printf("%sStarting configuration ...\n", K_WARNING);

    sleep(3);
    printf("%sThe configuration will be done with root access (y/n) :", K_WARNING);
    char chr;
    scanf("%c",&chr);
    if (chr == 'n' || chr == 'N')
    {
        printf("%sAborting ...",K_ERROR);
        return 0;
    }

    system("sudo ./.null");
    printf("\n===========================\n");

    printf("%sInstalling SDL libraries and dependencies :\n%s", K_INFO, KNRM);

    system("sudo apt install libsdl2-2.0-0 libsdl2-gfx-1.0-0 \
            libsdl2-image-2.0-0 libsdl2-mixer-2.0-0 libsdl2-net-2.0-0 \
            libsdl2-ttf-2.0-0");

    printf("%s--------------------------\n", K_RESET);

    printf("%sInstalling v4l2 libraries and dependencies :\n", K_INFO);

    printf("%s", KNRM);
    system("sudo apt-get install v4l-utils");

    printf("%s--------------------------\n", K_RESET);



    printf("%sConfiguration done with Success !\n", K_SUCCESS);
    return 0;
}
