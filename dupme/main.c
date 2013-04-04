#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int stringToInt(char* c)
{
    int ans = 0;
    while (*c) 
    {
        if (*c >= '0' && *c <= '9')
	    ans = ans * 10 + *s - '0'; else
	    return -1;
	s++;
    }
    return ans;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        return 1;
    }
}
