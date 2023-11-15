#include <RawProcess.h>
#include <sstream>
#include <iostream>

RawProcess::RawProcess(const std::string &path)
    : file_(path)
{
}

RawProcess::~RawProcess()
{
    file_.close();
}

void RawProcess::read()
{
   if(file_.is_open())
   {
       std::getline(file_, stat_);
   }
}

std::string RawProcess::getToken(Tokens token)
{
    std::string result;
    std::istringstream iss(stat_);
    for (int i = 0; i < static_cast<int>(token); ++i)
    {
        iss >> result;
    }
    return result;
}

std::string RawProcess::name()
{
    return getToken(Tokens::NAME);
}

std::string RawProcess::status()
{
    return getToken(Tokens::STATUS);
}

std::string RawProcess::priority()
{
    return getToken(Tokens::PRIORITY);
}

long RawProcess::cpuUsage()
{
    try
    {
        std::string token;
        std::istringstream iss(stat_);
        long utime, stime = 0.0;
        for (int i = 0; i < static_cast<int>(Tokens::CPU); ++i)
        {
            iss >> token;
            if (i == 13)
                utime = std::stod(token);
            if (i == 14)
                stime = std::stod(token);
        }
        return utime + stime;
    }
    catch (const std::exception &e)
    {
        throw;
    }

    return 0;
}

unsigned long long RawProcess::ramUsage()
{
    const auto ram = getToken(Tokens::RAM);
    if (ram.empty() || ram.find_first_not_of("0123456789") != std::string::npos)
    {
        return 0;
    }
    return std::stoull(ram);
}