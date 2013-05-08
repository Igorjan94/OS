#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

void brokenSymLinks(char* name)
{
    DIR* d;
    struct dirent* f;
    if (name == ".")
      return;
    d = opendir(name);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("No directory for search broken symlinks\n");
        return 1;
    }
    brokenSymLinks(argv[1]);
    printf("%s\n", argv[1]);
    return 0;
}
