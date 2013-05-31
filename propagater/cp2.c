#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("No arguments\n");
        return 1;
    }
    char** command = (char**) malloc((argc + 2) * sizeof(char*));
    command[0] = (char*) malloc(10);
    sprintf(command[0], "%s", "./main");
    int in, out;
    int j = 0;
    for (j = 1; j < argc; j += 2)
    {
        command[j] = (char*) malloc(10);
        command[j + 1] = (char*) malloc(10);
        in = open(argv[j], O_RDONLY);
        out = open(argv[j + 1], O_WRONLY | O_CREAT, 0644);
        sprintf(command[j], "%d", in);
        sprintf(command[j + 1], "%d", out);
    }
    if (fork())
        execvp(command[0], command + 1);
    return 0;
}
