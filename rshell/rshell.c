#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>

int main(int argc, char** argv)
{
    if (argc < 2)
        return 1;
    struct addrinfo hints;
    struct addrinfo *result;
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    int s = getaddrinfo(NULL, "8822", &hints, &result);
    if (s != 0)
    {
        printf("%s\n", gai_strerror(s));
        return 1;
    }
    int sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sfd == -1)
        return 2;
    int yes = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) != 0)
        return 4;
    if (bind(sfd, result->ai_addr, result->ai_addrlen) != 0)
        return 3;
    freeaddrinfo(result);
    if (listen(sfd, 5) == -1)
        return 5;
    struct sockaddr_storage sdt_stor;
    socklen_t sdt_stor_len = sizeof(sdt_stor);
    int fd = accept(sfd, (struct sockaddr*) &sdt_stor, &sdt_stor_len);
    if (fd < 0)
        return 6;
    if (fork())
    {
        close(fd);
    } else
    {
        dup2(fd, 0);
        dup2(fd, 1);
        printf("Hello, world!\n");
        close(fd);
        return 0;
    }
    printf("%s\n", argv[1]);
    return 0;
}
