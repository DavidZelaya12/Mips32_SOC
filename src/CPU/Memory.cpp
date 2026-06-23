#include "Memory.h"

Memory::Memory(uint32_t imSize, uint32_t dmSize, uint32_t stackSize)
    : instructionMem(imSize, 0),
      dataMem(dmSize, 0),
      stackMem(stackSize, 0),
      stackBase(STACK_TOP - stackSize + 1)
{
}

uint8_t Memory::readByte(uint32_t address)
{
    const uint8_t *p = resolve(address);
    return p ? *p : 0;
}

uint16_t Memory::readHalf(uint32_t address)
{
    return readByte(address) | (readByte(address + 1) << 8);
}

uint32_t Memory::readWord(uint32_t address)
{
    return readByte(address) |
           (readByte(address + 1) << 8) |
           (readByte(address + 2) << 16) |
           (readByte(address + 3) << 24);
}

void Memory::writeByte(uint32_t address, uint8_t value)
{
    uint8_t *p = resolve(address);
    if (p)
        *p = value;
}

void Memory::writeHalf(uint32_t address, uint16_t value)
{
    writeByte(address, value & 0xFF);
    writeByte(address + 1, (value >> 8) & 0xFF);
}

void Memory::writeWord(uint32_t address, uint32_t value)
{
    writeByte(address, value & 0xFF);
    writeByte(address + 1, (value >> 8) & 0xFF);
    writeByte(address + 2, (value >> 16) & 0xFF);
    writeByte(address + 3, (value >> 24) & 0xFF);
}

const uint8_t *Memory::resolve(uint32_t address) const
{
    if (address >= IM_BASE && address < IM_BASE + instructionMem.size())
        return &instructionMem[address - IM_BASE];
    if (address >= DM_BASE && address < DM_BASE + dataMem.size())
        return &dataMem[address - DM_BASE];
    if (address >= stackBase && address <= STACK_TOP)
        return &stackMem[address - stackBase];

    return nullptr;
}

uint8_t *Memory::resolve(uint32_t address)
{
    // Reusa la version const para no duplicar la logica del mapa de memoria.
    const Memory *self = this;
    return const_cast<uint8_t *>(self->resolve(address));
}
