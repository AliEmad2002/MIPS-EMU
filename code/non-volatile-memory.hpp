#pragma once


/******************************************************************************
 * Dependancy Includes
 *****************************************************************************/
#include <string>
#include <vector>
#include <fstream>
#include <array>
#include "processor.hpp"


/******************************************************************************
 * Component's Class
 *****************************************************************************/
class CNonVolatileMemory{
public:
	CNonVolatileMemory(
		const std::string& binFileName,
		const std::string& logFileName):
		m_binFile(binFileName, std::ifstream::binary),
		m_logFile(logFileName)
		{
			while(!m_binFile.eof())
			{
				std::cout << std::hex << m_binFile.get() << "\t" << std::flush;
			}
		}

	u32 operator [] (u32 address)
	{
		char readBuf[4];
		m_binFile.clear();
		m_binFile.seekg(address - 0x00400000);
		m_binFile.read(readBuf, 4);
		u32 readVal = readBuf[3];
		readVal |= readBuf[2] << 8;
		readVal |= readBuf[1] << 16;
		readVal |= readBuf[0] << 24;
		return readVal;
	}

	void logCurrentRegisterValues(const std::array<u32, REGISTER_FILE_SIZE_IN_WORDS>& regArr)
	{
		for (const u32 r: regArr)
		{
			m_logFile << r << '\t';
		}
		m_logFile << std::endl;
	}

private:
	std::ifstream m_binFile;
	std::ofstream m_logFile;
};