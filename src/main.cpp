#define  WINVER      0x0500
#define _WIN32_WINNT 0x0500
#define  PSAPI_VERSION    1
#include <windows.h>
#include <tchar.h>

#include <iostream>
#include <cstdio>

#include "cmdline.h"    // Command line parsing
#include "unpage.h"     // Unpaging routine
#include "listprocs.h"  // Process listing routine

int main(int argc, char * argv[])
{

  // Parse command line options
  MyOptions myOpts;
  int       oi = parseOptions(argc, argv, myOpts);

  // We have two operation modes:
  // 1. List all processes
  // 2. Unpage a list of processes, possibly in an automated way

  if (myOpts.listProcesses)    // List all processes (like "ps")
  {
    ListProcesses();
  }
  else if (myOpts.automatic)   // Unpage all processes matching some pattern
  {
    // Unpage everything

    // Unpage matching user ID

    // Unpage matching image name
  }
  else while(argc > oi)         // PIDs specified on the command line, unpage each one.
  {
    SYSTEM_INFO si;
    DWORD       pid;

    ::GetSystemInfo(&si);
    pid = atoi(argv[--argc]);

    std::cerr << std::endl << "Unpaging process " << pid << " (\"" << getProcessNameByID(pid) << "\")." << std::endl
              << "This system: maximal application address is " << si.lpMaximumApplicationAddress
              << ", page file size is " << si.dwPageSize << " bytes." << std::endl;

    UnpageProcessByID(pid, si.lpMaximumApplicationAddress, si.dwPageSize);
  }

  return 0;

}
