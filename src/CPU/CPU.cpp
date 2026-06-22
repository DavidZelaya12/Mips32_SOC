#include "CPU.h"

void CPU::ExecRFormat(const Instruction::R_Format &rFormat)
{
    switch (rFormat.funct)
    {
    }
}

void CPU::ExecIFormat(const Instruction::I_Format &iFormat)
{
    switch (iFormat.opcode)
    {
    }
}
void CPU::ExecJFormat(const Instruction::J_Format &jFormat)
{
    switch (jFormat.opcode)
    {
    }
}

void CPU::ExecuteInstruction()
{
    uint32_t pc = registerFile->getRegister(RegisterFile::reg::Pc);
    if (pc >= instructionMemory.size())
        throw std::runtime_error("Program counter out of bounds");

    uint32_t instruction = instructionMemory[pc];
    std::variant<Instruction::I_Format, Instruction::R_Format, Instruction::J_Format> decodedInstruction = Instruction::DecodeInstruction(instruction);

    uint8_t opcode = static_cast<uint8_t>(instruction >> 26);

    switch (Instruction::HandleFormat(opcode))
    {
    case Instruction::Format::R:
        std::cout << "Executing R-format instruction at PC: " << pc << std::endl;
        ExecRFormat(std::get<Instruction::R_Format>(decodedInstruction));
        break;

    case Instruction::Format::I:
        std::cout << "Executing I-format instruction at PC: " << pc << std::endl;
        ExecIFormat(std::get<Instruction::I_Format>(decodedInstruction));
        break;

    case Instruction::Format::J:
        std::cout << "Executing J-format instruction at PC: " << pc << std::endl;
        ExecJFormat(std::get<Instruction::J_Format>(decodedInstruction));
        break;

    default:
        throw std::runtime_error("Error: Unknown instruction format");
    }

    registerFile->incrementPC(1);
}
void CPU::Run()
{
    while (true)
    {
        ExecuteInstruction();
    }
}