#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <utility>
#include "lazystream.h"

using namespace std;

LazyStream::LazyStream(int fd, int size, char separator)
{
    this->buffer.resize(size);
    this->fd = fd;
    this->size = size;
    this->separator = separator;
}

int LazyStream::findSeparator()
{
    for (int i = 0; i < current_size; i++)
        if (buffer[i] == separator)
           return i;
    return -1;
}

vector<char> LazyStream::read()
{
    int current = 0;
    vector<char> temp;
    int i;
    bool flag = false;
    while ((i = findSeparator()) == -1)
    {
        if (current_size >= size)
        {
            current_size = 0; 
            flag = true;
        }
        else
            while (current < size)
            {
                int result = ::read(fd, buffer.data() + current_size, size - current_size);
                if (result < 0)
                    exit(1);
                if (result == 0)
                {
                    i = findSeparator();
                    break;
                }
                current_size += result;
            }
    }
    current_size -= i + 1;
    vector<char> temp2(size);
    memmove(temp2.data(), buffer.data() + i + 1, current_size);
    fill(buffer.begin() + i, buffer.end(), 0);
    vector<char> forReturn(move(buffer));
    buffer = move(temp2);
    if (flag)
        return move(temp);
    return forReturn;
}

void LazyStream::write(vector<char>& toWrite)
{
    int result = 0;
    int ffd = fd;
    fd = 1;
    while (result < size)
    {
        int done = ::write(fd, toWrite.data() + result, size - result);
        if (done < 0)
            return; else
            result += done;
    }
    ::write(fd, &separator, 1);
    fd = ffd;
}

