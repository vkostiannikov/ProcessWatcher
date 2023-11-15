#if !defined(__FORMATTER_H__)
#define __FORMATTER_H__

#include <string>
#include <vector>

class ProcessList;

class Formatter {
public:
    Formatter();
    ~Formatter();

    std::string Format(ProcessList& threadsInfo);
    std::string formatCpuLoad(double load);
};


#endif // __FORMATTER_H__