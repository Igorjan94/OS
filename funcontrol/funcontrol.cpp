#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#define COUNT_OF_TEMPERATURES 4

using namespace std;

pid_t pid;

int main(int, char **)
{
    pid = fork();
    if (pid)
    {
        cout << "Started daemon, pid: " << pid << "\n";
        int status;
        waitpid(pid, &status, 0);
        cout << "Daemon stopped" << "\n";
    } else
    {
        if (dup(STDOUT_FILENO) == -1)
            _exit(1);
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        setsid();
        int in = open("input.txt", O_RDONLY);
        int out = open("output.txt", O_CREAT | O_WRONLY, 0644);
        if (in < 0 || out < 0)
        {
            printf("fail opening file\n");
            return 1;
        }
        close(in);
        close(out);
    }
    return 0;
}
