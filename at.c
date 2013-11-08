#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char** argv)
{
    int h = (argv[1][0] - '0') * 10 + argv[1][1] - '0';
    int m = (argv[1][3] - '0') * 10 + argv[1][4] - '0';
    printf("%d:%d\n", h, m);
    h = h * 60 + m;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    int j = h - (tm.tm_hour * 60 + tm.tm_min);
    printf("%d\n", j);
    if (j < 0)
        j += 24 * 60;
    sleep(j * 60);
    execvp(argv[2], argv + 2);
    return 0;
}
