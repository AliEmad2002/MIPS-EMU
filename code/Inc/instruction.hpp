#pragma once


/******************************************************************************
 * Dependancy Includes
 *****************************************************************************/

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
        // std::stringstream ss(asmLineStr);

        // /*  Get operation string (ex.: "add", "nor", ..etc)*/
        // std::string operation;
        // std::getline(ss, operation, ' ');

        // /*  Get 'op' and 'funct' that co-relate to the operation    */

        // /*  if 'op' expresses an R-type instruction */
        // if (1)
        // {
        //     /*  Get values of 'rd', 'rt', 'rs'  */

        // }
        // /*  otherwise, if 'op' expresses an I-type instruction */
        // else if (1)
        // {
        //     /*  Get values of 'rt', 'rs', 'imm'  */

        // }
        // /*  otherwise, if 'op' expresses a J-type instruction */
        // else if (1)
        // {
        //     /*  Get value of 'target'  */

        // }
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