#include <unistd.h>
//#include <stdio.h>
#include <stdlib.h>
#define STDIN 0
#define STDOUT 1

int stringToInt(char* c)
{
    int ans = 0;
    while (*c) 
    {
        if (*c >= '0' && *c <= '9')
	    ans = ans * 10 + *c - '0'; else
	    return -1;//invalid param
	c++;
    }
    return ans;
}

int main(int argc, char** argv)
{
//  printf("%s is working normally\n", argv[0]);
    if (argc != 2)
        return 1;//no arguments
    int k = stringToInt(argv[1]);
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
        }
	length += n;
	if (endOfString == -1)
	{
	    if (length == k)
	    {
	        length = 0;
		skip = 1;
	    }
	} else
	{
 	    length -= endOfString + 1;
	    memmove(buffer, buffer + endOfString + 1, length);
	}
    }
    free(buffer);
//  printf("%s finished normally\n", argv[0]);
    return 0;
}
