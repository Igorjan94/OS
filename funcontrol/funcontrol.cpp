#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

pid_t pid;

void handler(int) {
    kill(pid, SIGINT);
}

int main(int, char **)
{
    pid = fork();
    if (pid)
    {
        signal(SIGINT, handler);
        std::cout << "Started daemon, pid: " << pid << std::endl;
        int status;
        waitpid(pid, &status, 0);
        std::cout << "Daemon stopped" << std::endl;
    } else
    {
        int log_fd = dup(STDOUT_FILENO);
        if (log_fd == -1)
        {
            _exit(43);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        setsid();
    }
    return 0;
}
