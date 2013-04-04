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
    int i, j;
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
	break;
    }
    printf("%d\n", argc);
    return 0;
}
