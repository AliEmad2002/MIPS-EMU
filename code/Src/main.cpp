#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <functional>
#include <map>
#include "configuration.hpp"
#include "types.hpp"
#include "instruction.hpp"
#include "processor.hpp"
#include "isa.hpp"
#include "nvm.hpp"
#include "assembler.hpp"
#include "emu.hpp"

int main (int argc, char* argv[])
{
    // parse command line input (todo: add cmd error checking):
    std::string binFileName = std::string(argv[1]);
    std::string logFileName = std::string(argv[2]);
    bool isDebugLoggingEnabled = ( (argc > 3) && (std::string(argv[3]) == "-v") );

    // intialize the emulator
    CEmulator emu(binFileName, logFileName, isDebugLoggingEnabled);

    // run and return error code
    return emu.run();
}
