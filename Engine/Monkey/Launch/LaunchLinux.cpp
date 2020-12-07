#include "Common/Common.h"
#include "Common/Log.h"

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

extern int32 GuardedMain(const std::vector<std::string>& cmdLine);

std::vector<std::string> g_CmdLine;

int main(int argc, char **argv) 
{
    setbuf(stdout, NULL);

    printf("test123\n");

    std::cout << "main()" << std::endl;

    for (int32 i = 0; i < argc; ++i)
    {
        g_CmdLine.push_back(argv[i]);
    }
    
    return GuardedMain(g_CmdLine);
}