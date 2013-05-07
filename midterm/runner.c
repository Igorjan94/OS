#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

char* buffer; 
int* separators;
int* parsed;
int buffer_size = 4096;
int sep_size = 128;
int sep_count = 0;
char sep = '\n';

size_t _read(int fd, char* buffer, size_t size)
{
    size_t current = 0;
    while (current < size)
    {
        size_t result = read(fd, buffer + current, size);
        if (result <= 0)
            return current;
        current += result;
        size -= current;
    }
    return current;
}

int find_separator(char separator, char* buffer, int size)
{
    if (size <= 0)
        return -1;
    int i;
    for (i = 0; i < size; i++)
        if (buffer[i] == separator && buffer[i + 1] == separator)
            return i;
}

int parse_commands(char separator, char* buffer, int size)
{
    int temp;
    int i = 0;
    int count = 0;
    while (size > 1)
    {
        temp = find_separator(separator, buffer + count, size);
        count = temp + 2;
        size -= temp + 2;
        if (temp == -1)
            break;
        if (i > 0)
            separators[i++] = separators[i - 1] + temp; else
                separators[i++] = temp;
    }
    return i;
}

void one_run(int s)
{
    int pid = fork();
    if (pid) 
    {
        pid_t tpid;
        int status;
        do
        {
            tpid = wait(&status);
        } while (tpid != pid);
        _exit(0);
    } else
        if (execvp(buffer + separators[s], buffer + parsed[s]) == -1)
            _exit(1);
}

void parse(char separator, char* buffer, int size)
{
    int i;
    int j = 0;
    int flag = 0;
    for (i = 0; i < size; i++)
    {
        if (i == separators[j])
        {
            j++;
            flag = 0;
        }
        if (buffer[i] == sep)
        {
            buffer[i] = ' ';
            if (!flag)
            {
                flag = 1;
                parsed[j] = i;
            }
        }
    }
}
void run(int s)
{
    if (s == 0)
    {
       one_run(s);
       return;
    } 
    int pipefd[2];
    pipe(pipefd);
    if (fork())
    {
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        close(pipefd[1]);
        run(s--);
    } else 
    {
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        close(pipefd[1]);
        one_run(s);
    }
}

int make(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("uncorrect count of arguments\n");
        return 1;
    }
    int fd = open(argv[1]);
    if (fd == -1)
    {
        printf("error while reading file %s\n", argv[1]);
        return 2;
    }
    int size = _read(fd, buffer, buffer_size);
    close(fd);
    buffer = (char*) malloc(buffer_size);
    separators = (int*) malloc(sep_size);
    if (size < 0)
    {
        printf("File is empty!\n");
        return 3;
    }
    write(1, buffer, size);
    sep_count = parse_commands(sep, buffer, size);
    if (sep_count < 0)
    {
        printf("No separators in file\n");
        return 4;
    }
    parsed = (int*) malloc(sep_count);
    int i;
    for (i = 0; i < sep_count; i++)
        printf("%d ", parsed[i]);
    printf("\n");
    run(sep_count);
    return 0;
}

int main(int argc, char** argv)
{
    int er = make(argc, argv);
    if (er == 0)
        printf("FINISHED!\n");
    free(buffer);
    free(separators);
    free(parsed);
    return er;
}
