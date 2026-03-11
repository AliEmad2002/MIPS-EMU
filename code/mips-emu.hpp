#pragma once

/******************************************************************************
 * Dependancy Includes
 *****************************************************************************/
#include "types.hpp"
#include <string>
#include "processor.hpp"
#include "non-volatile-memory.hpp"
#include "instruction.hpp"


/******************************************************************************
 * Component's Class
 *****************************************************************************/
class CMipsEmulator{

public:
    CMipsEmulator(
        const std::string& binFileName,
        const std::string& logFileName,
        const bool isDebugLoggingEnabled):
    m_nvm(binFileName, logFileName),
    m_isDebugLoggingEnabled(isDebugLoggingEnabled)
    {
        //  Reset RAM:
        m_ram.fill(0);
    }

    i32 run()
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
                // todo
            }

            // execute the instruction
            m_cpu.execute(instruction);
            
            //  increment program count one word size (i.e.: one instruction size)
            m_cpu[REGISTER_PC] += sizeof(u32);

            // log resiters status
            m_nvm.logCurrentRegisterValues(m_cpu.getRegisterArray());
        }

        return 0;
    }


private:
	CProcessor m_cpu;
	CNonVolatileMemory m_nvm;
	std::array<u32, 4096> m_ram;
	bool m_isDebugLoggingEnabled;
};