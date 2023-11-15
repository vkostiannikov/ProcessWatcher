#include <formatter.h>

#include <sstream>
#include <iomanip>

#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ProcessList.h>
#include <SystemResources.h>

Formatter::Formatter()
{
}

Formatter::~Formatter()
{
}
const int STATUS_WIDTH = 8;
const int RAM_WIDTH = 28;
const int CPU_WIDTH = 28;
const int EXEC_TIME_WIDTH = 17;
const int PRIORITY_WIDTH = 12;
const int ID_WIDTH = 11;
const int NAME_WIDTH = 41;

std::string createProgressBar(double load, int width = 20)
{
    int bar_width = width; // Number of blocks to represent the progress bar
    int load_blocks = static_cast<int>(load * bar_width / 100.0);
    int color_code = load > 80.0 ? 31 : (load > 50.0 ? 33 : 32);
    std::stringstream ss;
    ss << "\033[" << color_code << "m"; // Set color
    // ss << std::left << std::setw(20) << std::fixed;
    for (int i = 0; i < bar_width; ++i)
    {
        if (i < load_blocks || (i == 0 && load > 0 && load < 100.0 / bar_width))
            ss << "█"; // Full block
        else
            ss << " "; // Fill the remaining symbols with spaces
    }
    ss << "\033[0m"; // Reset color
    ss << " " << std::setw(6) << std::fixed << std::setprecision(2) << load << "%";
    return ss.str();
}

std::string centerAlign(const std::string &str, int width) {
    int len = str.length();
    if(width < len) { return str; }

    int diff = width - len;
    int pad1 = diff/2;
    int pad2 = diff - pad1;
    return std::string(pad1, ' ') + str + std::string(pad2, ' ');
}

int getTerminalHeight()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

std::string Formatter::Format(ProcessList &processList)
{
    std::stringstream ss;
    ss << std::left; // Set alignment to left for all columns

    ss << "CPU usage% " << createProgressBar(SystemResourcesInstance.cpuLoad()) << "\n" << std::fixed << std::setprecision(2)
        << "RAM usage% " << createProgressBar(SystemResourcesInstance.ramUsage()) << "\t\tTotal memory: " 
        << SystemResourcesInstance.totalRam()/1024.0 << "MiB Free memory:" << SystemResourcesInstance.freeRam()/1024.0
        << "MiB Used memory: " << SystemResourcesInstance.usedRam() / 1024.0
        << "MiB Cached memory: " << SystemResourcesInstance.cachedRam() / 1024.0 << "MiB\n";
    ss << std::string(STATUS_WIDTH + RAM_WIDTH + CPU_WIDTH + EXEC_TIME_WIDTH + PRIORITY_WIDTH + ID_WIDTH + NAME_WIDTH + (7*3) - 2, '-') << "\n";
    // Headers
    ss << centerAlign("STATUS", STATUS_WIDTH) << " | "
       << centerAlign("RAM (MB)", RAM_WIDTH) << " | "
       << centerAlign("CPU (%)", CPU_WIDTH) << " | "
       << centerAlign("EXEC TIME", EXEC_TIME_WIDTH) << " | "
       << centerAlign("PRIORITY", PRIORITY_WIDTH) << " | "
       << centerAlign("ID", ID_WIDTH) << " | "
       << centerAlign("NAME", NAME_WIDTH) << " |"
       << "\n";
    ss << std::string(STATUS_WIDTH + RAM_WIDTH + CPU_WIDTH + EXEC_TIME_WIDTH + PRIORITY_WIDTH + ID_WIDTH + NAME_WIDTH + (7*3) - 2, '-') << "\n";
   // ss << "--------------------------------------------------------------------------------------------------\n";
    ss << std::right;
    int terminalHeight = getTerminalHeight() - 9;
    for (size_t i = 0; i < processList.size(); ++i)
    {
        const auto info = processList[i];
        ss << std::setw(STATUS_WIDTH) << info->getStatus() << " | "
           << createProgressBar(info->getRamUsage()) << " | "
           << createProgressBar(info->getCpuUsage()) << " | "
           << std::setw(EXEC_TIME_WIDTH) << std::fixed << std::setprecision(2) << info->getExecTime() << " | "
           << std::setw(PRIORITY_WIDTH) << info->getPriority() << " | "
           << std::setw(ID_WIDTH) << info->getPID() << " | "
           << std::setw(NAME_WIDTH) << info->getName() << " |" << "\n";
           if(terminalHeight > 0) {
               terminalHeight--;
           } else {
               break;
           }
    }
    ss << std::string(STATUS_WIDTH + RAM_WIDTH + CPU_WIDTH + EXEC_TIME_WIDTH + PRIORITY_WIDTH + ID_WIDTH + NAME_WIDTH + (7*3) - 2, '-') << "\n";
    return ss.str();
}

std::string Formatter::formatCpuLoad(double load)
{
    std::stringstream ss;
    ss << createProgressBar(load, 100) << "%\n";
    return ss.str();
}