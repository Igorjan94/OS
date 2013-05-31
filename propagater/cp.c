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
    char** command = (char**) malloc(argc * sizeof(char*));
    int i;
    printf("asdfg %d\n", argc);
    command[0] = (char*) malloc(10);
    sprintf(command[0], "%s", "./main");
    int in, out;
    for (int i = 0; i < (argc - 1) / 2; i++)
    {
        int pid = fork();
        if (pid)
        {
	    pid_t tpid;
        int child_status;
        do
        {
            tpid = wait(&child_status);
            if (WIFEXITED(child_status) && !WEXITSTATUS(child_status))
            {
            command[i + 1] = (char*) malloc(10);
            command[i + 2] = (char*) malloc(10);
            in = open(argv[i + 1], O_RDONLY);
            out = open(argv[i + 2], O_WRONLY | O_CREAT, 0644);
            printf("%d %d\n", in, out);
            sprintf(command[i + 1], "%d", in);
            sprintf(command[i + 2], "%d", out);

            command[i + 3] = NULL;
            }
        }
        while (tpid != pid);

        } else
        {
            execvp(command[0], command + i + 1);
        }
    }
    return 0;
}
