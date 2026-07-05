#include "vga/VGATextWindow.h"
#include "vga/VGAFramebuffer.h"
#include "vga/VGAFont.h"
#include "vga/Keypad.h"

#include "CLI_args/CliArgs.h"
#include "File_loader.cpp"
#include "CPU/InstructionHandler.h"
#include "CPU/CPU.h"

#include <array>
#include <format>
#include <iostream>
#include <thread>
#include <atomic>

using namespace Instruction;

inline constexpr std::array<uint8_t, 16> kDefaultVGAPalette = {
    0x00,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0A,
    0x0B,
    0x0C,
    0x0D,
    0x0E,
    0x0F,
};

void PrintInstructions(const std::vector<std::uint32_t> &instructions)
{
    for (size_t i = 0; i < instructions.size(); ++i)
    {

        auto decoded = DecodeInstruction(instructions[i]);
        std::visit([](auto &&arg)
                   {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, I_Format>)
                std::cout << "I-Format: opcode=" << std::hex << static_cast<int>(arg.opcode)
                          << " rs=" << static_cast<int>(arg.rs)
                          << " rt=" << static_cast<int>(arg.rt)
                          << " immediate=" << std::hex << arg.immediate << "\n";
            else if constexpr (std::is_same_v<T, R_Format>)
                std::cout << "R-Format: opcode=" << std::hex << static_cast<int>(arg.opcode)
                          << " rs=" << static_cast<int>(arg.rs)
                          << " rt=" << static_cast<int>(arg.rt)
                          << " rd=" << static_cast<int>(arg.rd)
                          << " shamt=" << std::hex << static_cast<int>(arg.shamt)
                          << " funct=" << std::hex << static_cast<int>(arg.funct) << "\n";
            else if constexpr (std::is_same_v<T, J_Format>)
                std::cout << "J-Format: opcode=" << std::hex << static_cast<int>(arg.opcode)
                          << " address=" << std::hex << arg.address << "\n"; },
                   decoded);
    }
}

inline constexpr std::array<const char *, 32> kRegisterNames = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

void PrintRegisters(RegisterFile *registerFile)
{
    constexpr int kInnerWidth = 72; // 4 columnas x 18 chars
    const std::string border = "+" + std::string(kInnerWidth, '-') + "+";

    std::cout << "\n"
              << std::format("+{:-^{}}+\n", " Estado final de registros ", kInnerWidth);
    for (int row = 0; row < 8; ++row)
    {
        std::cout << "|";
        for (int col = 0; col < 4; ++col)
        {
            const int i = row + col * 8; // orden por columnas: $zero..$a3 | $t0.. | $s0.. | $t8..
            std::cout << std::format(" {:>5} 0x{:08X} ", kRegisterNames[i],
                                     registerFile->getRegister(static_cast<uint8_t>(i)));
        }
        std::cout << "|\n";
    }
    std::cout << border << "\n"
              << std::format("|{:^{}}|\n",
                             std::format("HI 0x{:08X}    LO 0x{:08X}    PC 0x{:08X}",
                                         registerFile->getRegister(RegisterFile::reg::Hi),
                                         registerFile->getRegister(RegisterFile::reg::Lo),
                                         registerFile->getRegister(RegisterFile::reg::Pc)),
                             kInnerWidth)
              << border << "\n";
}

void PrintBanner(const std::string &programPath, size_t instructionCount, bool usedDefaultProgram)
{
    std::cout << "\n"
              << "==========================================================\n"
              << "         MIPS32 SoC Simulator  |  CPU + VGA + MMIO        \n"
              << "==========================================================\n";
    if (usedDefaultProgram)
        std::cout << "  Programa   : (semilla por defecto, no se pudo cargar '" << programPath << "')\n";
    else
        std::cout << "  Programa   : " << programPath << "\n";
    std::cout << std::format("  Instrucciones cargadas : {}\n", instructionCount)
              << "  Pantalla   : VGA texto 80x30, 16 colores\n"
              << "----------------------------------------------------------\n"
              << "  Controles:\n"
              << "    Flechas / Q P B Espacio ............ teclas K1-K8\n"
              << "    PgUp PgDn Home End Ins Del F1 F2 ... teclas K9-K16\n"
              << "    R = Reset | S = Stop | Cerrar ventana = salir\n"
              << "==========================================================\n\n";
}

