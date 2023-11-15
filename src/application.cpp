#include <application.h>
#include <ProcessManager.h>
#include <termios.h>
#include <unistd.h>
#include <display.h>
#include <csignal>
#include <functional>

void application::help()
{
    std::cout << "Usage: proccessWatcher [OPTION]..." << std::endl;
    std::cout << "  -h, --help\t\t\tDisplay this help and exit" << std::endl;
    std::cout << "  -v, --version\t\t\tDisplay version information and exit" << std::endl;
    std::cout << "In run mode, press 'q' to exit" << std::endl;

    std::cout << "Sorting Order Options:" << std::endl;
    std::cout << "  'p'  Sort by Process ID (Ascending)" << std::endl;
    std::cout << "  'P'  Sort by Process ID (Descending)" << std::endl;
    std::cout << "  'm'  Sort by Memory Usage (%) (Ascending)" << std::endl;
    std::cout << "  'M'  Sort by Memory Usage (%) (Descending)" << std::endl;
    std::cout << "  'u'  Sort by CPU Usage (%) (Ascending)" << std::endl;
    std::cout << "  'U'  Sort by CPU Usage (%) (Descending)" << std::endl;
    std::cout << "  't'  Sort by Process Status (Ascending)" << std::endl;
    std::cout << "  'T'  Sort by Process Status (Descending)" << std::endl;
    std::cout << "  'n'  Sort by Process Name (Ascending)" << std::endl;
    std::cout << "  'N'  Sort by Process Name (Descending)" << std::endl;
}

char application::getKeyStroke()
{
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void application::parseArgs(char *argv[], int argc)
{
    if (argc == 1)
    {
        start();
    }
    else if (argc == 2)
    {
        std::string arg = argv[1];
        if (arg == "-h" || arg == "--help")
        {
            help();
        }
        else if (arg == "-v" || arg == "--version")
        {
            std::cout << "proccessWatcher version 1.0" << std::endl;
        }
        else
        {
            std::cout << "Invalid argument: " << arg << std::endl;
            help();
        }
    }
    else
    {
        std::cout << "Invalid number of arguments" << std::endl;
        help();
    }
}

void application::start()
{
    auto display = std::make_shared<Display>();
    auto processManager = std::make_unique<ProcessManager>(display);
    processManager->start();
    char key;
    while (true)
    {
        // read keystroke
        key = getKeyStroke();
        if (key == 'q')
        {
            processManager->stop();
            break;
        }
        else
        {
            processManager->setSort(key);
        }
    }
}

void application::run(char *argv[], int argc)
{
    try
    {
        parseArgs(argv, argc);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}