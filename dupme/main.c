#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define STDIN 0
#define STDOUT 1

int _write(char* buffer, int from, int to)
{
    int beginningOfWrite = from;
    while (beginningOfWrite < to)
    {
        int writer = write(STDOUT, buffer + beginningOfWrite, to - from - 1);
        if (writer == -1)
        {
            free(buffer);
            return -1;//IO error
        }
        beginningOfWrite += writer;
    }
    return 0;
}

int solve(int argc, char** argv)
{
    if (argc != 2)
        return 1;//no arguments
    int k = atoi(argv[1]);
    if (k < 1)
    	return 2;//invalid length( <= 0)
    k += 1;
    char* buffer = (char* ) malloc(k);
    int length = 0, n;
    int skip = 0;
    while (1)
    {
    	n = read(STDIN, buffer + length, k - length);
    	if (n == 0)
    	    break;//the end
    	if (n == -1)
    	{
    	    free(buffer);
    	    return 3;//IO error
    	}
    	int i;
    	int endOfString = -1;
    	for (i = length; i < length + n; i++)
    	    if (buffer[i] == '\n')
    	    {
                if (!skip)
                {
                    if (_write(buffer, endOfString + 1, i) != 0)
                        return 4;
                    if (_write(buffer, endOfString + 1, i) != 0)
                        return 4;
                }
                endOfString = i;
                skip = 0;
            }
        length += n;
        if (endOfString == -1 && length == k)
        {
            length = 0;
            skip = 1;
        } else
        {
            length -= endOfString + 1;
            memmove(buffer, buffer + endOfString + 1, length);
        }
    }
    free(buffer);
    return 0;
}

int main(int argc, char** argv)
{
    return solve(argc, argv);
}
