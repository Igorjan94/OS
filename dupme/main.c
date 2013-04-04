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

int main(int argc, char **argv)
{
    printf("%s is working normally\n", argv[0]);
    if (argc != 2)
    {
        return 1;
    }
    printf("%d\n", argc);
    return 0;
}
