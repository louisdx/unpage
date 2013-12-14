#define  WINVER      0x0500
#define _WIN32_WINNT 0x0500
#define  PSAPI_VERSION    1
#include <windows.h>
#include <tchar.h>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "cmdline.h"    // Command line parsing
#include "unpage.h"     // Unpaging routine
#include "listprocs.h"  // Process listing routine

int main(int argc, char * argv[])
{

    // Parse command line options
    MyOptions myOpts;
    int const oi = parseOptions(argc, argv, myOpts);

    // We have two operation modes:
    // 1. List all processes
    // 2. Unpage a list of processes, possibly in an automated way

    if (myOpts.listProcesses)    // List all processes (like "ps")
    {
        ListProcesses();
    }
    else                         // PIDs specified on the command line, unpage each one.
    {
        for (int i = oi; i < argc; ++i)
        {
            SYSTEM_INFO si;
            DWORD pid;

            ::GetSystemInfo(&si);

            char * e;
            errno = 0;
            pid = std::strtoul(argv[i], &e, 10);

            if (*e != '\0' || errno != 0)
            {
                std::cerr << "Failed to parse command line argument '" << argv[i] << "', skipping.\n";
                continue;
            }

            std::cerr << "\nUnpaging process " << pid << " (\"" << getProcessNameByID(pid) << "\").\n"
                      << "This system: maximal application address is " << si.lpMaximumApplicationAddress
                      << ", page file size is " << si.dwPageSize << " bytes.\n";

            UnpageProcessByID(pid, si.lpMaximumApplicationAddress, si.dwPageSize);
        }
    }
}
