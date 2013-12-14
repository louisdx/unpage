#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <getopt.h>
#include "cmdline.h"

static void printUsage()
{
    std::cerr << "Missing arguments or options. Try 'unpage -l' to list processes, or 'unpage -h' for help.\n";
}

int parseOptions(int argc, char * argv[], MyOptions & options)
{
    char option;

    while ((option = getopt(argc, argv, "hvl")) != EOF)
    {
        switch(option)
        {
        case 'h':
            std::cout << "Process lister and unpager." << std::endl << std::endl
                      << "Usage: unpage -h" << std::endl
                      << "       unpage -l" << std::endl
                      << "       unpage [-v] PID [PID ...]" << std::endl << std::endl
                      << "       -l:    Lists all processes." << std::endl
                      << "       -v:    Outputs verbose information." << std::endl
                      << "       -h:    Displays this help text."  << std::endl << std::endl
                      << "If one or more process IDs (PIDs) are specified, each corresponding" << std::endl
                      << "process will be \"unpaged\" by reading from each page of its memory," << std::endl
                      << "forcing swapped memory pages to be read back into physical RAM." << std::endl
                      << std::endl;
            std::exit(0);
            break;
        case 'v':
            options.verbose = true;
            break;
        case 'l':
            options.listProcesses = true;
            break;
        case '?':
            if (optopt == 'm')
            {
                std::cerr << "Option -" << reinterpret_cast<char *>(&optopt) << " requires an argument." << std::endl;
            }
            else if (isprint(optopt))
            {
                std::cerr << "Unknown option `-" << reinterpret_cast<char *>(&optopt) << "'." << std::endl;
            }
            printUsage();
            return 1;
        default:
            std::cerr << "Internal option parsing bug, we should never get here!" << std::endl;
            printUsage();
            std::abort();
        }
    }

    if (!options.listProcesses && optind == argc)
    {
        printUsage();
        std::exit(0);
    }

    return optind;
}
