#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "lazystream/lazystream.cpp"

using namespace std;

int main(int argc, char** argv)
{
    LazyStream l(0, 64, '\n');
    vector< vector<char> > com;
    vector<char> temp;
    int asdf = 0;
    while (true)
    {
        temp = l.read();
        if (temp.size() == 0)
            break;
        com.push_back(temp);
    }
    for (int i = 0; i < com.size(); i++)
    {
        char* temp[1000];
        vector<vector<char>> t1;
        t1.resize(1000);
        vector<char> t = com[i];
        temp[0] = const_cast<char*> ("./at");
        int k = 1;
        t.push_back('\0');
        for (int i = 0; i < t.size(); i++)
        {
            if (t[i] == ' ' || i == t.size() - 1)
            {
                t1[k].push_back('\0');
                temp[k] = const_cast<char*>(t1[k].data());
                k++;
            } else
                t1[k].push_back(t[i]);
        }
        if (fork())
            execvp(temp[0], temp);
    }
    return 0;
}
