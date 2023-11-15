#if !defined(_RAW_PROCESS_H_)
#define _RAW_PROCESS_H_

#include <string>
#include <fstream>

enum class Tokens
{
    NAME = 2,
    STATUS = 3,
    RAM = 24,
    CPU = 15,
    PRIORITY = 18
};

class RawProcess
{
public:
    RawProcess(const std::string &path);
    ~RawProcess();
    void read();

    std::string name();
    std::string status();
    std::string priority();
    long cpuUsage();
    unsigned long long ramUsage();

private:
    std::string getToken(Tokens token);

private:
    std::ifstream file_;
    std::string stat_;
};

#endif // _RAW_PROCESS_H_