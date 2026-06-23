#pragma once
#include <cstdint>
#include <vector>
class RegisterFile
{
private:
    std::vector<uint32_t> registers;

public:
    enum reg
    {
        Zero,
        At,
        v0,
        v1,
        a0,
        a1,
        a2,
        a3,
        t0,
        t1,
        t2,
        t3,
        t4,
        t5,
        t6,
        t7,
        s0,
        s1,
        s2,
        s3,
        s4,
        s5,
        s6,
        s7,
        t8,
        t9,
        k0,
        k1,
        Gp,
        Sp,
        Fp,
        Ra,
        Lo,
        Hi,
        Pc
    };

    uint32_t getRegister(reg r);
    uint32_t getRegister(uint8_t index);
    void setRegister(reg r, uint32_t value);
    void setRegister(uint8_t index, uint32_t value);
    void incrementPC(uint32_t offset);

    RegisterFile()
    {
        registers.resize(35, 0);
    };
};