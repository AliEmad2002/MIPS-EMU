#pragma once


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
	REGISTER_S8_FP = 30, // stack frame pointer is named 's8'
	REGISTER_RA = 31,

	/*	Special-Purpose Registers	*/
	REGISTER_PC = 32,
	REGISTER_HI = 33,
	REGISTER_LO = 34,

	/*	TODO: Coprocessor 0 (CP0) — Privileged / Exception Handling Registers	*/

	/*	Size of the register file (In words)	*/
	REGISTER_FILE_SIZE_IN_WORDS,
};

typedef std::array<u32, REGISTER_FILE_SIZE_IN_WORDS> TRegArr;

class CProcessor{
public:
	CProcessor();

	u32& operator [] (u32 registerIndex);

	TRegArr& getRegisterArray()	{	return m_regArr;	}

private:
	TRegArr m_regArr;
};
