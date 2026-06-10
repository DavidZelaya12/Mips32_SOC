#include <iostream>
#include "CliArgs.hpp"

int main(int argc, const char *argv[])
{
    CliArgs cliArgs(argc, argv);
    auto error = cliArgs.parse();

    if (error == std::nullopt)
    {
        std::cout << "Program path: " << cliArgs.getProgramPath() << "\n";
        std::cout << "Font path: " << cliArgs.getFontPath() << "\n";
        if (auto dataFilePath = cliArgs.getDataFilePath())
        {
            std::cout << "Data file path: " << *dataFilePath << "\n";
        }
        else
        {
            std::cout << "Data file path: Not provided\n";
        }
    }
    else
    {
        std::cerr << "Error: " << *error << "\n\n";
        return 1;
    }

    return 0;
}