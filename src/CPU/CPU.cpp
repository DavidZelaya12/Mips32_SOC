#include "CPU.h"

void CPU::ExecRFormat(const Instruction::R_Format &rFormat)
{
    switch (rFormat.funct)
    {
    // SLL
    case 0x00:
        std::cout << "Executing SLL instruction" << std::endl;
        uint32_t value = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        uint32_t result = value << rFormat.shamt;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), result);
        break;

    // SRL
    case 0x02:
        std::cout << "Executing SRL instruction" << std::endl;
        uint32_t value = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        uint32_t result = value >> rFormat.shamt;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), result);
        break;

    // sra
    case 0x03:
        std::cout << "Executing SRA instruction" << std::endl;
        uint32_t value = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        uint32_t result = static_cast<int32_t>(value) >> rFormat.shamt;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), result);
        break;

    //  sllv
    case 0x04:
        std::cout << "Executing SLLV instruction" << std::endl;
        uint32_t value = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        uint32_t shiftAmount = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)) & 0x1F; // Use only the lower 5 bits
        uint32_t result = value << shiftAmount;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), result);
        break;

    //  srlv
    case 0x06:
        std::cout << "Executing SRLV instruction" << std::endl;
        uint32_t value = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        uint32_t shiftAmount = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)) & 0x1F; // Use only the lower 5 bits
        uint32_t result = value >> shiftAmount;

    // srav
    case 0x07:
        std::cout << "Executing SRAV instruction" << std::endl;
        uint32_t value = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        uint32_t shiftAmount = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)) & 0x1F; // Use only the lower 5 bits
        uint32_t result = static_cast<int32_t>(value) >> shiftAmount;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), result);
        break;

        // jr
    case 0x08:
        std::cout << "Executing JR instruction" << std::endl;
        uint32_t targetAddress = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs));
        registerFile->setRegister(RegisterFile::reg::Pc, targetAddress);
        break;

        // mult
    case 0x18:
        std::cout << "Executing MULT instruction" << std::endl;
        int32_t multiplicand = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)));
        int32_t multiplier = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt)));
        int64_t product = static_cast<int64_t>(multiplicand) * static_cast<int64_t>(multiplier);
        registerFile->setRegister(RegisterFile::reg::Lo, static_cast<uint32_t>(product & 0xFFFFFFFF));
        registerFile->setRegister(RegisterFile::reg::Hi, static_cast<uint32_t>((product >> 32) & 0xFFFFFFFF));
        break;

    // multu
    case 0x19:
        std::cout << "Executing MULTU instruction" << std::endl;
        uint32_t multiplicand = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs));
        uint32_t multiplier = registerFile->getRegister(static_cast<RegisterFile::reg>(RegisterFile::reg>(rFormat.rt));
        uint64_t product = static_cast<uint64_t>(multiplicand) * static_cast<uint64_t>(multiplier);
        registerFile->setRegister(RegisterFile::reg::Lo, static_cast<uint32_t>(product & 0xFFFFFFFF));
        registerFile->setRegister(RegisterFile::reg::Hi, static_cast<uint32_t>((product >> 32) & 0xFFFFFFFF));
        break;

        //div 
        case 0x1A:
        std::cout << "Executing DIV instruction" << std::endl;
        int32_t dividend = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)));
        int32_t divisor = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt)));

        if (divisor == 0)        
            throw std::runtime_error("Error: Division by zero");

        int32_t quotient = dividend / divisor;
        int32_t remainder = dividend % divisor;
        registerFile->setRegister(RegisterFile::reg::Lo, static_cast<uint32_t>(quotient));
        registerFile->setRegister(RegisterFile::reg::Hi, static_cast<uint32_t>(remainder));
        break;

        //divu
        case 0x1B:
        std::cout << "Executing DIVU instruction" << std::endl;
        uint32_t dividend = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs));
        uint32_t divisor = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        if (divisor == 0)        
            throw std::runtime_error("Error: Division by zero");

        uint32_t quotient = dividend / divisor;
        uint32_t remainder = dividend % divisor;
        registerFile->setRegister(RegisterFile::reg::Lo, quotient);
        registerFile->setRegister(RegisterFile::reg::Hi, remainder);    
        break;  
        //add
        case 0x20:
        std::cout << "Executing ADD instruction" << std::endl;
        int32_t add1 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)));
        int32_t add2 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt)));
        int32_t sum = add1 + add2;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), static_cast<uint32_t>(sum));
        break;

        //addu
        case 0x21:
        std::cout << "Executing ADDU instruction" << std::endl;
        uint32_t add1 = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs));
        uint32_t add2 = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        uint32_t sum = add1 + add2;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), sum);
        break;
        
        //sub
        case 0x22:
        std::cout << "Executing SUB instruction" << std::endl;
        int32_t sub1 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)));
        int32_t sub2 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt)));
        int32_t difference = sub1 - sub2;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), static_cast<uint32_t>(difference));
        break;

        //subu
        case 0x23:
        std::cout << "Executing SUBU instruction" << std::endl;
        uint32_t sub1 = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs));
        uint32_t sub2 = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        uint32_t difference = sub1 - sub2;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), difference);
        break;

        //and 
        case 0x24:
        std::cout << "Executing AND instruction" << std::endl;  
        int32_t and1 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)));
        int32_t and2 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt)));
        int32_t andResult = and1 & and2;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), static_cast<uint32_t>(andResult));
        break;
        //or
        case 0x25:
        std::cout << "Executing OR instruction" << std::endl;
        int32_t or1 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)));
        int32_t or2 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt)));
        int32_t orResult = or1 | or2;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), static_cast<uint32_t>(orResult));
        break;

        //xor
        case 0x26:
        std::cout << "Executing XOR instruction" << std::endl;
        int32_t xor1 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)));
        int32_t xor2 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt)));
        int32_t xorResult = xor1 ^ xor2;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), static_cast<uint32_t>(xorResult));
        break;

        //slt 
        case 0x2A:
        std::cout << "Executing SLT instruction" << std::endl;
        int32_t slt1 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs)));
        int32_t slt2 = static_cast<int32_t>(registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt)));
        int32_t sltResult = (slt1 < slt2) ? 1 : 0;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), static_cast<uint32_t>(sltResult));
        break;

        //sltu
        case 0x2B:
        std::cout << "Executing SLTU instruction" << std::endl;
        uint32_t slt1 = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rs));
        uint32_t slt2 = registerFile->getRegister(static_cast<RegisterFile::reg>(rFormat.rt));
        uint32_t sltResult = (slt1 < slt2) ? 1 : 0;
        registerFile->setRegister(static_cast<RegisterFile::reg>(rFormat.rd), sltResult);
        break;
        
        default:
        throw std::runtime_error("Error: Unknown R-format instruction");
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