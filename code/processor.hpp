#pragma once


/******************************************************************************
 * Dependancy Includes
 *****************************************************************************/
#include <functional>
#include <string>
#include <vector>
#include <map>
#include "types.hpp"
#include "instruction.hpp"


/******************************************************************************
 * Component's Class
 *****************************************************************************/
enum ERegister{
	/*	General Purpose Registers (GPRs) — 32 registers, each 32-bit	*/
	REGISTER_ZERO = 0,
	REGISTER_AT = 1,
	REGISTER_V0 = 2,
	REGISTER_V1 = 3,
	REGISTER_A0 = 4,
	REGISTER_A1 = 5,
	REGISTER_A2 = 6,
	REGISTER_A3 = 7,
	REGISTER_T0 = 8,
	REGISTER_T1 = 9,
	REGISTER_T2 = 10,
	REGISTER_T3 = 11,
	REGISTER_T4 = 12,
	REGISTER_T5 = 13,
	REGISTER_T6 = 14,
	REGISTER_T7 = 15,
	REGISTER_S0 = 16,
	REGISTER_S1 = 17,
	REGISTER_S2 = 18,
	REGISTER_S3 = 19,
	REGISTER_S4 = 20,
	REGISTER_S5 = 21,
	REGISTER_S6 = 22,
	REGISTER_S7 = 23,
	REGISTER_T8 = 24,
	REGISTER_T9 = 25,
	REGISTER_K0 = 26,
	REGISTER_K1 = 27,
	REGISTER_GP = 28,
	REGISTER_SP = 29,
	REGISTER_FP = 30,
	REGISTER_RA = 31,

	/*	Special-Purpose Registers	*/
	REGISTER_PC = 32,
	REGISTER_HI = 34,
	REGISTER_LO = 35,

	/*	TODO: Coprocessor 0 (CP0) — Privileged / Exception Handling Registers*/

	/*	Size of the register file (In words)	*/
	REGISTER_FILE_SIZE_IN_WORDS,
};

class CProcessor{
public:
	CProcessor()
	{
		/*	Reset all registers	*/
		m_regArr.fill(0);

		/* Special startup value for program count register	*/
		m_regArr[REGISTER_PC] = 0x00400000;
	}

	i32 execute(CInstruction& instruction)
	{
		return m_isaMap.at(instruction.get().op)(instruction.get());
	}

	u32& operator [] (u32 registerIndex)
	{
		return m_regArr[registerIndex];
	}

	std::array<u32, REGISTER_FILE_SIZE_IN_WORDS>& getRegisterArray()
	{
		return m_regArr;
	}

private:
	std::array<u32, REGISTER_FILE_SIZE_IN_WORDS> m_regArr;
	
	const std::map< u8 /*op*/, std::function<i32(UInstruction)> > m_isaMap{
		{
			0,
			[&](UInstruction inst)
			{
				// R-type: todo
				return 0;
			}
		},
		{
			0b000010, // jump 'j'
			[&](UInstruction inst)
			{
				m_regArr[REGISTER_PC] = (m_regArr[REGISTER_PC] & (0b1111 << 28)) | (inst.jTypeInst.m_target << 2);
				m_regArr[REGISTER_PC] -= sizeof(u32);
				return 0;
			}
		},
		{
			0b000011, // jump and link 'jal'
			[&](UInstruction inst)
			{
				m_regArr[REGISTER_RA] = m_regArr[REGISTER_PC] + sizeof(u32);
				m_regArr[REGISTER_PC] = inst.jTypeInst.m_target;
				return 0;
			}
		},
		{
			0b001001, // add immediate unsigned 'addiu'
			[&](UInstruction inst)
			{
				m_regArr[inst.iTypeInst.m_rt] = m_regArr[inst.iTypeInst.m_rs] + inst.iTypeInst.m_imm;
				return 0;
			}
		},
	};
};