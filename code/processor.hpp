#pragma once


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

		/* Special startup value for stack pointer register (starts from end of RAM)	*/
		m_regArr[REGISTER_SP] = uiRAM_SIZE_IN_BYTES-1;
	}

	i32 execute(const CInstruction& instruction, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
	{
		i32 ret = m_isaMap.at(instruction.get().opEval.m_op)(instruction.get(), ram);
		// static int i = 0;
		// std::cout << "executed " << i++ << "-th instruction: " << std::hex << instruction.get().instructionWord << std::endl;
		return ret;
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
	u32 zero_extend(u16 imm)
	{
		/*	Using dynamic casting to fill the upper 16-bits w/ zeros	*/
		return (u32)imm;
	}

	u32 sign_extend(u16 imm)
	{
		/*	Using dynamic casting to fill the upper 16-bits w/ value of the MSB of "imm" (i.e.: bit #15)	*/
		return (u32)((i32)((i16)imm));
	}

	std::array<u32, REGISTER_FILE_SIZE_IN_WORDS> m_regArr;

	const std::map< u8 /*funct*/, std::function<i32(const UInstruction&, std::array<u8, uiRAM_SIZE_IN_BYTES>&)> > m_rTypeInstructionsMap{
		{
			0b100000, // add signed with ovf trap 'add'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				i64 result = (i64)(i32)m_regArr[inst.rTypeInst.m_rs] + (i64)(i32)m_regArr[inst.rTypeInst.m_rt];
				// todo: raise overflow exception when result > INT32_MAX || result < INT32_MIN
				m_regArr[inst.rTypeInst.m_rd] = (u32)(i32)result;
				return 0;
			}
		},
		{
			0b100001, // add unsigned without ovf trap 'addu'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] + m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100010, // subtract signed with ovf trap 'sub'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				i64 result = (i64)(i32)m_regArr[inst.rTypeInst.m_rs] - (i64)(i32)m_regArr[inst.rTypeInst.m_rt];
				// todo: raise overflow exception when result > INT32_MAX || result < INT32_MIN
				m_regArr[inst.rTypeInst.m_rd] = (u32)(i32)result;
				return 0;
			}
		},
		{
			0b100011, // subtract unsigned without ovf trap 'subu'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] - m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100100, // and
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] & m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100101, // or
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] | m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100110, // xor
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] ^ m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100111, // nor
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = ~(m_regArr[inst.rTypeInst.m_rs] | m_regArr[inst.rTypeInst.m_rt]);
				return 0;
			}
		},
		{
			0b101010, // set less than signed 'slt'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = ((i32)m_regArr[inst.rTypeInst.m_rs] < (i32)m_regArr[inst.rTypeInst.m_rt]) ? 1 : 0;
				return 0;
			}
		},
		{
			0b101011, // set less than unsigned 'sltu'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = (m_regArr[inst.rTypeInst.m_rs] < m_regArr[inst.rTypeInst.m_rt]) ? 1 : 0;
				return 0;
			}
		},
		{
			0b000000, // shift left logical 'sll'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rt] << inst.rTypeInst.m_shamt;
				return 0;
			}
		},
		{
			0b000010, // shift right logical 'srl'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rt] >> inst.rTypeInst.m_shamt;
				return 0;
			}
		},
		{
			0b000011, // shift right arithmetic 'sra'
			/*	Casting to i32 before shifting makes C++ propagate the sign bit naturally	*/
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = (u32)((i32)m_regArr[inst.rTypeInst.m_rt] >> inst.rTypeInst.m_shamt);
				return 0;
			}
		},
		{
			0b000100, // shift left logical by variable 'sllv'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				/*	Only the low 5 bits of rs are used as shift amount (max 31)	*/
				u32 shamt = m_regArr[inst.rTypeInst.m_rs] & 0b11111;
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rt] << shamt;
				return 0;
			}
		},
		{
			0b000110, // shift right logical by variable 'srlv'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u32 shamt = m_regArr[inst.rTypeInst.m_rs] & 0b11111;
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rt] >> shamt;
				return 0;
			}
		},
		{
			0b000111, // shift right arithmetic by variable 'srav'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u32 shamt = m_regArr[inst.rTypeInst.m_rs] & 0b11111;
				m_regArr[inst.rTypeInst.m_rd] = (u32)((i32)m_regArr[inst.rTypeInst.m_rt] >> shamt);
				return 0;
			}
		},
		{
			0b011000, // multiply signed 'mult'
			/*	64-bit product stored across HI (upper 32) and LO (lower 32)	*/
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				i64 product = (i64)(i32)m_regArr[inst.rTypeInst.m_rs] * (i64)(i32)m_regArr[inst.rTypeInst.m_rt];
				m_regArr[REGISTER_HI] = (u32)(product >> 32);
				m_regArr[REGISTER_LO] = (u32)(product & 0xFFFFFFFF);
				return 0;
			}
		},
		{
			0b011001, // multiply unsigned 'multu'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u64 product = (u64)m_regArr[inst.rTypeInst.m_rs] * (u64)m_regArr[inst.rTypeInst.m_rt];
				m_regArr[REGISTER_HI] = (u32)(product >> 32);
				m_regArr[REGISTER_LO] = (u32)(product & 0xFFFFFFFF);
				return 0;
			}
		},
		{
			0b011010, // divide signed 'div'
			/*	LO = quotient, HI = remainder. Behaviour on division by zero is undefined in MIPS.	*/
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				i32 rs = (i32)m_regArr[inst.rTypeInst.m_rs];
				i32 rt = (i32)m_regArr[inst.rTypeInst.m_rt];
				if (rt != 0)
				{
					m_regArr[REGISTER_LO] = (u32)(rs / rt);
					m_regArr[REGISTER_HI] = (u32)(rs % rt);
				}
				return 0;
			}
		},
		{
			0b011011, // divide unsigned 'divu'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u32 rs = m_regArr[inst.rTypeInst.m_rs];
				u32 rt = m_regArr[inst.rTypeInst.m_rt];
				if (rt != 0)
				{
					m_regArr[REGISTER_LO] = rs / rt;
					m_regArr[REGISTER_HI] = rs % rt;
				}
				return 0;
			}
		},
		{
			0b010000, // move from HI 'mfhi'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[REGISTER_HI];
				return 0;
			}
		},
		{
			0b010010, // move from LO 'mflo'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[REGISTER_LO];
				return 0;
			}
		},
		{
			0b010001, // move to HI 'mthi'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[REGISTER_HI] = m_regArr[inst.rTypeInst.m_rs];
				return 0;
			}
		},
		{
			0b010011, // move to LO 'mtlo'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[REGISTER_LO] = m_regArr[inst.rTypeInst.m_rs];
				return 0;
			}
		},
		{
			0b001000, // jump register 'jr'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				/*	PC is incremented by the fetch loop after execute(), so we
				 *	subtract one word to land exactly at the target next cycle.	*/
				m_regArr[REGISTER_PC] = m_regArr[inst.rTypeInst.m_rs] - sizeof(u32);
				return 0;
			}
		},
		{
			0b001001, // jump and link register 'jalr'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[REGISTER_PC] + sizeof(u32);
				m_regArr[REGISTER_PC] = m_regArr[inst.rTypeInst.m_rs] - sizeof(u32);
				return 0;
			}
		},
		{
			0b001100, // syscall
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				// todo: dispatch on $v0 to implement syscall table
				return 0;
			}
		},
		{
			0b001101, // break
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				// todo: raise breakpoint exception
				return 0;
			}
		},
	};

	const std::map< u8 /*op*/, std::function<i32(const UInstruction&, std::array<u8, uiRAM_SIZE_IN_BYTES>&)> > m_isaMap{
		{
			0,
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				return m_rTypeInstructionsMap.at(inst.rTypeInst.m_funct)(inst, ram);
			}
		},
		{
			0b000010, // jump 'j'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[REGISTER_PC] = (m_regArr[REGISTER_PC] & (0b1111 << 28)) | ((u32)inst.jTypeInst.m_target << 2);
				// m_regArr[REGISTER_PC] -= sizeof(u32);
				return 0;
			}
		},
		{
			0b000011, // jump and link 'jal'
			// todo: here I ignored the delay slot, returning to a 'nop' and executing it is of no harm, right??
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[REGISTER_RA] = m_regArr[REGISTER_PC] + sizeof(u32);
				m_regArr[REGISTER_PC] = (m_regArr[REGISTER_PC] & (0b1111 << 28)) | ((u32)inst.jTypeInst.m_target << 2);
				// m_regArr[REGISTER_PC] -= sizeof(u32);
				return 0;
			}
		},
		{
			0b001000, // add immediate with ovf trap 'addi'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = (u32)((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// todo: check ovf and update ovf trap accordingly
				return 0;
			}
		},
		{
			0b001001, // add immediate unsigned without ovf trap 'addiu'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = m_regArr[inst.iTypeInst.m_rs] + sign_extend(inst.iTypeInst.m_imm);
				return 0;
			}
		},
		{
			0b001100, // and immediate 'andi'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = m_regArr[inst.iTypeInst.m_rs] & zero_extend(inst.iTypeInst.m_imm);
				return 0;
			}
		},
		{
			0b001101, // or immediate 'ori'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = m_regArr[inst.iTypeInst.m_rs] | zero_extend(inst.iTypeInst.m_imm);
				return 0;
			}
		},
		{
			0b001110, // xor immediate 'xori'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = m_regArr[inst.iTypeInst.m_rs] ^ zero_extend(inst.iTypeInst.m_imm);
				return 0;
			}
		},
		{
			0b001111, // load upper immediate 'lui'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = zero_extend(inst.iTypeInst.m_imm) << 16;
				return 0;
			}
		},
		{
			0b001010, // set less than immediate 'slti'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = ((i32)m_regArr[inst.iTypeInst.m_rs] < (i32)sign_extend(inst.iTypeInst.m_imm)) ? 1 : 0;
				return 0;
			}
		},
		{
			0b001011, // set less than immediate unsigned 'sltiu'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = ((u32)m_regArr[inst.iTypeInst.m_rs] < (u32)sign_extend(inst.iTypeInst.m_imm)) ? 1 : 0;
				return 0;
			}
		},
		{
			0b100011, // load word 'lw'
			/*	Loads 4 bytes from RAM at byte address (rs + sign_ext(imm)) into rt	*/
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				u32 value = ((u32)ptr[0] << 24) | ((u32)ptr[1] << 16) | ((u32)ptr[2] << 8) | (u32)ptr[3];
				m_regArr[inst.iTypeInst.m_rt] = value;
				return 0;
			}
		},
		{
			0b100001, // load halfword signed 'lh'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				u16 raw = ((u16)ptr[0] << 8) | (u16)ptr[1];
				m_regArr[inst.iTypeInst.m_rt] = sign_extend(raw);
				return 0;
			}
		},
		{
			0b100101, // load halfword unsigned 'lhu'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				u16 raw = ((u16)ptr[0] << 8) | (u16)ptr[1];
				m_regArr[inst.iTypeInst.m_rt] = zero_extend(raw);
				return 0;
			}
		},
		{
			0b100000, // load byte signed 'lb'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				/*	Sign extend the byte by casting through i8 then i32	*/
				m_regArr[inst.iTypeInst.m_rt] = (u32)(i32)(i8)ptr[0];
				return 0;
			}
		},
		{
			0b100100, // load byte unsigned 'lbu'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				m_regArr[inst.iTypeInst.m_rt] = (u32)ptr[0];
				return 0;
			}
		},
		{
			0b101011, // store word 'sw'
			/*	Stores all 4 bytes of rt into RAM at byte address (rs + sign_ext(imm)), big-endian	*/
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				u32 value = m_regArr[inst.iTypeInst.m_rt];
				ptr[0] = (u8)(value >> 24);
				ptr[1] = (u8)(value >> 16);
				ptr[2] = (u8)(value >> 8);
				ptr[3] = (u8)(value);
				return 0;
			}
		},
		{
			0b101001, // store halfword 'sh'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				u32 value = m_regArr[inst.iTypeInst.m_rt];
				ptr[0] = (u8)(value >> 8);
				ptr[1] = (u8)(value);
				return 0;
			}
		},
		{
			0b101000, // store byte 'sb'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				ptr[0] = (u8)m_regArr[inst.iTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b110000, // load linked 'll'
			/*	Simplified: behaves like lw. A full implementation would set a
			 *	reservation bit for atomic read-modify-write support.	*/
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				u32 value = ((u32)ptr[0] << 24) | ((u32)ptr[1] << 16) | ((u32)ptr[2] << 8) | (u32)ptr[3];
				m_regArr[inst.iTypeInst.m_rt] = value;
				// todo: set load-linked reservation bit for this address
				return 0;
			}
		},
		{
			0b111000, // store conditional 'sc'
			/*	Simplified: always succeeds (sets rt=1 and stores).
			 *	A full implementation checks the reservation bit set by ll.	*/
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				u8* ptr = &ram.at((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// u8* ptr = ram.data() + (i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
				u32 value = m_regArr[inst.iTypeInst.m_rt];
				ptr[0] = (u8)(value >> 24);
				ptr[1] = (u8)(value >> 16);
				ptr[2] = (u8)(value >> 8);
				ptr[3] = (u8)(value);
				m_regArr[inst.iTypeInst.m_rt] = 1; // 1 = success
				// todo: check reservation bit; set rt=0 and skip store if it was cleared
				return 0;
			}
		},
		{
			0b000100, // branch if equal 'beq'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				if (m_regArr[inst.iTypeInst.m_rs] == m_regArr[inst.iTypeInst.m_rt])
				{
					m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
					// m_regArr[REGISTER_PC] -= sizeof(u32);
				}
				return 0;
			}
		},
		{
			0b000101, // branch if not equal 'bne'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				if (m_regArr[inst.iTypeInst.m_rs] != m_regArr[inst.iTypeInst.m_rt])
				{
					m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
					// m_regArr[REGISTER_PC] -= sizeof(u32);
				}
				return 0;
			}
		},
		{
			0b000110, // branch if less than or equal to zero 'blez'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				if ((i32)m_regArr[inst.iTypeInst.m_rs] <= 0)
				{
					m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
					// m_regArr[REGISTER_PC] -= sizeof(u32);
				}
				return 0;
			}
		},
		{
			0b000111, // branch if greater than zero 'bgtz'
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				if ((i32)m_regArr[inst.iTypeInst.m_rs] > 0)
				{
					m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
					// m_regArr[REGISTER_PC] -= sizeof(u32);
				}
				return 0;
			}
		},
		{
			0b000001, // REGIMM group: bltz, bgez, bltzal, bgezal — differentiated by rt field
			[&](UInstruction inst, std::array<u8, uiRAM_SIZE_IN_BYTES>& ram)
			{
				if (inst.iTypeInst.m_rt == 0b00000)			// branch if less than zero 'bltz'
				{
					if ((i32)m_regArr[inst.iTypeInst.m_rs] < 0)
					{
						m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
						// m_regArr[REGISTER_PC] -= sizeof(u32);
					}
				}
				else if (inst.iTypeInst.m_rt == 0b00001)	// branch if greater than or equal to zero 'bgez'
				{
					if ((i32)m_regArr[inst.iTypeInst.m_rs] >= 0)
					{
						m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
						// m_regArr[REGISTER_PC] -= sizeof(u32);
					}
				}
				else if (inst.iTypeInst.m_rt == 0b10000)	// branch if less than zero and link 'bltzal'
				{
					if ((i32)m_regArr[inst.iTypeInst.m_rs] < 0)
					{
						m_regArr[REGISTER_RA] = m_regArr[REGISTER_PC] + sizeof(u32);
						m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
						// m_regArr[REGISTER_PC] -= sizeof(u32);
					}
				}
				else if (inst.iTypeInst.m_rt == 0b10001)	// branch if greater than or equal to zero and link 'bgezal'
				{
					if ((i32)m_regArr[inst.iTypeInst.m_rs] >= 0)
					{
						m_regArr[REGISTER_RA] = m_regArr[REGISTER_PC] + sizeof(u32);
						m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
						// m_regArr[REGISTER_PC] -= sizeof(u32);
					}
				}
				return 0;
			}
		},
	};
};
