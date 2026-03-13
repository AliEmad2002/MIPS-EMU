#pragma once


/******************************************************************************
 * Component's Class
 *****************************************************************************/
class CNvm{
public:
	CNvm(
		const std::string& binFileName,
		const std::string& logFileName):
		m_binFile(binFileName, std::ifstream::binary),
		m_logFile(logFileName)	{}

	u32 operator [] (u32 address);

	void logCurrentRegisterValues(const TRegArr& regArr);

private:
	std::ifstream m_binFile;
	std::ofstream m_logFile;
};