#pragma once


/******************************************************************************
 * Component's Class
 *****************************************************************************/

class CEmulator{
public:
    CEmulator(
        const std::string& binFileName,
        const std::string& logFileName,
        const bool isDebugLoggingEnabled):
    m_nvm(binFileName, logFileName),
    m_isDebugLoggingEnabled(isDebugLoggingEnabled)
    {
        //  Reset RAM:
        m_ram.fill(0);
    }

    i32 run();

private:
    const CIsa m_isa;
	CProcessor m_cpu;
	TRam m_ram;
    CNvm m_nvm;	
	bool m_isDebugLoggingEnabled;
};