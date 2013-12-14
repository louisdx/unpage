#ifndef H_CMDLINE
#define H_CMDLINE

#include <string>

class MyOptions
{
public:
    MyOptions()
    : verbose(false)
    , automatic(false)
    , listProcesses(false)
    { }

    bool verbose;
    bool automatic;
    bool listProcesses;
};

int parseOptions(int argc, char * argv[], MyOptions & options);

#endif
