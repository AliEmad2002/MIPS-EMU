#include <iostream>
#include <string>
#include "mips-emu.hpp"

int main (int argc, char* argv[])
{
    // parse command line input (todo: add cmd error checking):
    std::string binFileName = "test.bin";//std::string(argv[1]);
    std::string logFileName = "log.txt";//std::string(argv[2]);
    bool isDebugLoggingEnabled = true;//( (argc > 3) && (std::string(argv[3]) == "-v") );

    // intialize the emulator
    CMipsEmulator emu(binFileName, logFileName, isDebugLoggingEnabled);

    // run and return error code
    return emu.run();
}
