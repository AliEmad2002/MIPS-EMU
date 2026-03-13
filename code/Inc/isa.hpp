#pragma once

typedef std::function<i32(const UInstruction&, CProcessor&, TRam&)> tExecuter;

class CIsa{
public:
    const tExecuter& operator[](const CInstruction& instruction) const
    {
        return m_isaMap.at(instruction.get().opEval.m_op);
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
    
    const std::map< u8 /*funct*/, tExecuter > m_rTypeInstructionsMap{
        {
            0b100000, // add signed with ovf trap 'add'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                i64 result = (i64)(i32)cpu[inst.rTypeInst.m_rs] + (i64)(i32)cpu[inst.rTypeInst.m_rt];
                // todo: raise overflow exception when result > INT32_MAX || result < INT32_MIN
                cpu[inst.rTypeInst.m_rd] = (u32)(i32)result;
                return 0;
            }
        },
        {
            0b100001, // add unsigned without ovf trap 'addu'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[inst.rTypeInst.m_rs] + cpu[inst.rTypeInst.m_rt];
                return 0;
            }
        },
        {
            0b100010, // subtract signed with ovf trap 'sub'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                i64 result = (i64)(i32)cpu[inst.rTypeInst.m_rs] - (i64)(i32)cpu[inst.rTypeInst.m_rt];
                // todo: raise overflow exception when result > INT32_MAX || result < INT32_MIN
                cpu[inst.rTypeInst.m_rd] = (u32)(i32)result;
                return 0;
            }
        },
        {
            0b100011, // subtract unsigned without ovf trap 'subu'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[inst.rTypeInst.m_rs] - cpu[inst.rTypeInst.m_rt];
                return 0;
            }
        },
        {
            0b100100, // and
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[inst.rTypeInst.m_rs] & cpu[inst.rTypeInst.m_rt];
                return 0;
            }
        },
        {
            0b100101, // or
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[inst.rTypeInst.m_rs] | cpu[inst.rTypeInst.m_rt];
                return 0;
            }
        },
        {
            0b100110, // xor
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[inst.rTypeInst.m_rs] ^ cpu[inst.rTypeInst.m_rt];
                return 0;
            }
        },
        {
            0b100111, // nor
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = ~(cpu[inst.rTypeInst.m_rs] | cpu[inst.rTypeInst.m_rt]);
                return 0;
            }
        },
        {
            0b101010, // set less than signed 'slt'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = ((i32)cpu[inst.rTypeInst.m_rs] < (i32)cpu[inst.rTypeInst.m_rt]) ? 1 : 0;
                return 0;
            }
        },
        {
            0b101011, // set less than unsigned 'sltu'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = (cpu[inst.rTypeInst.m_rs] < cpu[inst.rTypeInst.m_rt]) ? 1 : 0;
                return 0;
            }
        },
        {
            0b000000, // shift left logical 'sll'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[inst.rTypeInst.m_rt] << inst.rTypeInst.m_shamt;
                return 0;
            }
        },
        {
            0b000010, // shift right logical 'srl'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[inst.rTypeInst.m_rt] >> inst.rTypeInst.m_shamt;
                return 0;
            }
        },
        {
            0b000011, // shift right arithmetic 'sra'
            /*	Casting to i32 before shifting makes C++ propagate the sign bit naturally	*/
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = (u32)((i32)cpu[inst.rTypeInst.m_rt] >> inst.rTypeInst.m_shamt);
                return 0;
            }
        },
        {
            0b000100, // shift left logical by variable 'sllv'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                /*	Only the low 5 bits of rs are used as shift amount (max 31)	*/
                u32 shamt = cpu[inst.rTypeInst.m_rs] & 0b11111;
                cpu[inst.rTypeInst.m_rd] = cpu[inst.rTypeInst.m_rt] << shamt;
                return 0;
            }
        },
        {
            0b000110, // shift right logical by variable 'srlv'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u32 shamt = cpu[inst.rTypeInst.m_rs] & 0b11111;
                cpu[inst.rTypeInst.m_rd] = cpu[inst.rTypeInst.m_rt] >> shamt;
                return 0;
            }
        },
        {
            0b000111, // shift right arithmetic by variable 'srav'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u32 shamt = cpu[inst.rTypeInst.m_rs] & 0b11111;
                cpu[inst.rTypeInst.m_rd] = (u32)((i32)cpu[inst.rTypeInst.m_rt] >> shamt);
                return 0;
            }
        },
        {
            0b011000, // multiply signed 'mult'
            /*	64-bit product stored across HI (upper 32) and LO (lower 32)	*/
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                i64 product = (i64)(i32)cpu[inst.rTypeInst.m_rs] * (i64)(i32)cpu[inst.rTypeInst.m_rt];
                cpu[REGISTER_HI] = (u32)(product >> 32);
                cpu[REGISTER_LO] = (u32)(product & 0xFFFFFFFF);
                return 0;
            }
        },
        {
            0b011001, // multiply unsigned 'multu'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u64 product = (u64)cpu[inst.rTypeInst.m_rs] * (u64)cpu[inst.rTypeInst.m_rt];
                cpu[REGISTER_HI] = (u32)(product >> 32);
                cpu[REGISTER_LO] = (u32)(product & 0xFFFFFFFF);
                return 0;
            }
        },
        {
            0b011010, // divide signed 'div'
            /*	LO = quotient, HI = remainder. Behaviour on division by zero is undefined in MIPS.	*/
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                i32 rs = (i32)cpu[inst.rTypeInst.m_rs];
                i32 rt = (i32)cpu[inst.rTypeInst.m_rt];
                if (rt != 0)
                {
                    cpu[REGISTER_LO] = (u32)(rs / rt);
                    cpu[REGISTER_HI] = (u32)(rs % rt);
                }
                return 0;
            }
        },
        {
            0b011011, // divide unsigned 'divu'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u32 rs = cpu[inst.rTypeInst.m_rs];
                u32 rt = cpu[inst.rTypeInst.m_rt];
                if (rt != 0)
                {
                    cpu[REGISTER_LO] = rs / rt;
                    cpu[REGISTER_HI] = rs % rt;
                }
                return 0;
            }
        },
        {
            0b010000, // move from HI 'mfhi'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[REGISTER_HI];
                return 0;
            }
        },
        {
            0b010010, // move from LO 'mflo'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[REGISTER_LO];
                return 0;
            }
        },
        {
            0b010001, // move to HI 'mthi'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[REGISTER_HI] = cpu[inst.rTypeInst.m_rs];
                return 0;
            }
        },
        {
            0b010011, // move to LO 'mtlo'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[REGISTER_LO] = cpu[inst.rTypeInst.m_rs];
                return 0;
            }
        },
        {
            0b001000, // jump register 'jr'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                /*	PC is incremented by the fetch loop after execute(), so we
                    *	subtract one word to land exactly at the target next cycle.	*/
                cpu[REGISTER_PC] = cpu[inst.rTypeInst.m_rs] - sizeof(u32);
                return 0;
            }
        },
        {
            0b001001, // jump and link register 'jalr'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.rTypeInst.m_rd] = cpu[REGISTER_PC] + sizeof(u32);
                cpu[REGISTER_PC] = cpu[inst.rTypeInst.m_rs] - sizeof(u32);
                return 0;
            }
        },
        {
            0b001100, // syscall
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                // todo: dispatch on $v0 to implement syscall table
                return 0;
            }
        },
        {
            0b001101, // break
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                // todo: raise breakpoint exception
                return 0;
            }
        },
    };

    const std::map< u8 /*op*/, tExecuter > m_isaMap{
        {
            0,
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                return m_rTypeInstructionsMap.at(inst.rTypeInst.m_funct)(inst, cpu, ram);
            }
        },
        {
            0b000010, // jump 'j'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[REGISTER_PC] = (cpu[REGISTER_PC] & (0b1111 << 28)) | ((u32)inst.jTypeInst.m_target << 2);
                // cpu[REGISTER_PC] -= sizeof(u32);
                return 0;
            }
        },
        {
            0b000011, // jump and link 'jal'
            // todo: here I ignored the delay slot, returning to a 'nop' and executing it is of no harm, right??
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[REGISTER_RA] = cpu[REGISTER_PC] + sizeof(u32);
                cpu[REGISTER_PC] = (cpu[REGISTER_PC] & (0b1111 << 28)) | ((u32)inst.jTypeInst.m_target << 2);
                // cpu[REGISTER_PC] -= sizeof(u32);
                return 0;
            }
        },
        {
            0b001000, // add immediate with ovf trap 'addi'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.iTypeInst.m_rt] = (u32)((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // todo: check ovf and update ovf trap accordingly
                return 0;
            }
        },
        {
            0b001001, // add immediate unsigned without ovf trap 'addiu'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.iTypeInst.m_rt] = cpu[inst.iTypeInst.m_rs] + sign_extend(inst.iTypeInst.m_imm);
                return 0;
            }
        },
        {
            0b001100, // and immediate 'andi'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.iTypeInst.m_rt] = cpu[inst.iTypeInst.m_rs] & zero_extend(inst.iTypeInst.m_imm);
                return 0;
            }
        },
        {
            0b001101, // or immediate 'ori'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.iTypeInst.m_rt] = cpu[inst.iTypeInst.m_rs] | zero_extend(inst.iTypeInst.m_imm);
                return 0;
            }
        },
        {
            0b001110, // xor immediate 'xori'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.iTypeInst.m_rt] = cpu[inst.iTypeInst.m_rs] ^ zero_extend(inst.iTypeInst.m_imm);
                return 0;
            }
        },
        {
            0b001111, // load upper immediate 'lui'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.iTypeInst.m_rt] = zero_extend(inst.iTypeInst.m_imm) << 16;
                return 0;
            }
        },
        {
            0b001010, // set less than immediate 'slti'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.iTypeInst.m_rt] = ((i32)cpu[inst.iTypeInst.m_rs] < (i32)sign_extend(inst.iTypeInst.m_imm)) ? 1 : 0;
                return 0;
            }
        },
        {
            0b001011, // set less than immediate unsigned 'sltiu'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                cpu[inst.iTypeInst.m_rt] = ((u32)cpu[inst.iTypeInst.m_rs] < (u32)sign_extend(inst.iTypeInst.m_imm)) ? 1 : 0;
                return 0;
            }
        },
        {
            0b100011, // load word 'lw'
            /*	Loads 4 bytes from RAM at byte address (rs + sign_ext(imm)) into rt	*/
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                u32 value = ((u32)ptr[0] << 24) | ((u32)ptr[1] << 16) | ((u32)ptr[2] << 8) | (u32)ptr[3];
                cpu[inst.iTypeInst.m_rt] = value;
                return 0;
            }
        },
        {
            0b100001, // load halfword signed 'lh'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                u16 raw = ((u16)ptr[0] << 8) | (u16)ptr[1];
                cpu[inst.iTypeInst.m_rt] = sign_extend(raw);
                return 0;
            }
        },
        {
            0b100101, // load halfword unsigned 'lhu'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                u16 raw = ((u16)ptr[0] << 8) | (u16)ptr[1];
                cpu[inst.iTypeInst.m_rt] = zero_extend(raw);
                return 0;
            }
        },
        {
            0b100000, // load byte signed 'lb'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                /*	Sign extend the byte by casting through i8 then i32	*/
                cpu[inst.iTypeInst.m_rt] = (u32)(i32)(i8)ptr[0];
                return 0;
            }
        },
        {
            0b100100, // load byte unsigned 'lbu'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                cpu[inst.iTypeInst.m_rt] = (u32)ptr[0];
                return 0;
            }
        },
        {
            0b101011, // store word 'sw'
            /*	Stores all 4 bytes of rt into RAM at byte address (rs + sign_ext(imm)), big-endian	*/
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                u32 value = cpu[inst.iTypeInst.m_rt];
                ptr[0] = (u8)(value >> 24);
                ptr[1] = (u8)(value >> 16);
                ptr[2] = (u8)(value >> 8);
                ptr[3] = (u8)(value);
                return 0;
            }
        },
        {
            0b101001, // store halfword 'sh'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                u32 value = cpu[inst.iTypeInst.m_rt];
                ptr[0] = (u8)(value >> 8);
                ptr[1] = (u8)(value);
                return 0;
            }
        },
        {
            0b101000, // store byte 'sb'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                ptr[0] = (u8)cpu[inst.iTypeInst.m_rt];
                return 0;
            }
        },
        {
            0b110000, // load linked 'll'
            /*	Simplified: behaves like lw. A full implementation would set a
                *	reservation bit for atomic read-modify-write support.	*/
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                u32 value = ((u32)ptr[0] << 24) | ((u32)ptr[1] << 16) | ((u32)ptr[2] << 8) | (u32)ptr[3];
                cpu[inst.iTypeInst.m_rt] = value;
                // todo: set load-linked reservation bit for this address
                return 0;
            }
        },
        {
            0b111000, // store conditional 'sc'
            /*	Simplified: always succeeds (sets rt=1 and stores).
                *	A full implementation checks the reservation bit set by ll.	*/
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                u8* ptr = &ram.at((i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm));
                // u8* ptr = ram.data() + (i32)cpu[inst.iTypeInst.m_rs] + (i32)sign_extend(inst.iTypeInst.m_imm);
                u32 value = cpu[inst.iTypeInst.m_rt];
                ptr[0] = (u8)(value >> 24);
                ptr[1] = (u8)(value >> 16);
                ptr[2] = (u8)(value >> 8);
                ptr[3] = (u8)(value);
                cpu[inst.iTypeInst.m_rt] = 1; // 1 = success
                // todo: check reservation bit; set rt=0 and skip store if it was cleared
                return 0;
            }
        },
        {
            0b000100, // branch if equal 'beq'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                if (cpu[inst.iTypeInst.m_rs] == cpu[inst.iTypeInst.m_rt])
                {
                    cpu[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
                    // cpu[REGISTER_PC] -= sizeof(u32);
                }
                return 0;
            }
        },
        {
            0b000101, // branch if not equal 'bne'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                if (cpu[inst.iTypeInst.m_rs] != cpu[inst.iTypeInst.m_rt])
                {
                    cpu[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
                    // cpu[REGISTER_PC] -= sizeof(u32);
                }
                return 0;
            }
        },
        {
            0b000110, // branch if less than or equal to zero 'blez'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                if ((i32)cpu[inst.iTypeInst.m_rs] <= 0)
                {
                    cpu[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
                    // cpu[REGISTER_PC] -= sizeof(u32);
                }
                return 0;
            }
        },
        {
            0b000111, // branch if greater than zero 'bgtz'
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                if ((i32)cpu[inst.iTypeInst.m_rs] > 0)
                {
                    cpu[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
                    // cpu[REGISTER_PC] -= sizeof(u32);
                }
                return 0;
            }
        },
        {
            0b000001, // REGIMM group: bltz, bgez, bltzal, bgezal — differentiated by rt field
            [&](const UInstruction& inst, CProcessor& cpu, TRam& ram)
            {
                if (inst.iTypeInst.m_rt == 0b00000)			// branch if less than zero 'bltz'
                {
                    if ((i32)cpu[inst.iTypeInst.m_rs] < 0)
                    {
                        cpu[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
                        // cpu[REGISTER_PC] -= sizeof(u32);
                    }
                }
                else if (inst.iTypeInst.m_rt == 0b00001)	// branch if greater than or equal to zero 'bgez'
                {
                    if ((i32)cpu[inst.iTypeInst.m_rs] >= 0)
                    {
                        cpu[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
                        // cpu[REGISTER_PC] -= sizeof(u32);
                    }
                }
                else if (inst.iTypeInst.m_rt == 0b10000)	// branch if less than zero and link 'bltzal'
                {
                    if ((i32)cpu[inst.iTypeInst.m_rs] < 0)
                    {
                        cpu[REGISTER_RA] = cpu[REGISTER_PC] + sizeof(u32);
                        cpu[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
                        // cpu[REGISTER_PC] -= sizeof(u32);
                    }
                }
                else if (inst.iTypeInst.m_rt == 0b10001)	// branch if greater than or equal to zero and link 'bgezal'
                {
                    if ((i32)cpu[inst.iTypeInst.m_rs] >= 0)
                    {
                        cpu[REGISTER_RA] = cpu[REGISTER_PC] + sizeof(u32);
                        cpu[REGISTER_PC] += (sign_extend(inst.iTypeInst.m_imm) << 2);
                        // cpu[REGISTER_PC] -= sizeof(u32);
                    }
                }
                return 0;
            }
        },
    };
};