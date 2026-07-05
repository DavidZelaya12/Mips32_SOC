#include <iostream>
#include <format>
#include "CliArgs.h"

void CliArgs::printUsage()
{
    std::cout << "Usage:\n"
              << "Options:\n"
              << "  --program <file_path>   Path to the machine code file (required)\n"
              << "  --font <file_path>      Path to the font file (required)\n"
              << "  --data <file_path>      Path to the data file (optional)\n";
}

std::optional<std::string> CliArgs::parse()
{
    for (int i = 0; i < argc_; i++)
    {
        if (std::strcmp(argv_[i], "--program") == 0)
        {
            if (i + 1 == argc_)
            {
                return std::string("Option '--program' requires a file path");
            }
            machine_code = argv_[++i];
        }
        else if (std::strcmp(argv_[i], "--font") == 0)
        {
            if (i + 1 == argc_)
            {
                return std::string("Option '--font' requires a file path");
            }
            fontpath_ = argv_[++i];
        }
        else if (std::strcmp(argv_[i], "--data") == 0)
        {
            if (i + 1 == argc_)
            {
                return std::string("Option '--data' requires a file path");
            }
            datafilepath_ = argv_[++i];
        }
        else
        {
            return std::format("Invalid option '{}'", argv_[i]);
        }
    }

    if (machine_code.empty())
    {
        return std::string("Missing required option '--program <file_path>'");
    }
    if (fontpath_.empty())
    {
        return std::string("Missing required option '--font <file_path>'");
    }
    if (datafilepath_ != std::nullopt && datafilepath_->empty())
    {
        return std::string("Option '--data' requires a non-empty file path");
    }

    return std::nullopt;
}
