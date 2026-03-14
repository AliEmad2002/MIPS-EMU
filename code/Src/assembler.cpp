#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <functional>
#include <map>
#include <cctype>
#include "configuration.hpp"
#include "types.hpp"
#include "instruction.hpp"
#include "assembler.hpp"

CAssembler::CAssembler(std::string asmFileName):
m_asmFile(asmFileName) 
{
    /*  Initialize a label - address map    */
    std::map<std::string, u32> labelAddressMap;

    /*  Initialize an address counter   */
    u32 addressCounter = 0;

    /*  For every line in the .asm */
    std::string line;
    while(std::getline(m_asmFile, line, '\n'))
    {
        /*  Remove any white spaces or tabs at the beginning of the line    */
        line = line.substr(line.find_first_not_of(" \t"));

        /*  If line starts with a '#' (i.e.: a comment), ignore it  */
        if(line[0] == '#')
        {
            /*  Do nothing, just continue   */
            continue;
        }

        /*  If line starts with an upper case char, it is a label, not an instruction */
        if (std::isupper(line[0]))
        {
            /*  Remove the ':' at the end of the label line */
            line = line.substr(0, line.length()-1);

            /*  Store label's address in the label-address map and continue  */
            labelAddressMap[line] = addressCounter;
            continue;
        }

        /*  Otherwise, line should be an isntruction    */

        /*  Get binary word of this instruction using the instruction-ascii-to-bin map  */

        /*  Append instruction's binary word to the "m_binFile" */

        /*  Increment the address counter   */

    }
}