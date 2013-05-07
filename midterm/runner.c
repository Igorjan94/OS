#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    printf("%s\n", "Hello, midterm...");
    if (argc != 2)
    {
        printf("uncorrect count of arguments\n");
        return 1;
    }
    int fd = open(argv[1]);
    if (fd == -1)
    {
        printf("error while reading file %s\n", argv[1]);
        return 2;
    }
    close(fd);
    printf("FINISHED!\n");
    return 0;
}
