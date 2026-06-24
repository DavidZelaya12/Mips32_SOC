#pragma once

#include "Memory.h"
#include "InstructionHandler.h"
#include "RegisterFile.h"
#include "../vga/VGAFramebuffer.h"
#include "../vga/Keypad.h"
#include "../Timer.h"
#include <iostream>
#include <utility>

class CPU
{
private:
    Memory *memory;
    RegisterFile *registerFile;
    Keypad keypad;
    Timer timer;
    VGAFramebuffer framebuffer;
    std::vector<uint32_t> instructionMemory;

public:
    CPU(std::vector<uint32_t> program)
        : instructionMemory(std::move(program))
    {
        memory = new Memory();
        registerFile = new RegisterFile();
        memory->attachDevices(&keypad, &timer);
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

    Keypad &getKeypad() { return keypad; }
    Timer &getTimer() { return timer; }
    VGAFramebuffer &getFramebuffer() { return framebuffer; }

    RegisterFile *getRegisterFile() const { return registerFile; }
    Memory *getMemory() const { return memory; }
    const std::vector<uint32_t> &getInstructionMemory() const { return instructionMemory; }
};