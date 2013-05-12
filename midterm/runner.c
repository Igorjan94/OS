#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

char* buffer; 
int* separators;
int* parsed;
int* out;
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
        if (result == 0)
            return current;
        if (result == -1)
            return -1;
        current += result;
        size -= current;
    }
    write(1, buffer, current);
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
            separators[i] = separators[i - 1] + temp; else
                separators[i] = temp;
        i++;
    }
    return i;
}

void one_run(int s)
{
//    printf("здесь могла быть ваша реклама!\n");
//    return;

    char* inin = (char*) malloc(sizeof(char*) * (parsed[s] - separators[s] + 1));
    char* outout = (char*) malloc(sizeof(char*) * (separators[s + 1] - out[s]));
    memcpy(inin, buffer + separators[s], parsed[s] - separators[s] - 1);
    memcpy(outout, buffer + out[s], separators[s + 1] - out[s] - 2);
    printf("input : %s\noutput : %s\n", inin, outout);
    int pipefd[2];
    pipe(pipefd);
    int in = open(buffer + separators[s], O_RDONLY);
    int out1 = open(buffer + out[s], O_RDONLY);
    dup2(pipefd[0], in);
    dup2(pipefd[1], out1);
    char** temp = (char**) malloc((out[s] - parsed[s] + 2) * (sizeof(char*)));
    int i;
    int j = 0;
    int k = 0;
    for (i = out[s]; i < parsed[s] - 1; i++)
    {
        if (buffer[i] == sep)
        {
            j++;
            k = 0;
            continue;
        }
        temp[j][k++] = buffer[i];
    }
//    temp[j] = NULL;
//    int pid = fork();
//    if (pid) 
//    {
//        pid_t tpid;
//        int status;
//        do
//        {
//            tpid = wait(&status);
//        } while (tpid != pid);
//        _exit(0);
//    } else
//        if (execvp(temp[0], temp + 1) == -1)
//            _exit(1);
 execvp(temp[0], temp + 1); 
}

void parse(char separator, char* buffer, int size)
{
    int i;
    int j = 0;
    int flag = 0;
    int k = 0;
    for (i = 0; i < size; i++)
    {
        if (i == separators[j])
        {
            flag = 0;
            out[j] = k;
            j++;
            i++;
        } else
        if (buffer[i] == sep)
        {
            k = i;
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
        one_run(s);
    else 
        if (fork())
            run(--s); 
        else 
            one_run(s);
}

int make(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("uncorrect count of arguments\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        printf("error while reading file %s\n", argv[1]);
        return 2;
    }
    buffer = (char*) malloc(buffer_size);
    int size = _read(fd, buffer, buffer_size);
    close(fd);
    if (size < 0)
    {
        printf("File is empty!\n");
        return 3;
    }
    printf("Its size is : %d\nFile :\n%s\n", size, buffer);
    separators = (int*) malloc(sep_size + 1);
    parsed = (int*) malloc(sep_size);
    out = (int*) malloc(sep_size);
    sep_count = parse_commands(sep, buffer, size);
    if (sep_count < 0)
    {
        printf("No separators in file\n");
        return 4;
    }
    printf("count of separators : %d\n", sep_count);
    parse(sep, buffer, size);
    int i;
    for (i = sep_count - 1; i >= 0; i--)
        separators[i + 1] = separators[i] + 2;
    separators[0] = 0;
    for (i = 0; i < sep_count; i++)
    {
        parsed[i]++;
        out[i]++;
    }
    for (i = 0; i < sep_count + 1; i++)
        printf("%d %d %d\n", separators[i], parsed[i], out[i]);
    run(sep_count - 1);
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
    free(out);
    return er;
}
