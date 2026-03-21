/******************************************************************************
 * Dependancy Includes
 *****************************************************************************/
#include <functional>
#include <string>
#include <vector>
#include <map>
#include "configuration.hpp"
#include "types.hpp"
#include "instruction.hpp"
#include "processor.hpp"
#include "isa.hpp"


/******************************************************************************
 * Component's Class
 *****************************************************************************/
CProcessor::CProcessor()
{
	/*	Reset all registers	*/
	m_regArr.fill(0);

	/* Special startup value for program count register	*/
	m_regArr[REGISTER_PC] = 0x00400130;

	/* Special startup value for stack pointer register (starts from end of RAM)	*/
	m_regArr[REGISTER_SP] = uiRAM_SIZE_IN_BYTES - sizeof(u32);
}

u32& CProcessor::operator [] (u32 registerIndex)
{
	// ignore writes on the "$zero" register:
	if (registerIndex == REGISTER_ZERO)
	{
		static u32 dummyReg;
		dummyReg = 0;
		return dummyReg;
	}

	// otherwise, return the requested register
	return m_regArr[registerIndex];
}