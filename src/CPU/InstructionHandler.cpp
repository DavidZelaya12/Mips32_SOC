#include "InstructionHandler.h"

namespace Instruction
{
    Format HandleFormat(uint8_t opcode)
    {
        if (opcode == 0x00)
            return R;

        if (opcode == 0x02 || opcode == 0x03)
            return J;

        if (opcode == 0x04 || opcode == 0x05 || opcode == 0x06 || opcode == 0x07 || // beq, bne, blez, bgtz
            opcode == 0x08 || opcode == 0x09 || opcode == 0x0A || opcode == 0x0B || // addi, addiu, slti, sltiu
            opcode == 0x0C || opcode == 0x0D || opcode == 0x0E || opcode == 0x0F || // andi, ori, xori, lui
            opcode == 0x20 || opcode == 0x21 || opcode == 0x23 ||                   // lb, lh, lw
            opcode == 0x24 || opcode == 0x25 ||                                     // lbu, lhu
            opcode == 0x28 || opcode == 0x29 || opcode == 0x2B)                     // sb, sh, sw
            return I;

        return undefined;
    }

    I_Format DecodeIFormat(uint32_t instruction)
    {
        I_Format format;
        format.opcode = (instruction >> 26) & 0x3F;
        format.rs = (instruction >> 21) & 0x1F;
        format.rt = (instruction >> 16) & 0x1F;
        format.immediate = instruction & 0xFFFF;
        return format;
    }

    R_Format DecodeRFormat(uint32_t instruction)
    {
        R_Format format;
        format.opcode = (instruction >> 26) & 0x3F;
        format.rs = (instruction >> 21) & 0x1F;
        format.rt = (instruction >> 16) & 0x1F;
        format.rd = (instruction >> 11) & 0x1F;
        format.shamt = (instruction >> 6) & 0x1F;
        format.funct = instruction & 0x3F;
        return format;
    }

    J_Format DecodeJFormat(uint32_t instruction)
    {
        J_Format format;
        format.opcode = (instruction >> 26) & 0x3F;
        format.address = instruction & 0x3FFFFFF;
        return format;
    }

    std::variant<I_Format, R_Format, J_Format> DecodeInstruction(uint32_t instruction)
    {
        uint8_t opcode = (instruction >> 26) & 0x3F;
        Format formatType = HandleInstructionFormat(opcode);

        switch (formatType)
        {
        case R:
            return DecodeRFormat(instruction);
        case I:
            return DecodeIFormat(instruction);
        case J:
            return DecodeJFormat(instruction);
        default:
            throw std::runtime_error("Undefined instruction format");
        }
    }
}