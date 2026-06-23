#include "RegisterFile.h"

uint32_t RegisterFile::getRegister(reg r)
{
    return registers[r];
}

uint32_t RegisterFile::getRegister(uint8_t index)
{
    return registers[index];
}

void RegisterFile::setRegister(reg r, uint32_t value)
{
    if (r != Zero)
        registers[r] = value;
}

void RegisterFile::setRegister(uint8_t index, uint32_t value)
{
    if (index != Zero)
        registers[index] = value;
}

void RegisterFile::incrementPC(uint32_t offset)
{
    registers[Pc] += offset;
}