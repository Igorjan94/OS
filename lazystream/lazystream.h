#include <vector>
#include <cstdlib>

class LazyStream
{
    int fd;
    char separator;
    int size;
    int current_size;
    std::vector<char> buffer;
    int findSeparator();
    public:
    LazyStream(int fd, int size, char separator);
    std::vector<char> read();
    void write(std::vector<char>& toWrite);
};
