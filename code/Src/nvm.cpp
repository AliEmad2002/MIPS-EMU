/******************************************************************************
 * Dependancy Includes
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <functional>
#include <map>
#include "configuration.hpp"
#include "types.hpp"
#include "processor.hpp"
#include "nvm.hpp"


/******************************************************************************
 * Component's Class
 *****************************************************************************/
u32 CNvm::operator [] (u32 address)
{
	u8 readBuf[4];
	m_binFile.clear();
	m_binFile.seekg(address - 0x00400000);
	m_binFile.read((char*)readBuf, 4);
	u32 readVal = readBuf[3];
	readVal |= (u32)readBuf[2] << 8;
	readVal |= (u32)readBuf[1] << 16;
	readVal |= (u32)readBuf[0] << 24;
	return readVal;
}

void CNvm::logCurrentRegisterValues(const std::array<u32, REGISTER_FILE_SIZE_IN_WORDS>& regArr)
{
	for (const u32 r: regArr)
	{
		m_logFile << "0x" << std::hex << r << '\t';
	}
	m_logFile << std::endl;
}