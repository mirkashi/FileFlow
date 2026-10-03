#pragma once

#include "FileOrganizer.hpp"
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace fileflow {

enum class Command {
    Help,
    Organize,
    Invalid
};

struct CliOptions {
    Command command = Command::Invalid;
    std::filesystem::path target_path;
    bool dry_run = false;
    bool show_help = false;
    std::string error_message;
};

class Cli {
public:
    explicit Cli(int argc, char* argv[]);

    [[nodiscard]] const CliOptions& getOptions() const noexcept;

    void printHelp() const noexcept;
    void printVersion() const noexcept;
    void printCategories() const noexcept;
    void printResult(const OrganizeResult& result, bool dry_run) const noexcept;

private:
    CliOptions options_;
    std::vector<std::string> args_;

    void parse() noexcept;
    [[nodiscard]] Command parseCommand(std::string_view cmd) const noexcept;
};

} // namespace fileflow