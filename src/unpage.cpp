#define  WINVER      0x0500
#define _WIN32_WINNT 0x0500
#define  PSAPI_VERSION    1
#include <windows.h>

#include <iostream>
#include <iomanip>
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
                auto e = GetLastError();
                if (e == ERROR_INVALID_PARAMETER)
                {
                    // Error: address above the highest memory address accessible to the process
                    break;
                }
                else
                {
                    // Some other error
                    std::cerr << "Error during VirtualQueryEx(), skipping process ID (error code "
                              << std::dec << e << ", PID " << processID << ").\n";
                    break;
                }
            }

            if (meminfo.RegionSize < stepsize)
            {
                stepsize = meminfo.RegionSize;
            }

            switch (meminfo.State)
            {
            case MEM_COMMIT:
            {
                //fprintf(stderr, "Page at 0x%08X: Good, unpaging.\n", lpMem);

                if (meminfo.Protect & PAGE_GUARD)
                {
                    // skip guard page, which is not readable, nor necessary
                    break;
                }

                if (0 == ReadProcessMemory(hProcess, reinterpret_cast<LPVOID>(lpMem), &buf, 1, &nbytes))
                {
                    std::cerr << "Failed to read one byte from 0x" << std::hex << lpMem << ", error "
                              << std::dec << GetLastError() << " (" << nbytes << " bytes read); Protection is 0x"
                              << std::setfill('0') << std::setw(8) << std::hex << meminfo.Protect << "\n";
                }
                else
                {
                    // std::fprintf(stderr, "Read %u byte(s) successfully from 0x%X (byte was: 0x%X).\n", nbytes, lpMem, buf);
                }

                memsize += stepsize;

                break;
            }
            case MEM_FREE:
            {
                // std::fprintf(stderr, "Page at 0x%08X: Free (unused), skipping.\n", lpMem);
                stepsize = meminfo.RegionSize;
                break;
            }
            case MEM_RESERVE:
            {
                // std::fprintf(stderr, "Page at 0x%08X: Reserved, skipping.\n", lpMem);
                stepsize = meminfo.RegionSize;
                break;
            }
            default:
                std::cerr << "Page at 0x" << std::setfill('0') << std::setw(8) << std::hex << lpMem << ": Unknown state, panic!\n" << std::dec;
            }

            lpMem += stepsize;
        }

        std::cerr << "Done. The process has " << std::dec << memsize << " bytes (approx. "
                  << memsize / 1024 / 1024  << " MiB) of committed memory regions.\n";
    }

    CloseHandle(hProcess);
}
