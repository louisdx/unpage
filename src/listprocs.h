#ifndef H_LISTPROCS
#define H_LISTPROCS

#include <string>

std::string getProcessNameByHandle(HANDLE hProcess);
std::string getProcessNameByID(DWORD processID);
void ListProcesses();

#endif
