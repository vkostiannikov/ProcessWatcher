#if !defined(__APPLICATION_H__)
#define __APPLICATION_H__

#include <memory>
#include <atomic>

class Display;
class ProcessManager;

class application
{
private:
public:
    void run(char *argv[], int argc);
    void parseArgs(char *argv[], int argc);
    void help();

private:
    char getKeyStroke();
    void start();
};

#endif