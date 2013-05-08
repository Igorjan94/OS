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
    int length = strlen(name);
    char* buf = malloc(length + 512);
    memcpy(buf, name, sizeof(char) * (length + 1));
    //printf("directory for search : %s\n", buf);
    if (buf[length - 1] != '/')
    {
        buf[length] = '/';
        buf[++length] = 0;
    }
    while ((f = readdir(d)) != NULL)
    {
        if (!strcmp(f->d_name, ".") || !strcmp(f->d_name, ".."))
            continue;
        memcpy(buf + length, f->d_name, sizeof(char) * (strlen(f->d_name) + 1));
        if (f->d_type == DT_DIR)
            brokenSymLinks(buf);
        if (f->d_type == DT_LNK && access(buf, F_OK) == -1)
            printf("broken symlink: %s\n", buf);
        buf[length] = 0;
    }
    closedir(d);
    free(buf);
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
