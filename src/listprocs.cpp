#define  WINVER      0x0500
#define _WIN32_WINNT 0x0500
#define  PSAPI_VERSION    1
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <sddl.h>

#include <iostream>
#include "listprocs.h"


std::string getProcessNameByHandle(HANDLE hProcess)
{
  if (NULL == hProcess)
    return "<unknown>";

  TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
  HMODULE hMod;
  DWORD cbNeeded;

  if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
    ::GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));

  /*
  HANDLE tHandle;
  if (::OpenProcessToken(hProcess, TOKEN_READ, &tHandle))
  {
    DWORD retLength;
    TOKEN_OWNER tInfoOut;
    if (::GetTokenInformation(tHandle, TokenOwner, (LPVOID)&tInfoOut, sizeof(tInfoOut), &retLength))
    {
      LPTSTR s;
      ::ConvertSidToStringSid(tInfoOut.Owner, &s);
      std::cout << "Token says: " << s << std::endl;
      ::LocalFree(s);
    }
    else
    {
      std::cerr << "GetTokenInformation() failed, needed " << retLength << " bytes but provided "
                << sizeof(tInfoOut) << " bytes. Error: " << GetLastError() << std::endl;
    }
  }
  else
  {
    std::cerr << "OpenProcessToken() failed, error: " << GetLastError() << std::endl;
  }
  */

  return std::string(szProcessName);
}


std::string getProcessNameByID(DWORD processID)
{
  HANDLE      hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
  std::string result   = getProcessNameByHandle(hProcess);
  ::CloseHandle(hProcess);

  return result;
}


void ListProcesses()
{
  DWORD aProcesses[1024], cbNeeded, cProcesses;
  MEMORYSTATUSEX statex;

  statex.dwLength = sizeof(statex);

  ::GlobalMemoryStatusEx(&statex);

  ::EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
  cProcesses = cbNeeded / sizeof(DWORD);

  for (size_t i = 0; i < cProcesses; ++i)
    if(aProcesses[i] != 0)
      std::cout << "PID: " << aProcesses[i] << ", Name: " << getProcessNameByID(aProcesses[i]) << std::endl;
}
