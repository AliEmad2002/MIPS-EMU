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

	i32 execute(const CInstruction& instruction, std::array<u8, 4096>& ram)
	{
		i32 ret = m_isaMap.at(instruction.get().opEval.m_op)(instruction.get(), ram);
		static int i = 0;
		std::cout << "executed " << i++ << "-th instruction" << std::endl;
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
		return (u32)(	(i32)( (i16)imm)	);
	}

	std::array<u32, REGISTER_FILE_SIZE_IN_WORDS> m_regArr;
	
	const std::map< u8 /*funct*/, std::function<i32(const UInstruction&, std::array<u8, 4096>&)> > m_rTypeInstructionsMap{
		{
			0b100000, // add signed with ovf trap 'add'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = (u32)((i32)m_regArr[inst.rTypeInst.m_rs] + (i32)m_regArr[inst.rTypeInst.m_rt]);
				// todo: check ovf and update ovf trap accordingly
				return 0;
			}
		},
		{
			0b100001, // add unsigned without ovf trap 'addu'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] + m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100010, // subtract signed with ovf trap 'sub'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = (u32)((i32)m_regArr[inst.rTypeInst.m_rs] - (i32)m_regArr[inst.rTypeInst.m_rt]);
				// todo: check ovf and update ovf trap accordingly
				return 0;
			}
		},
		{
			0b100011, // subtract unsigned without ovf trap 'subu'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] - m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100100, // and
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] & m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100101, // or
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] | m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100110, // xor
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = m_regArr[inst.rTypeInst.m_rs] ^ m_regArr[inst.rTypeInst.m_rt];
				return 0;
			}
		},
		{
			0b100111, // nor
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = ~(m_regArr[inst.rTypeInst.m_rs] | m_regArr[inst.rTypeInst.m_rt]);
				return 0;
			}
		},
		{
			0b000000, // shift left logical 'sll'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = (m_regArr[inst.rTypeInst.m_rt] << inst.rTypeInst.m_shamt);
				return 0;
			}
		},
		{
			0b000010, // shift right logical 'srl'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.rTypeInst.m_rd] = (m_regArr[inst.rTypeInst.m_rt] >> inst.rTypeInst.m_shamt);
				return 0;
			}
		},
		{
			0b000011, // shift right arithmetic 'sra'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				i32 rt = (i32)m_regArr[inst.rTypeInst.m_rt];
				if (rt < 0)
				{
					// todo: there's absloutly a better syntax here!
					m_regArr[inst.rTypeInst.m_rd] = (u32)(-(i32)(((u32)(-rt)) >> inst.rTypeInst.m_shamt));
				}
				else
				{
					m_regArr[inst.rTypeInst.m_rd] = (((u32)(rt)) >> inst.rTypeInst.m_shamt);
				}
				return 0;
			}
		},
		{
			0b000100, // shift left logical by variable 'sllv'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				u32 shamt = m_regArr[inst.rTypeInst.m_rs] & 0b1111;
				m_regArr[inst.rTypeInst.m_rd] = (m_regArr[inst.rTypeInst.m_rt] << shamt);
				return 0;
			}
		},
		{
			0b000110, // shift right logical by variable 'srlv'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				u32 shamt = m_regArr[inst.rTypeInst.m_rs] & 0b1111;
				m_regArr[inst.rTypeInst.m_rd] = (m_regArr[inst.rTypeInst.m_rt] >> shamt);
				return 0;
			}
		},
		{
			0b000111, // shift right arithmetic by variable 'srav'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				u32 shamt = m_regArr[inst.rTypeInst.m_rs] & 0b1111;
				i32 rt = (i32)m_regArr[inst.rTypeInst.m_rt];
				if (rt < 0)
				{
					// todo: there's absloutly a better syntax here!
					m_regArr[inst.rTypeInst.m_rd] = (u32)(-(i32)(((u32)(-rt)) >> shamt));
				}
				else
				{
					m_regArr[inst.rTypeInst.m_rd] = (((u32)(rt)) >> shamt);
				}
				return 0;
			}
		},
	};

	const std::map< u8 /*op*/, std::function<i32(const UInstruction&, std::array<u8, 4096>&)> > m_isaMap{
		{
			0,
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				return m_rTypeInstructionsMap.at(inst.rTypeInst.m_funct)(inst, ram);
			}
		},
		{
			0b000010, // jump 'j'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[REGISTER_PC] = (m_regArr[REGISTER_PC] & (0b1111 << 28)) | ((u32)inst.jTypeInst.m_target << 2);
				m_regArr[REGISTER_PC] -= sizeof(u32);
				return 0;
			}
		},
		{
			0b000011, // jump and link 'jal'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[REGISTER_RA] = m_regArr[REGISTER_PC] + sizeof(u32);
				m_regArr[REGISTER_PC] = inst.jTypeInst.m_target;
				return 0;
			}
		},
		{
			0b001000, // add immediate with ovf trap 'addi'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = (u32)((i32)m_regArr[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
				// todo: check ovf and update ovf trap accordingly
				return 0;
			}
		},
		{
			0b001001, // add immediate unsigned without ovf trap 'addiu'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = m_regArr[inst.iTypeInst.m_rs] + sign_extend(inst.iTypeInst.m_imm);
				return 0;
			}
		},
		{
			0b001100, // and immediate 'andi'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = m_regArr[inst.iTypeInst.m_rs] & zero_extend(inst.iTypeInst.m_imm);
				return 0;
			}
		},
		{
			0b001101, // or immediate 'ori'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = m_regArr[inst.iTypeInst.m_rs] | zero_extend(inst.iTypeInst.m_imm);
				return 0;
			}
		},
		{
			0b001110, // xor immediate 'xori'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = m_regArr[inst.iTypeInst.m_rs] ^ zero_extend(inst.iTypeInst.m_imm);
				return 0;
			}
		},
		{
			0b001111, // load upper immediate 'lui'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = zero_extend(inst.iTypeInst.m_imm) << 16;
				return 0;
			}
		},
		{
			0b001010, // set less than immediate 'slti'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = ((i32)m_regArr[inst.iTypeInst.m_rs] < (i32)sign_extend(inst.iTypeInst.m_imm)) ? 1 : 0;
				return 0;
			}
		},
		{
			0b001011, // set less than immediate unsigned 'sltiu'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				m_regArr[inst.iTypeInst.m_rt] = ((u32)m_regArr[inst.iTypeInst.m_rs] < (u32)sign_extend(inst.iTypeInst.m_imm)) ? 1 : 0;
				return 0;
			}
		},
		{
			0b000100, // branch if equal 'beq'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				if (m_regArr[inst.iTypeInst.m_rs] == m_regArr[inst.iTypeInst.m_rt])
				{
					m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
					m_regArr[REGISTER_PC] -= sizeof(u32);
				}
				return 0;
			}
		},
		{
			0b000101, // branch if not equal 'bne'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				if (m_regArr[inst.iTypeInst.m_rs] != m_regArr[inst.iTypeInst.m_rt])
				{
					m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
					m_regArr[REGISTER_PC] -= sizeof(u32);
				}
				return 0;
			}
		},
		{
			0b000110, // branch if less than or equal to zero 'blez'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				if ((i32)m_regArr[inst.iTypeInst.m_rs] <= 0)
				{
					m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
					m_regArr[REGISTER_PC] -= sizeof(u32);
				}
				return 0;
			}
		},
		{
			0b000111, // branch if greater than zero 'bgtz'
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				if ((i32)m_regArr[inst.iTypeInst.m_rs] > 0)
				{
					m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
					m_regArr[REGISTER_PC] -= sizeof(u32);
				}
				return 0;
			}
		},
		{
			0b000001, // combination of branch instructions, specified by 'rt' value. todo: this is strange and lengthy and hence hart to maintain, find a better approach!
			[&](UInstruction inst, std::array<u8, 4096>& ram)
			{
				if (inst.iTypeInst.m_rt == 0b00000)	// branch if less than zero 'bltz'
				{
					if ((i32)m_regArr[inst.iTypeInst.m_rs] < 0)
					{
						m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
						m_regArr[REGISTER_PC] -= sizeof(u32);
					}
				}
				else if (inst.iTypeInst.m_rt == 0b00001)	// branch if greater than or equal to zero 'bgez'
				{
					if ((i32)m_regArr[inst.iTypeInst.m_rs] >= 0)
					{
						m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
						m_regArr[REGISTER_PC] -= sizeof(u32);
					}
				}
				else if (inst.iTypeInst.m_rt == 0b10000)	// branch if less than zero and link 'bltzal'
				{
					if ((i32)m_regArr[inst.iTypeInst.m_rs] < 0)
					{
						m_regArr[REGISTER_RA] = m_regArr[REGISTER_PC] + sizeof(u32);
						m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
						m_regArr[REGISTER_PC] -= sizeof(u32);
					}
				}
				else if (inst.iTypeInst.m_rt == 0b10001)	// branch if greater than or equal to zero and link 'bgezal'
				{
					if ((i32)m_regArr[inst.iTypeInst.m_rs] >= 0)
					{
						m_regArr[REGISTER_RA] = m_regArr[REGISTER_PC] + sizeof(u32);
						m_regArr[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
						m_regArr[REGISTER_PC] -= sizeof(u32);
					}
				}
				return 0;
			}
		},		
	};
};