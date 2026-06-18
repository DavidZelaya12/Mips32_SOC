#pragma once
#include <cstdint>
#include <variant>
#include <stdexcept>

namespace Instruction
{
    enum Format
    {
        R,
        I,
        J,
        undefined
    };

    struct I_Format
    {
        uint8_t opcode;
        uint8_t rs;
        uint8_t rt;
        uint16_t immediate;
    };

    struct R_Format
    {
        uint8_t opcode;
        uint8_t rs;
        uint8_t rt;
        uint8_t rd;
        uint8_t shamt;
        uint8_t funct;
    };

    struct J_Format
    {
        uint8_t opcode;
        uint32_t address;
    };

    Format HandleFormat(uint8_t opcode);
    I_Format DecodeIFormat(uint32_t instruction);
    R_Format DecodeRFormat(uint32_t instruction);
    J_Format DecodeJFormat(uint32_t instruction);
    std::variant<I_Format, R_Format, J_Format> DecodeInstruction(uint32_t instruction);
}