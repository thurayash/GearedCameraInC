#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <time.h>
#include <errno.h>

void write_string(int fd, char* msg);
void verification(int fd);

void delay(int numOfSec)
{
    int numOfMilliSec = 1000 * numOfSec;
    time_t startTime = clock();
    while(clock() < startTime + numOfMilliSec);
}



int main(int argc, char** argv)
{
    if(argc < 3)
        errx(EXIT_FAILURE, "Needs the port and a JSON string");

    int fd; /* File descriptor for the port */

    fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1)
        perror("open_port: Unable to open the filedescriptor !");
    else
        fcntl(fd, F_SETFL, 0);

    write_string(fd, argv[2]);

    close(fd);

    return 0;
}


void write_string(int fd, char* msg)
{// {'Name':X}
    for(; *msg != '\0'; msg++)
    {
        char buffer[2];
        sprintf(buffer, "%c", *msg); // It doesn't accept char
        write(fd, buffer, 1); // Sending the byte
        //delay(5);
        verification(fd); // Wait for the response
    }
}


void verification(int fd){
    int r;
    char buffer[2];
    while((r = read(fd, buffer, 1)) >= 0){
        if(r == 0)
            perror("Verification : ERROR reading bytes");
        if(!strcmp(buffer, "1"))
            return (void)NULL;
    }
}


