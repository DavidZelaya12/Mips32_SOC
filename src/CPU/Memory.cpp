#include "Memory.h"
#include "../vga/Keypad.h"
#include "../vga/VGAFramebuffer.h"
#include "../vga/VGAConstants.h"
#include "../Timer.h"

Memory::Memory(uint32_t imSize, uint32_t dmSize, uint32_t stackSize)
    : instructionMem(imSize, 0),
      dataMem(dmSize, 0),
      stackMem(stackSize, 0),
      vgaMem(0x12C0, 0),
      stackBase(STACK_TOP - stackSize + 1)
{
}

void Memory::attachDevices(Keypad *kp, Timer *tm)
{
    keypad = kp;
    timer = tm;
}

void Memory::blitVGA(VGAFramebuffer &fb) const
{
    for (std::size_t i = 0; i < VGA_ROWS * VGA_COLS; ++i)
    {
        uint16_t cell = vgaMem[2 * i] |
                        (static_cast<uint16_t>(vgaMem[2 * i + 1]) << 8);
        fb.writePixel(i / VGA_COLS, i % VGA_COLS,
                      cell & 0xFF, (cell >> 8) & 0x0F, (cell >> 12) & 0x0F);
    }
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

    if (address == KEYPAD_ADDR)
        return keypad ? (keypad->state() & 0xFFFF) : 0;
    if (address == TIMER_ADDR)
        return timer ? timer->read() : 0;

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
    if (address >= VGA_BASE && address < VGA_BASE + vgaMem.size())
        return &vgaMem[address - VGA_BASE];
    if (address >= stackBase && address <= STACK_TOP)
        return &stackMem[address - stackBase];

    return nullptr;
}

uint8_t *Memory::resolve(uint32_t address)
{
    const Memory *self = this;                            // Use const version to avoid code duplication
    return const_cast<uint8_t *>(self->resolve(address)); // Safe because we only cast away constness for non-const objects
}
