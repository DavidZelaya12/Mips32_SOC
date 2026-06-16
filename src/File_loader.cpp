#include <optional>
#include <fstream>
#include <vector>
#include <cstdint>

static std::optional<std::vector<std::uint32_t>> Loadinstructions(const std::string &filepath)
{
    std::vector<std::uint32_t> instructions = {};
    std::ifstream file(filepath, std::ios::binary);

    if (!file)
    {
        return std::nullopt;
    }
    std::uint32_t instruction;
    while (file.read(reinterpret_cast<char *>(&instruction), sizeof(instruction)))
    {
        instructions.push_back(instruction);
    }

    if (instructions.empty())
    {
        return std::nullopt;
    }

    return instructions;
}