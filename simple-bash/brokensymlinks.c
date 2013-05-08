#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

int brokenSymLinks(char* name)
{
    DIR* d;
    struct dirent* f;
    if (name == ".")
      return;
    d = opendir(name);
    if (!d)
    {
        printf("%s directory not found", name);
        return 2;
    }
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("No directory for search broken symlinks\n");
        return 1;
    }
    int er = brokenSymLinks(argv[1]);
    return er;
}
