#pragma once


/******************************************************************************
 * Dependancy Includes
 *****************************************************************************/
#include <string>
#include <vector>
#include <variant>


/******************************************************************************
 * Component's Class
 *****************************************************************************/
struct SRTypeInstruction{
    u8 m_op    : 6;
    u8 m_rs    : 5;
    u8 m_rt    : 5;
    u8 m_rd    : 5;
    u8 m_shamt : 5;
    u8 m_funct : 6;
};

struct SITypeInstruction{
    u8 m_op    : 6;
    u8 m_rs    : 5;
    u8 m_rt    : 5;
    u16 m_imm   : 16;
};

struct SJTypeInstruction{
    u8 m_op     : 6;
    u32 m_target : 26;
};

union UInstruction{
    u32 instructionWord;
    u8 op : 6;
    SRTypeInstruction rTypeInst;
    SITypeInstruction iTypeInst;
    SJTypeInstruction jTypeInst;
};

class CInstruction{
public:
    CInstruction(std::string asmLineStr)
    {
        // todo: parse into "m_inst"
    }

	CInstruction(u32 instructionWord)
    {
        m_inst.instructionWord = instructionWord;
    }

    UInstruction get()
    {
        return m_inst;
    }

private:
    UInstruction m_inst;
};