#pragma once

#include <string>
#include <optional>
#include <cstring>

class CliArgs
{
public:
    CliArgs(int argc, const char *argv[])
        : argc_(argc - 1), argv_(argv + 1)
    {
    }

    std::string getProgramPath() const
    {
        return machine_code;
    }
    std::string getFontPath() const
    {
        return fontpath_;
    }
    std::optional<std::string> getDataFilePath() const
    {
        return (datafilepath_ != std::nullopt) ? std::make_optional(*datafilepath_) : std::nullopt;
    }

    void printUsage();

    [[nodiscard]] std::optional<std::string> parse();

private:
    int argc_;
    const char **argv_;
    std::string machine_code{};
    std::string fontpath_{};
    std::optional<std::string> datafilepath_{};
};