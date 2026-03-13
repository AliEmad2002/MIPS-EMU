/******************************************************************************
 * Dependancy Includes
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <array>
#include <functional>
#include <map>
#include "configuration.hpp"
#include "types.hpp"
#include "instruction.hpp"
#include "processor.hpp"
#include "nvm.hpp"
#include "isa.hpp"
#include "emu.hpp"


/******************************************************************************
 * Component's Class
 *****************************************************************************/
i32 CEmulator::run()
{
    while(1)
    {
        //  Fetch istruction word at: binary[program count register]
        u32 instructionWord = m_nvm[m_cpu[REGISTER_PC]];

        // parse binary word into an instruction object
        CInstruction instruction(instructionWord);

        // debugging print / log (if enabled)
        if (m_isDebugLoggingEnabled)
        {
            std::cout << "executed instruction: " << std::hex << instructionWord << std::endl;
        }

        // execute the instruction
        m_isa[instruction](instruction.get(), m_cpu, m_ram);
        
        //  increment program count one word size (i.e.: one instruction size)
        m_cpu[REGISTER_PC] += sizeof(u32);

        // log resiters status
        m_nvm.logCurrentRegisterValues(m_cpu.getRegisterArray());
    }

    return 0;
}