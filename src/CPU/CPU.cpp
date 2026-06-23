#include "CPU.h"

void CPU::ExecRFormat(const Instruction::R_Format &rFormat)
{
    switch (rFormat.funct)
    {
    // SLL
    case 0x00:
    {
        std::cout << "Executing SLL instruction" << std::endl;
        uint32_t value = registerFile->getRegister(rFormat.rt);
        uint32_t result = value << rFormat.shamt;
        registerFile->setRegister(rFormat.rd, result);
        break;
    }

    // SRL
    case 0x02:
    {
        std::cout << "Executing SRL instruction" << std::endl;
        uint32_t value = registerFile->getRegister(rFormat.rt);
        uint32_t result = value >> rFormat.shamt;
        registerFile->setRegister(rFormat.rd, result);
        break;
    }

    // sra
    case 0x03:
    {
        std::cout << "Executing SRA instruction" << std::endl;
        uint32_t value = registerFile->getRegister(rFormat.rt);
        uint32_t result = static_cast<int32_t>(value) >> rFormat.shamt;
        registerFile->setRegister(rFormat.rd, result);
        break;
    }

    //  sllv
    case 0x04:
    {
        std::cout << "Executing SLLV instruction" << std::endl;
        uint32_t value = registerFile->getRegister(rFormat.rt);
        uint32_t shiftAmount = registerFile->getRegister(rFormat.rs) & 0x1F; // Use only the lower 5 bits
        uint32_t result = value << shiftAmount;
        registerFile->setRegister(rFormat.rd, result);
        break;
    }

    //  srlv
    case 0x06:
    {
        std::cout << "Executing SRLV instruction" << std::endl;
        uint32_t value = registerFile->getRegister(rFormat.rt);
        uint32_t shiftAmount = registerFile->getRegister(rFormat.rs) & 0x1F; // Use only the lower 5 bits
        uint32_t result = value >> shiftAmount;
        registerFile->setRegister(rFormat.rd, result);
        break;
    }

    // srav
    case 0x07:
    {
        std::cout << "Executing SRAV instruction" << std::endl;
        uint32_t value = registerFile->getRegister(rFormat.rt);
        uint32_t shiftAmount = registerFile->getRegister(rFormat.rs) & 0x1F; // Use only the lower 5 bits
        uint32_t result = static_cast<int32_t>(value) >> shiftAmount;
        registerFile->setRegister(rFormat.rd, result);
        break;
    }

        // jr
    case 0x08:
    {
        std::cout << "Executing JR instruction" << std::endl;
        uint32_t targetAddress = registerFile->getRegister(rFormat.rs);
        registerFile->setRegister(RegisterFile::reg::Pc, targetAddress);
        break;
    }

        // mfhi
    case 0x10:
    {
        std::cout << "Executing MFHI instruction" << std::endl;
        registerFile->setRegister(rFormat.rd, registerFile->getRegister(RegisterFile::reg::Hi));
        break;
    }

        // mflo
    case 0x12:
    {
        std::cout << "Executing MFLO instruction" << std::endl;
        registerFile->setRegister(rFormat.rd, registerFile->getRegister(RegisterFile::reg::Lo));
        break;
    }

        // mult
    case 0x18:
    {
        std::cout << "Executing MULT instruction" << std::endl;
        int32_t multiplicand = static_cast<int32_t>(registerFile->getRegister(rFormat.rs));
        int32_t multiplier = static_cast<int32_t>(registerFile->getRegister(rFormat.rt));
        int64_t product = static_cast<int64_t>(multiplicand) * static_cast<int64_t>(multiplier);
        registerFile->setRegister(RegisterFile::reg::Lo, static_cast<uint32_t>(product & 0xFFFFFFFF));
        registerFile->setRegister(RegisterFile::reg::Hi, static_cast<uint32_t>((product >> 32) & 0xFFFFFFFF));
        break;
    }

    // multu
    case 0x19:
    {
        std::cout << "Executing MULTU instruction" << std::endl;
        uint32_t multiplicand = registerFile->getRegister(rFormat.rs);
        uint32_t multiplier = registerFile->getRegister(rFormat.rt);
        uint64_t product = static_cast<uint64_t>(multiplicand) * static_cast<uint64_t>(multiplier);
        registerFile->setRegister(RegisterFile::reg::Lo, static_cast<uint32_t>(product & 0xFFFFFFFF));
        registerFile->setRegister(RegisterFile::reg::Hi, static_cast<uint32_t>((product >> 32) & 0xFFFFFFFF));
        break;
    }

        // div
    case 0x1A:
    {
        std::cout << "Executing DIV instruction" << std::endl;
        int32_t dividend = static_cast<int32_t>(registerFile->getRegister(rFormat.rs));
        int32_t divisor = static_cast<int32_t>(registerFile->getRegister(rFormat.rt));

        if (divisor == 0)
            throw std::runtime_error("Error: Division by zero");

        int32_t quotient = dividend / divisor;
        int32_t remainder = dividend % divisor;
        registerFile->setRegister(RegisterFile::reg::Lo, static_cast<uint32_t>(quotient));
        registerFile->setRegister(RegisterFile::reg::Hi, static_cast<uint32_t>(remainder));
        break;
    }

        // divu
    case 0x1B:
    {
        std::cout << "Executing DIVU instruction" << std::endl;
        uint32_t dividend = registerFile->getRegister(rFormat.rs);
        uint32_t divisor = registerFile->getRegister(rFormat.rt);
        if (divisor == 0)
            throw std::runtime_error("Error: Division by zero");

        uint32_t quotient = dividend / divisor;
        uint32_t remainder = dividend % divisor;
        registerFile->setRegister(RegisterFile::reg::Lo, quotient);
        registerFile->setRegister(RegisterFile::reg::Hi, remainder);
        break;
    }
        // add
    case 0x20:
    {
        std::cout << "Executing ADD instruction" << std::endl;
        int32_t add1 = static_cast<int32_t>(registerFile->getRegister(rFormat.rs));
        int32_t add2 = static_cast<int32_t>(registerFile->getRegister(rFormat.rt));
        int32_t sum = add1 + add2;
        registerFile->setRegister(rFormat.rd, static_cast<uint32_t>(sum));
        break;
    }

        // addu
    case 0x21:
    {
        std::cout << "Executing ADDU instruction" << std::endl;
        uint32_t add1 = registerFile->getRegister(rFormat.rs);
        uint32_t add2 = registerFile->getRegister(rFormat.rt);
        uint32_t sum = add1 + add2;
        registerFile->setRegister(rFormat.rd, sum);
        break;
    }

        // sub
    case 0x22:
    {
        std::cout << "Executing SUB instruction" << std::endl;
        int32_t sub1 = static_cast<int32_t>(registerFile->getRegister(rFormat.rs));
        int32_t sub2 = static_cast<int32_t>(registerFile->getRegister(rFormat.rt));
        int32_t difference = sub1 - sub2;
        registerFile->setRegister(rFormat.rd, static_cast<uint32_t>(difference));
        break;
    }

        // subu
    case 0x23:
    {
        std::cout << "Executing SUBU instruction" << std::endl;
        uint32_t sub1 = registerFile->getRegister(rFormat.rs);
        uint32_t sub2 = registerFile->getRegister(rFormat.rt);
        uint32_t difference = sub1 - sub2;
        registerFile->setRegister(rFormat.rd, difference);
        break;
    }

        // and
    case 0x24:
    {
        std::cout << "Executing AND instruction" << std::endl;
        int32_t and1 = static_cast<int32_t>(registerFile->getRegister(rFormat.rs));
        int32_t and2 = static_cast<int32_t>(registerFile->getRegister(rFormat.rt));
        int32_t andResult = and1 & and2;
        registerFile->setRegister(rFormat.rd, static_cast<uint32_t>(andResult));
        break;
    }
        // or
    case 0x25:
    {
        std::cout << "Executing OR instruction" << std::endl;
        int32_t or1 = static_cast<int32_t>(registerFile->getRegister(rFormat.rs));
        int32_t or2 = static_cast<int32_t>(registerFile->getRegister(rFormat.rt));
        int32_t orResult = or1 | or2;
        registerFile->setRegister(rFormat.rd, static_cast<uint32_t>(orResult));
        break;
    }

        // xor
    case 0x26:
    {
        std::cout << "Executing XOR instruction" << std::endl;
        int32_t xor1 = static_cast<int32_t>(registerFile->getRegister(rFormat.rs));
        int32_t xor2 = static_cast<int32_t>(registerFile->getRegister(rFormat.rt));
        int32_t xorResult = xor1 ^ xor2;
        registerFile->setRegister(rFormat.rd, static_cast<uint32_t>(xorResult));
        break;
    }

        // slt
    case 0x2A:
    {
        std::cout << "Executing SLT instruction" << std::endl;
        int32_t slt1 = static_cast<int32_t>(registerFile->getRegister(rFormat.rs));
        int32_t slt2 = static_cast<int32_t>(registerFile->getRegister(rFormat.rt));
        int32_t sltResult = (slt1 < slt2) ? 1 : 0;
        registerFile->setRegister(rFormat.rd, static_cast<uint32_t>(sltResult));
        break;
    }

        // sltu
    case 0x2B:
    {
        std::cout << "Executing SLTU instruction" << std::endl;
        uint32_t slt1 = registerFile->getRegister(rFormat.rs);
        uint32_t slt2 = registerFile->getRegister(rFormat.rt);
        uint32_t sltResult = (slt1 < slt2) ? 1 : 0;
        registerFile->setRegister(rFormat.rd, sltResult);
        break;
    }

    default:
        throw std::runtime_error("Error: Unknown R-format instruction");
    }
}

