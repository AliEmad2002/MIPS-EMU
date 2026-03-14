#pragma once

class CAssembler{
public:
    CAssembler(std::string asmFileName);


private:
    std::ifstream m_asmFile;
    std::ofstream m_binFile;
    const std::map<std::string, std::function<CInstruction(const std::string&)>> instructionStrToWordFuncMap{
        {
            std::string("add"),
            [](const std::string& instructionStr)
            {
                return CInstruction(0);// todo
            }
        },
    };
};