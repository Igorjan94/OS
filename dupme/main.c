#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int stringToInt(char* c)
{
    int ans = 0;
    while (*c) 
    {
        if (*c >= '0' && *c <= '9')
	    ans = ans * 10 + *c - '0'; else
	    return -1;
	c++;
    }
    return ans;
}

int main(int argc, char** argv)
{
    printf("%s is working normally\n", argv[0]);
    if (argc != 2)
    {
        return 1;
    }
    int k = stringToInt(argv[1]);
    if (k < 1)
	return 2;
    char* buffer = (char* ) malloc(k);
    int length, n;
    int skip = 0;
    length = 0;
    while (1)
    {
	n = read(0, buffer + length, k - length);
	if (n == 0)
	    break;
	if (n == -1)
	{
	    free(buffer);
	    return 3;
	}
	int i;
	int endOfString = -1;
	for (i = length; i < length + n; i++)
	{
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
		            int writer = write(1, buffer + beginningOfWrite, i - endOfString);
		            if (writer == -1)
		            {
		                free(buffer);
			        return 4;
			    }
		            beginningOfWrite += writer;
		        }
		    }
    	        }
		endOfString = i;
		skip = 0;
	    }
        }
	length += n;
	if (endOfString == -1)
	{
	    if (length == k)
	    {
	        length = 0;
		skip = 0;
	    }
	} else
	{
 	    length -= endOfString + 1;
	    memmove(buffer, buffer + endOfString + 1, length);
	}
    }
    free(buffer);
    printf("%s finished normally\n", argv[0]);
 //   printf("%d\n", argc);
    return 0;
}
