#pragma once
#include <cstdint>
#include <vector>

class Keypad;         // MMIO 0xFFFF0004 (vga/Keypad.h)
class Timer;          // MMIO 0xFFFF0008 (Timer.h)
class VGAFramebuffer; // destino del blit (vga/VGAFramebuffer.h)

class Memory
{
public:
    uint32_t IM_BASE = 0x00004000;  // Instruction Memory
    uint32_t DM_BASE = 0x10000000;  // Data Memory
    uint32_t VGA_BASE = 0x0000B800; // VGA Text Buffer
    uint32_t STACK_TOP = 0x7FFFEFFC;

    static constexpr uint32_t KEYPAD_ADDR = 0xFFFF0004;
    static constexpr uint32_t TIMER_ADDR = 0xFFFF0008;

    Memory(uint32_t imSize = 0x4000, uint32_t dmSize = 0x2000, uint32_t stackSize = 0x2000);

    void attachDevices(Keypad *kp, Timer *tm);

    void blitVGA(VGAFramebuffer &fb) const;

    uint8_t readByte(uint32_t address);
    uint16_t readHalf(uint32_t address);
    uint32_t readWord(uint32_t address);

    void writeByte(uint32_t address, uint8_t value);
    void writeHalf(uint32_t address, uint16_t value);
    void writeWord(uint32_t address, uint32_t value);

private:
    uint8_t *resolve(uint32_t address);
    const uint8_t *resolve(uint32_t address) const;

    std::vector<uint8_t> instructionMem;
    std::vector<uint8_t> dataMem;
    std::vector<uint8_t> stackMem;
    std::vector<uint8_t> vgaMem;
    uint32_t stackBase;

    Keypad *keypad = nullptr;
    Timer *timer = nullptr;
};