int main(int argc, const char *argv[])
{
    CliArgs cliArgs(argc, argv);

    if (auto err = cliArgs.parse(); err.has_value())
    {
        std::cerr << "Error parsing arguments: " << *err << "\n";
        cliArgs.printUsage();
        return EXIT_FAILURE;
    }

    std::optional<std::vector<std::uint32_t>> instructions;
    instructions = Loadinstructions(cliArgs.getProgramPath());
    const bool usedDefaultProgram = (instructions == std::nullopt);
    if (instructions == std::nullopt)
    {
        std::cerr << "Failed to load machine code from file: " << cliArgs.getProgramPath() << "\n";
        std::cerr << "Seeding with default instructions.\n";
        instructions = std::vector<std::uint32_t>{
            0x2008002A, //  0: addi  $t0, $zero, 42
            0x2009003A, //  1: addi  $t1, $zero, 58
            0x240A0064, //  2: addiu $t2, $zero, 100
            0x3C0B1234, //  3: lui   $t3, 0x1234
            0x356B5678, //  4: ori   $t3, $t3, 0x5678
            0x316C00FF, //  5: andi  $t4, $t3, 0x00FF
            0x396D0F0F, //  6: xori  $t5, $t3, 0x0F0F
            0x290E0064, //  7: slti  $t6, $t0, 100
            0x2D0F0064, //  8: sltiu $t7, $t0, 100
            0x01098020, //  9: add   $s0, $t0, $t1
            0x01288822, // 10: sub   $s1, $t1, $t0
            0x01099024, // 11: and   $s2, $t0, $t1
            0x01099825, // 12: or    $s3, $t0, $t1
            0x0109A026, // 13: xor   $s4, $t0, $t1
            0x0109A82A, // 14: slt   $s5, $t0, $t1
            0x0128B02B, // 15: sltu  $s6, $t1, $t0
            0x0008B900, // 16: sll   $s7, $t0, 4
            0x0008C102, // 17: srl   $t8, $t0, 4
            0x0009C883, // 18: sra   $t9, $t1, 2
            0x0128D004, // 19: sllv  $k0, $t0, $t1
            0x0128D806, // 20: srlv  $k1, $t0, $t1
            0x0109E007, // 21: srav  $gp, $t1, $t0
            0x0109E821, // 22: addu  $sp, $t0, $t1
            0x0128F023, // 23: subu  $fp, $t1, $t0
            0x01090018, // 24: mult  $t0, $t1
            0x01090019, // 25: multu $t0, $t1
            0x0109001A, // 26: div   $t0, $t1
            0x0109001B, // 27: divu  $t0, $t1
            0xAC080000, // 28: sw    $t0, 0($zero)
            0x8C040000, // 29: lw    $a0, 0($zero)
            0xA0080000, // 30: sb    $t0, 0($zero)
            0x11090002, // 31: beq   $t0, $t1, +2   (42 != 58)
            0x15080002, // 32: bne   $t0, $t0, +2   (iguales)
            0x1A000001, // 33: blez  $s0, +1        ($s0 = 100 > 0)
            0x1C000001, // 34: bgtz  $zero, +1      (0 > 0 falso)
            0x08000026, // 35: j     38   (salta jal/jr y aterriza en mfhi: targets en rango)
            0x0C000026, // 36: jal   38   (en rango; no se ejecuta porque el j de arriba lo salta)
            0x03E00008, // 37: jr    $ra
            0x00001010, // 38: mfhi  $v0
            0x00001812, // 39: mflo  $v1
            0x84050000, // 40: lh    $a1, 0($zero)
            0x90060000, // 41: lbu   $a2, 0($zero)
            0x94070000, // 42: lhu   $a3, 0($zero)
            0xA4080000, // 43: sh    $t0, 0($zero)
        };
    }

    PrintBanner(cliArgs.getProgramPath(), instructions->size(), usedDefaultProgram);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << std::format("SDL_Init failed: '{}'\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    struct SDLGuard
    {
        ~SDLGuard() { SDL_Quit(); }
    } sdlGuard;

    CPU *cpu = new CPU(*instructions);
    VGATextWindow window(VGA_WINDOW_WIDTH, VGA_WINDOW_HEIGHT, cpu->getKeypad());

    if (!window.initDisplay(cpu->getFramebuffer(), (cliArgs.getFontPath().c_str()), kDefaultVGAPalette))
    {
        std::cerr << "Failed to initialize VGA display.\n";
        return EXIT_FAILURE;
    }

    std::cout << ">> Simulacion en curso... (cierra la ventana VGA para terminar)\n";

    // Silenciar el log por-instruccion mientras corre (evita flood/lag en bucles).
    std::streambuf *coutBuf = std::cout.rdbuf();
    std::cout.rdbuf(nullptr);

    // Hilo de CPU (productor de frames); el hilo principal maneja SDL (consumidor).
    std::atomic<bool> stop{false};
    std::thread cpuThread([&]
                          { cpu->Run(stop); });

    while (!window.quitRequested())
    {
        window.poll();   // eventos SDL -> keypad / quit
        window.redraw(); // swapAndGetFront + presentar el frame publicado
        SDL_Delay(16);
    }

    stop.store(true, std::memory_order_release);
    cpuThread.join();

    std::cout.rdbuf(coutBuf); // restaurar salida
    std::cout << ">> Simulacion finalizada.\n";
    PrintRegisters(cpu->getRegisterFile());

    return EXIT_SUCCESS;
}