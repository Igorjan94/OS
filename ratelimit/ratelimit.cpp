#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include "./../lazystream/lazystream.cpp"
#define INF 1000000000ll

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Not enough arguments\n");
        return 1;
    }
    int n = atoi(argv[1]);
    if (n <= 0)
    {
        printf("Integer must be positive\n");
        return 1;
    }
    LazyStream l(0, 4096, '\n');
    int k = 0;
    vector<char> s;
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    long long t = time.tv_sec * INF + time.tv_nsec;
    long long cur_time;
    s = l.read();
    while (true)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        cur_time = time.tv_sec * INF + time.tv_nsec - t;
        if (k < n && cur_time < INF)
        {
            l.write(s);
            s = l.read();
            k++;
            if (k == n)
                usleep((INF - cur_time) / 1000);
        }
        clock_gettime(CLOCK_MONOTONIC, &time);
        cur_time = time.tv_sec * INF + time.tv_nsec - t;
        if (cur_time >= INF)
        {
            k = 0;
            t = time.tv_sec * INF + time.tv_nsec;
            cur_time = 0ll;
        }
    }
    return 0;
}
