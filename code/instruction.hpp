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
    u32 m_funct : 6;
    u32 m_shamt : 5;
    u32 m_rd    : 5;
    u32 m_rt    : 5;
    u32 m_rs    : 5;
    u32 m_op    : 6;
};

struct SITypeInstruction{
    u32 m_imm   : 16;
    u32 m_rt    : 5;
    u32 m_rs    : 5;
    u32 m_op    : 6;
};

struct SJTypeInstruction{
    u32 m_target : 26;
    u32 m_op     : 6;
};

struct SOpEval{
    u32 m_reserved : 26;
    u32 m_op     : 6;
};

union UInstruction{
    u32 instructionWord;
    SOpEval opEval;
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

    UInstruction get() const
    {
        return m_inst;
    }

private:
    UInstruction m_inst;
};