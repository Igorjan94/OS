#include <stdio.h>
#include "lazystream.cpp"

int main(int argc, char** argv)
{
    LazyStream a(0, 20, '\n');
    vector<char> s = a.read();
    s = a.read();
    a.write(s);
    s = a.read();
    a.write(s);
    return 0;
}
