#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <poll.h>
#define _GNU_SOURCE
#define buffer_size 4096

struct in_out
{
    char* buffer;
    struct pollfd in;
    struct pollfd out;
    int is_dead;
    int size;
};

size_t _read(int fd, char* buffer, size_t size)
{
    size_t current = 0;
    while (current < size)
    {
        size_t result = read(fd, buffer + current, size);
        if (result == 0)
            return current;
        if (result == -1)
            return -1;
        current += result;
        size -= current;
    }
    return current;
}

size_t _write(int fd, char* buffer, size_t size)
{
    size_t result = 0;
    while (result < size)
    {
        size_t done = write(fd, buffer, size - result);
        if (done < 0)
            return -1; else
            result += done;
    }
    memmove(buffer + result, buffer, buffer_size - result);
    return result; 
}

int main(int argc, char** argv)
{
    if (argc % 2 != 0)
    {
        printf("No arguments\n");
        return 1;
    }
    int t = 0;

    int count = argc / 2;
    struct in_out* fds = (struct in_out*) malloc(count * sizeof(struct in_out));
    int i;
    for (i = 0; i < argc; i += 2)
    {
        fds[i / 2].in.fd = atoi(argv[i]);
        fds[i / 2].out.fd = atoi(argv[i + 1]);
        fds[i / 2].buffer = (char*) malloc(buffer_size);
        fds[i / 2].in.events = POLLIN | POLLERR;
        fds[i / 2].out.events = POLLOUT | POLLERR;
        fds[i / 2].is_dead = 0;
        fds[i / 2].size = 0;
    }

    struct pollfd* for_poll = (struct pollfd*) malloc(argc * sizeof(struct pollfd));
    int j = 0;
    for (i = 0; i < count; i++)
    {
        for_poll[j++] = fds[i].in;
        for_poll[j++] = fds[i].out;
    }

    int f = 0;
    while (1) 
    {
        int y = poll(for_poll, argc, 0);
        if (y < 0)
        {
            printf("poll failed\n");
            return 2;
        }
        if (y == 0)
            break;

        int count_of_dead = 0;
        for (i = 0; i < argc; i++)
        {
            if (fds[i / 2].is_dead)
            {
                count_of_dead++;
                continue;
            }
            if (for_poll[i].revents & POLLERR)
                fds[i / 2].is_dead = 1;
            if (i % 2 == 0)
            {
                if (!(for_poll[i].revents & POLLIN))
                    continue;
                int a = _read(fds[i / 2].in.fd, fds[i / 2].buffer + fds[i / 2].size, buffer_size - fds[i / 2].size);
                if (a == 0 && fds[i / 2].size == 0)
                    fds[i / 2].is_dead = 1;
                if (a < 0)
                    continue;
                else
                    fds[i / 2].size += a;
            } else
            {
                if (!(for_poll[i].revents & POLLOUT))
                    continue;
                int a = _write(fds[i / 2].out.fd, fds[i / 2].buffer, fds[i / 2].size);
                if (a <= 0)
                    fds[i / 2].is_dead = 1;
                else
                    fds[i / 2].size -= a;
            }
        }
        if (count_of_dead == argc)
            break;
    }
    for (i = 0; i < count; i++)
        free(fds[i].buffer);
    free(fds);
    free(for_poll);
    return 0;
}
