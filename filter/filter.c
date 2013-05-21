#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#define STDIN 0
#define STDOUT 1
#define BUFFER_SIZE 4096

int find(int c, char* buffer, int from, int count)
{
    int i;
    for (i = from; i < from + count + 1; i++)
        if (buffer[i] == c)
            return i;
    return -1;
}

void write_(int fd, char* buffer, int size)
{
    int result = 0;
    while (result < size)
    {
        int done = write(fd, buffer, size - result);
        if (done < 0)
            _exit(1); else
            result += done;
    }
}

int one_run(char** command, char* buffer, int count, char separator)
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
                write_(1, buffer, count);
                write_(1, &separator, 1);
            }
        }
        while (tpid != pid);
    }
    else
    {
        execvp(command[0], command);
        exit(0);
    }
    return 0;
}

int main(int argc, char** argv)
{
    char c = 'a';
    int size = BUFFER_SIZE;
    int options = 0;
    while ((options = getopt(argc, argv, "nzb:")) != -1)
        switch (options)
        {
            case 'n' :
                if (c == '\0')
                    return 2;
                c = '\n';
                break;
            case 'z' :
                if (c == '\n')
                   return 2;
                c = '\0';
                break;
            case 'b' :
                size = atoi(optarg) + 1;
                break;
            case '?' :
            default :
                return 3;
        }
    if (c == 'a')
        c = '\n';
    if (argc - optind + 2 <= 0)
    {
        printf("No command\n");
        return 1;
    }
    char** command = malloc((argc - optind + 2) * sizeof(char*));
    int i;
    for (i = optind; i < argc; i++)
        command[i - optind] = argv[i];
    command[argc - optind + 1] = 0;
    int r;
    int length = 0;
    char* buffer = (char*) malloc(size);
    int olen = 0;
    do
    {
        r = read(STDIN, buffer + length, size - length);
        if (r == -1)
        {
            free(buffer);
            return 1;
        }
        int from = olen;
        olen += r;
        int delimpos;
        while ((delimpos = find(c, buffer, from, olen - from)) >= 0)
        {
            one_run(command, buffer, delimpos, c);
            memmove(buffer, buffer + delimpos + 1, olen - delimpos - 1);
            from = 0;
            olen -= delimpos + 1;
        }
        if (olen > 0)
        {
            if (olen + 1 >= size)
                return 4;
            buffer[olen + 1] = c;
            one_run(command, buffer, olen, c);
        }
    }
    while (r);
    free(buffer);
    free(command);
    return 0;
}
