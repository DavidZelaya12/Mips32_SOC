#pragma once

#include "Memory.h"
#include "InstructionHandler.h"
#include "RegisterFile.h"
#include <iostream>

class CPU
{
private:
    Memory *memory;
    RegisterFile *registerFile;
    std::vector<uint32_t> instructionMemory;

public:
    CPU(std::vector<uint32_t> program)
        : instructionMemory(program)
    {
        memory = new Memory();
        registerFile = new RegisterFile();
    };

    ~CPU()
    {
        delete memory;
        delete registerFile;
    };

    void Run();
    void ExecuteInstruction();
    void ExecRFormat(const Instruction::R_Format &rFormat);
    void ExecIFormat(const Instruction::I_Format &iFormat);
    void ExecJFormat(const Instruction::J_Format &jFormat);

    RegisterFile *getRegisterFile() const { return registerFile; }
    Memory *getMemory() const { return memory; }
    std::vector<uint32_t> getInstructionMemory() const { return instructionMemory; }
};