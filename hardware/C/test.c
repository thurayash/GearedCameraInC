#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <time.h>


void write_string(int fd, char* msg);
void verification(int fd);


int main(int argc, char** argv)
{
    if(argc < 2)
        errx(EXIT_FAILURE, "Needs a string");

    int fd; /* File descriptor for the port */

    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1)
        perror("open_port: Unable to open /dev/ttyACM0 - ");
    else
        fcntl(fd, F_SETFL, 0);

    write_string(fd, argv[1]);


    close(fd);
    //if (n < 0)
    //    fputs("write() of 4 bytes failed!\n", stderr);
    return 0;
}


void write_string(int fd, char* msg)
{
    for(; *msg != '\0'; msg++)
    {
        char buffer[2];
        sprintf(buffer, "%c", *msg); // It doesn't accept char
        write(fd, buffer, 1); // Sending the byte
        verification(fd); // Wait for the response
    }
}

void verification(int fd)
{
    int r;
    char buffer[3];
    while((r = read(fd, buffer, 1)) > 0)
    {
        if(r == 0)
            printf("Verification : ERROR reading bytes");
        //printf("%s", buffer);
        if(!strcmp(buffer, "1"))
        {
            //printf(": Received ! \n");
            return (void)NULL;
        }
    }
    return (void)NULL;
}

