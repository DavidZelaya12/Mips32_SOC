#include "RegisterFilea .h"

uint32_t RegisterFile::getRegister(reg r)
{
    return registers[r];
}

void RegisterFile::setRegister(reg r, uint32_t value)
{
    if (r != Zero)
        registers[r] = value;
}

void RegisterFile::incrementPC(uint32_t offset)
{
    registers[Pc] += offset;
}