void CPU::ExecIFormat(const Instruction::I_Format &iFormat)
{
    switch (iFormat.opcode)
    {
        // beq
    case 0x04:
    {
        std::cout << "Executing BEQ instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        uint32_t rtValue = registerFile->getRegister(iFormat.rt);
        if (rsValue == rtValue)
        {
            int16_t offset = static_cast<int16_t>(iFormat.immediate);
            uint32_t newPC = registerFile->getRegister(RegisterFile::reg::Pc) + offset;
            registerFile->setRegister(RegisterFile::reg::Pc, newPC);
        }
        break;
    }

    // bne
    case 0x05:
    {
        std::cout << "Executing BNE instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        uint32_t rtValue = registerFile->getRegister(iFormat.rt);
        if (rsValue != rtValue)
        {
            int16_t offset = static_cast<int16_t>(iFormat.immediate);
            uint32_t newPC = registerFile->getRegister(RegisterFile::reg::Pc) + offset;
            registerFile->setRegister(RegisterFile::reg::Pc, newPC);
        }
        break;
    }

        // blez
    case 0x06:
    {
        std::cout << "Executing BLEZ instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        if (static_cast<int32_t>(rsValue) <= 0)
        {
            int16_t offset = static_cast<int16_t>(iFormat.immediate);
            uint32_t newPC = registerFile->getRegister(RegisterFile::reg::Pc) + offset;
            registerFile->setRegister(RegisterFile::reg::Pc, newPC);
        }
        break;
    }
        // bgtz
    case 0x07:
    {
        std::cout << "Executing BGTZ instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        if (static_cast<int32_t>(rsValue) > 0)
        {
            int16_t offset = static_cast<int16_t>(iFormat.immediate);
            uint32_t newPC = registerFile->getRegister(RegisterFile::reg::Pc) + offset;
            registerFile->setRegister(RegisterFile::reg::Pc, newPC);
        }
        break;
    }

    // addi
    case 0x08:
    {
        std::cout << "Executing ADDI instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        int16_t immediate = static_cast<int16_t>(iFormat.immediate);
        uint32_t result = rsValue + immediate;
        registerFile->setRegister(iFormat.rt, result);
        break;
    }
        // addiu
    case 0x09:
    {
        std::cout << "Executing ADDIU instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        int16_t immediate = static_cast<int16_t>(iFormat.immediate);
        uint32_t result = rsValue + immediate;
        registerFile->setRegister(iFormat.rt, result);
        break;
    }

        // slti
    case 0x0A:
    {
        std::cout << "Executing SLTI instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        int16_t immediate = static_cast<int16_t>(iFormat.immediate);
        uint32_t result = (static_cast<int32_t>(rsValue) < immediate) ? 1 : 0;
        registerFile->setRegister(iFormat.rt, result);
        break;
    }

        // sltiu
    case 0x0B:
    {
        std::cout << "Executing SLTIU instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        int16_t immediate = static_cast<int16_t>(iFormat.immediate);
        uint32_t result = (rsValue < static_cast<uint32_t>(immediate)) ? 1 : 0;
        registerFile->setRegister(iFormat.rt, result);
        break;
    }

        // adni
    case 0x0C:
    {
        std::cout << "Executing ANDI instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        uint16_t immediate = iFormat.immediate;
        uint32_t result = rsValue & immediate;
        registerFile->setRegister(iFormat.rt, result);
        break;
    }
    // ori
    case 0x0D:
    {
        std::cout << "Executing ORI instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        uint16_t immediate = iFormat.immediate;
        uint32_t result = rsValue | immediate;
        registerFile->setRegister(iFormat.rt, result);
        break;
    }

        // xori
    case 0x0E:
    {
        std::cout << "Executing XORI instruction" << std::endl;
        uint32_t rsValue = registerFile->getRegister(iFormat.rs);
        uint16_t immediate = iFormat.immediate;
        uint32_t result = rsValue ^ immediate;
        registerFile->setRegister(iFormat.rt, result);
        break;
    }

        // lui
    case 0x0F:
    {
        std::cout << "Executing LUI instruction" << std::endl;
        uint16_t immediate = iFormat.immediate;
        uint32_t result = static_cast<uint32_t>(immediate) << 16;
        registerFile->setRegister(iFormat.rt, result);
        break;
    }

    // lb
    case 0x20:
    {
        std::cout << "Executing LB instruction" << std::endl;
        uint32_t baseAddress = registerFile->getRegister(iFormat.rs);
        int16_t offset = static_cast<int16_t>(iFormat.immediate);
        uint32_t effectiveAddress = baseAddress + offset;
        uint8_t byteValue = memory->readByte(effectiveAddress);
        registerFile->setRegister(iFormat.rt, static_cast<uint32_t>(static_cast<int8_t>(byteValue))); // Sign extension
        break;
    }

    // lh
    case 0x21:
    {
        std::cout << "Executing LH instruction" << std::endl;
        uint32_t baseAddress = registerFile->getRegister(iFormat.rs);
        int16_t offset = static_cast<int16_t>(iFormat.immediate);
        uint32_t effectiveAddress = baseAddress + offset;
        uint16_t halfValue = memory->readHalf(effectiveAddress);
        registerFile->setRegister(iFormat.rt, static_cast<uint32_t>(static_cast<int16_t>(halfValue))); // Sign extension
        break;
    }

        // lw
    case 0x23:
    {
        std::cout << "Executing LW instruction" << std::endl;
        uint32_t baseAddress = registerFile->getRegister(iFormat.rs);
        int16_t offset = static_cast<int16_t>(iFormat.immediate);
        uint32_t effectiveAddress = baseAddress + offset;
        uint32_t wordValue = memory->readWord(effectiveAddress);
        registerFile->setRegister(iFormat.rt, wordValue);
        break;
    }

        // lbu
    case 0x24:
    {
        std::cout << "Executing LBU instruction" << std::endl;
        uint32_t baseAddress = registerFile->getRegister(iFormat.rs);
        int16_t offset = static_cast<int16_t>(iFormat.immediate);
        uint32_t effectiveAddress = baseAddress + offset;
        uint8_t byteValue = memory->readByte(effectiveAddress);
        registerFile->setRegister(iFormat.rt, static_cast<uint32_t>(byteValue)); // No sign extension
        break;
    }

    // lhu
    case 0x25:
    {
        std::cout << "Executing LHU instruction" << std::endl;
        uint32_t baseAddress = registerFile->getRegister(iFormat.rs);
        int16_t offset = static_cast<int16_t>(iFormat.immediate);
        uint32_t effectiveAddress = baseAddress + offset;
        uint16_t halfValue = memory->readHalf(effectiveAddress);
        registerFile->setRegister(iFormat.rt, static_cast<uint32_t>(halfValue)); // No sign extension
        break;
    }

    // sb
    case 0x28:
    {
        std::cout << "Executing SB instruction" << std::endl;
        uint32_t baseAddress = registerFile->getRegister(iFormat.rs);
        int16_t offset = static_cast<int16_t>(iFormat.immediate);
        uint32_t effectiveAddress = baseAddress + offset;
        uint32_t valueToStore = registerFile->getRegister(iFormat.rt);
        memory->writeByte(effectiveAddress, static_cast<uint8_t>(valueToStore & 0xFF));
        break;
    }

    // sh
    case 0x29:
    {
        std::cout << "Executing SH instruction" << std::endl;
        uint32_t baseAddress = registerFile->getRegister(iFormat.rs);
        int16_t offset = static_cast<int16_t>(iFormat.immediate);
        uint32_t effectiveAddress = baseAddress + offset;
        uint32_t valueToStore = registerFile->getRegister(iFormat.rt);
        memory->writeHalf(effectiveAddress, static_cast<uint16_t>(valueToStore & 0xFFFF));
        break;
    }

        // sw
    case 0x2B:
    {
        std::cout << "Executing SW instruction" << std::endl;
        uint32_t baseAddress = registerFile->getRegister(iFormat.rs);
        int16_t offset = static_cast<int16_t>(iFormat.immediate);
        uint32_t effectiveAddress = baseAddress + offset;
        uint32_t valueToStore = registerFile->getRegister(iFormat.rt);
        memory->writeWord(effectiveAddress, valueToStore);
        break;
    }

    default:
        throw std::runtime_error("Error: Unknown I-format instruction");
    }
}
void CPU::ExecJFormat(const Instruction::J_Format &jFormat)
{
    switch (jFormat.opcode)
    {
    // j
    case 0x02:
    {
        std::cout << "Executing J instruction" << std::endl;
        uint32_t targetAddress = jFormat.address; // PC es indice de palabra: address ya es el indice destino
        registerFile->setRegister(RegisterFile::reg::Pc, targetAddress);
        break;
    }

    // jal
    case 0x03:
    {
        std::cout << "Executing JAL instruction" << std::endl;
        uint32_t returnAddress = registerFile->getRegister(RegisterFile::reg::Pc); // PC ya apunta a la siguiente instruccion
        registerFile->setRegister(RegisterFile::reg::Ra, returnAddress);
        uint32_t targetAddress = jFormat.address;
        registerFile->setRegister(RegisterFile::reg::Pc, targetAddress);
        break;
    }
    }
}

void CPU::ExecuteInstruction()
{
    uint32_t pc = registerFile->getRegister(RegisterFile::reg::Pc);
    if (pc >= instructionMemory.size())
    {
        std::cerr << "Error: PC out of bounds. PC: " << pc << ", Instruction Memory Size: " << instructionMemory.size() << std::endl;
        return; // no indexar fuera de rango (seria UB)
    }

    uint32_t instruction = instructionMemory[pc];
    std::variant<Instruction::I_Format, Instruction::R_Format, Instruction::J_Format> decodedInstruction = Instruction::DecodeInstruction(instruction);

    uint8_t opcode = static_cast<uint8_t>(instruction >> 26);

    // Avanzar el PC antes de ejecutar (semantica PC+4): los branches/jumps
    // ajustan relativo a esta siguiente instruccion.
    registerFile->incrementPC(1);

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
}
void CPU::Run()
{
    while (true)
    {
        ExecuteInstruction();
    }
}
