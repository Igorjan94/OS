#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define COUNT_OF_TEMPERATURES 4
#define MAX_SIZE 16

using namespace std;

pid_t pid;

int readInt(int fd)
{
    int res, cur = 0;
    char buffer[MAX_SIZE];
    while ((res = read(fd, buffer + cur, MAX_SIZE - cur)) > 0)
        cur += res;
    buffer[cur + 1] = '\0';
    return atoi(buffer);
}
 
bool writeInt(int fd, int w)
{
    int res, cur = 0;
    char buffer[MAX_SIZE];
    sprintf(buffer, "%d", w);
    int ms = strlen(buffer);
    while ((res = write(fd, buffer + cur, ms - cur)) > 0)
        cur += res;
    write(fd, "\n", 1);
    return res >= 0;
}
    
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
//        close(STDIN_FILENO);
 //       close(STDOUT_FILENO);
  //      close(STDERR_FILENO);
        setsid();
        int t[COUNT_OF_TEMPERATURES];
        for (int i = 0; i < COUNT_OF_TEMPERATURES; i++)
        {
            char s[15];
            sprintf(s, "input%d.txt", i);
            int in = open(s, O_RDONLY);
            if (in < 0)
                return 2;
            t[i] = readInt(in);
            close(in);
        }
        int out = open("output.txt", O_CREAT | O_WRONLY, 0644);
        for (int i = 0; i < COUNT_OF_TEMPERATURES; i++)
            writeInt(out, t[i]);
        write(out, "\n", 1);
        close(out);
    }
    return 0;
}
