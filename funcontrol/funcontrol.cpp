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
#define COUNT_OF_COOLERS 4
#define MAX_SIZE 16
#define SLEEP_TIME 5
#define NORMAL 20

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
 
struct vent
{
    char* name;
    int (*calculate)(vector<int>& a, vector<int>& b);

    int read()
    {
        int in = open(name, O_RDONLY);
        if (in < 0)
            _exit(1);
        int q = readInt(in);
        close(in);
        return q;
    }

    vent(char s[], int (*c)(vector<int>& a, vector<int>& b))
    {
        name = (char*) malloc((int)strlen(s) + 1);
        memcpy(name, s, (int)strlen(s));
        name[(int)strlen(s)] = '\0';
        calculate = c;
    }
};

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

void readInts(vector<int>& a, int count, char ss[])
{
    a.clear();
    a.resize(count);
    for (int i = 0; i < count; i++)
    {
        char s[15];
        sprintf(s, "%s%d.txt", ss, i);
        int in = open(s, O_RDONLY);
        if (in < 0)
            _exit(2);
        a[i] = readInt(in);
        close(in);
    }
}

void writeInts(vector<int>& a, int count, char ss[])
{
    for (int i = 0; i < count; i++)
    {
        char s[15];
        sprintf(s, "%s%d.txt", ss, i);
        int out = open(s, O_WRONLY | O_CREAT);
        if (out < 0)
            _exit(2);
        writeInt(out, a[i]);
        close(out);
    }
}

int calculate(vector<int>& t, vector<int>& c)
{
    int s = 0;
    for (int i = 0; i < COUNT_OF_TEMPERATURES; i++)
        s += t[i];
    s /= COUNT_OF_TEMPERATURES;
    int d = 0;
    if (s > NORMAL)
        for (int i = 0; i < COUNT_OF_COOLERS; i++)
            d += min(c[i] + 2, 255);
    else
        for (int i = 0; i < COUNT_OF_COOLERS; i++)
            if (t[i] > s)
                d += min(c[i] + 2, 255); else
                d += max(c[i] - 2, 0);
//    d = rand() % 100;
    return d / COUNT_OF_COOLERS;
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
        vector<int> t(COUNT_OF_TEMPERATURES);
        vector<int> c(COUNT_OF_COOLERS);
        vector<vent> vents;
        char te[] = "input";
        char co[] = "cooler";
        int counter = 0;
        for (int i = 0; i < COUNT_OF_COOLERS; i++)
        {
            char bu[15];
            sprintf(bu, "%s%d.txt", co, i);
            vents.push_back(vent(bu, calculate));
        }
        while (true)
        {
            readInts(t, COUNT_OF_TEMPERATURES, te);
            for (int i = 0; i < COUNT_OF_COOLERS; i++)
            {
                c[i] = vents[i].read();
                printf("%d\n", c[i]);
            }
            for (int i = 0; i < COUNT_OF_COOLERS; i++)
                c[i] = vents[i].calculate(t, c);
            writeInts(c, COUNT_OF_COOLERS, co);
            sleep(SLEEP_TIME);
            printf("%d\n", counter);
            if (counter++ == 10)
                break;
        }
        return 0;
    }
}
