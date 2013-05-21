#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define STDIN 0
#define STDOUT 1

int main(int argc, char** argv)
{
    if (argc != 2)
        return 1;//no arguments
    int k = atoi(argv[1]);
    if (k++ < 1)
    	return 2;//invalid length( == 0)
    char* buffer = (char* ) malloc(k);
    int length, n;
    int skip = 0;
    length = 0;
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
                    int j;
                    for (j = 0; j < 2; j++)
                    {
                        int beginningOfWrite = endOfString + 1;
                        while (beginningOfWrite < i + 1)
                        {
                            int writer = write(STDOUT, buffer + beginningOfWrite, i - endOfString);
                            if (writer == -1)
                            {
                                free(buffer);
                                return 4;//IO error
                            }
                            beginningOfWrite += writer;
                        }
                    }
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
