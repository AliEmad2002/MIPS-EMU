#pragma once

class CAssembler{
public:
    CAssembler(std::string asmFileName);


private:
    std::ifstream m_asmFile;
    std::ofstream m_binFile;
    const std::map<std::string, std::pair<char, u8>> instructionTypeOpFunctMap{
        /*  {Intruction ASCII, {Instruction Type, Instruction 'op' or 'funct' value}}   */
        { "add",    { 'R', 0b100000 } },
        { "addu",   { 'R', 0b100001 } },
        { "sub",    { 'R', 0b100010 } },
        { "subu",   { 'R', 0b100011 } },
        { "and",    { 'R', 0b100100 } },
        { "or",     { 'R', 0b100101 } },
        { "xor",    { 'R', 0b100110 } },
        { "nor",    { 'R', 0b100111 } },
        { "addi",   { 'I', 0b001000 } },
        { "addiu",  { 'I', 0b001001 } },
        { "andi",   { 'I', 0b001100 } },
        { "ori",    { 'I', 0b001101 } },
        { "xori",   { 'I', 0b001110 } },
        { "lui",    { 'I', 0b001111 } },
        { "sll",    { 'R', 0b000000 } },
        { "srl",    { 'R', 0b000010 } },
        { "sra",    { 'R', 0b000011 } },
        { "sllv",   { 'R', 0b000100 } },
        { "srlv",   { 'R', 0b000110 } },
        { "srav",   { 'R', 0b000111 } },
        { "mult",   { 'R', 0b011000 } },
        { "multu",  { 'R', 0b011001 } },
        { "div",    { 'R', 0b011010 } },
        { "divu",   { 'R', 0b011011 } },
        { "mfhi",   { 'R', 0b010000 } },
        { "mflo",   { 'R', 0b010010 } },
        { "mthi",   { 'R', 0b010001 } },
        { "mtlo",   { 'R', 0b010011 } },
        { "slt",    { 'R', 0b101010 } },
        { "sltu",   { 'R', 0b101011 } },
        { "slti",   { 'I', 0b001010 } },
        { "sltiu",  { 'I', 0b001011 } },
        { "lw",     { 'I', 0b100011 } },
        { "lh",     { 'I', 0b100001 } },
        { "lhu",    { 'I', 0b100101 } },
        { "lb",     { 'I', 0b100000 } },
        { "lbu",    { 'I', 0b100100 } },
        { "sw",     { 'I', 0b101011 } },
        { "sh",     { 'I', 0b101001 } },
        { "sb",     { 'I', 0b101000 } },
        { "ll",     { 'I', 0b110000 } },
        { "sc",     { 'I', 0b111000 } },
        { "beq",    { 'I', 0b000100 } },
        { "bne",    { 'I', 0b000101 } },
        { "blez",   { 'I', 0b000110 } },
        { "bgtz",   { 'I', 0b000111 } },
        { "bltz",   { 'I', 0b000001 } },
        { "bgez",   { 'I', 0b000001 } },
        { "bltzal", { 'I', 0b000001 } },
        { "bgezal", { 'I', 0b000001 } },
        { "j",      { 'J', 0b000010 } },
        { "jal",    { 'J', 0b000011 } },
        { "jr",     { 'R', 0b001000 } },
        { "jalr",   { 'R', 0b001001 } },
        /*  todo: add system calls  */
    };
};