#define  WINVER      0x0500
#define _WIN32_WINNT 0x0500
#define  PSAPI_VERSION    1
#include <windows.h>

#include <iostream>
#include "unpage.h"
#include "listprocs.h"

void UnpageProcessByID(DWORD processID, LPVOID MaximumApplicationAddress, DWORD PageSize)
{
  SIZE_T                   nbytes;
  DWORD                    lpMem = 0;
  DWORD                    memsize = 0;
  MEMORY_BASIC_INFORMATION meminfo;

  unsigned char buf;

  // Get a handle to the process.
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

  // Get the process name.
  if (NULL == hProcess )
  {
    std::cerr << "Could not get process handle, skipping requested process ID " << processID << std::endl;
  }
  else
  {
    while (reinterpret_cast<LPVOID>(lpMem) < MaximumApplicationAddress)
    {
      DWORD stepsize = PageSize;

      if (!VirtualQueryEx(hProcess, reinterpret_cast<LPVOID>(lpMem), &meminfo, sizeof(meminfo)))
      {
        fprintf(stderr, "Error during VirtualQueryEx(), skipping process ID (error code %u, PID %u).\n", GetLastError(), processID);
        break;
      }

      if (meminfo.RegionSize < stepsize) stepsize = meminfo.RegionSize;

      switch(meminfo.State)
      {
      case MEM_COMMIT:
        //fprintf(stderr, "Page at 0x%08X: Good, unpaging.\n", lpMem);

        if (0 == ReadProcessMemory(hProcess, reinterpret_cast<LPVOID>(lpMem), reinterpret_cast<LPVOID>(&buf), 1, &nbytes))
          fprintf(stderr, "Failed to read one byte from 0x%X, error %u (%u bytes read).\n", lpMem, GetLastError(), nbytes);
        else
          ; // fprintf(stderr, "Read %u byte(s) successfully from 0x%X (byte was: 0x%X).\n", nbytes, lpMem, buf);

        memsize += stepsize;

        break;

      case MEM_FREE:
        //fprintf(stderr, "Page at 0x%08X: Free (unused), skipping.\n", lpMem);
        stepsize = meminfo.RegionSize;
        break;

      case MEM_RESERVE:
        //fprintf(stderr, "Page at 0x%08X: Reserved, skipping.\n", lpMem);
        stepsize = meminfo.RegionSize;
        break;

      default:
        fprintf(stderr, "Page at 0x%08X: Unknown state, panic!\n", lpMem);
      }

      //lpMem = (LPVOID)((DWORD)meminfo.BaseAddress + (DWORD)meminfo.RegionSize);
      lpMem += stepsize;
    }

    std::cerr << "Done. The process has " << memsize << " bytes (approx. "
              << memsize/1024/1024  << " MiB) of committed memory regions." << std::endl;
  }

  CloseHandle(hProcess);

}
