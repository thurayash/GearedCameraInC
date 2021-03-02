#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

int main(int argc, char** argv){
    if (argc <2)
        err(127, "Not enough arguments");

    int alright = 1;
    for(int i = 1; i < argc; i++)
    {
        char buffer[BUFSIZ];
        strcpy(buffer, "awk 'length > max_length { max_length = length; longest_line = $0 } END { print longest_line }' ");
        strcat(buffer, argv[i]);
        FILE *fp = popen(buffer, "r");
        int c;
        char line[BUFSIZ];
        int count = 0;
        while(1) {
            char l[2];
            c = fgetc(fp);
            if( feof(fp) ) {
                break;
            }
            sprintf(l, "%c", c);
            strcat(line, l);
            count++;
        }
        if (count > 80)
        {
            printf(RED"Invalid file " RESET);
            printf(" %s : %s", argv[i],line);
            printf(YELLOW "%i Characters \n\n"RESET, count);
            alright = 0;
        }
        fclose(fp);
    }
    if (!alright)
        err(127, RESET"Files lines containing to much characters");
}
