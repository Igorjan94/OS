#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define STDIN 0
#define STDOUT 1

int find(int c, char* buffer, int from, int count)
{
    int i;
   for (i = from; i < from + count + 1; i++)
        if (buffer[i] == c)
            return i;
    return -1;
}

int main(int argc, char** argv)
{
    char c = 'a';
    int size = 4096;
    int r;
    int length = 0;
    int n;
    int options = 0;
    while ((options = getopt(argc, argv, "nzb:")) != -1)
        switch (options)
        {
            case 'n' :
                if (c == '\0')
                    return 2;
                c = '\n';
                break;
            case 'z' :
                if (c == '\n')
                   return 2;
                c = '\0';
                break;
            case 'b' :
                size = atoi(optarg);
                break;
            case '?' :
            default :
                return 3;
        }
    if (c == 'a')
        c = '\n';
    size++;
    printf("Params %s, buf_length = %d\n", c == '\n' ? "\\n" : "\\0", size);
    char* buffer = (char*) malloc(size);
    int olen = 0;
    int from;
    int delimpos;
    while (1)
    {
       r = read(STDIN, buffer + length, size - length);
       if (r == -1)
       {
           free(buffer);
           return 1;
       }
       from = olen;
       olen += r;
       int i;
       while ((delimpos = find(c, buffer, from, olen - from)) >= 0)
       {
           write(STDOUT, buffer, delimpos + 1);
           memmove(buffer, buffer + delimpos + 1, olen - delimpos - 1);
           from = 0;
           olen -= delimpos + 1;
       }
       if (olen > 0)
       {
           if (olen + 1 >= size)
               return 4;
           buffer[olen + 1] = c;
           write(STDOUT, buffer, delimpos + 1);
       }
       if (r == 0)
           break;//the end;
    }
    return 0;
}
