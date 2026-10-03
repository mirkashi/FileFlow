#pragma once

#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

namespace fileflow {

struct FileEntry {
    std::filesystem::path path;
    std::string filename;
    std::string extension;
    std::uintmax_t size = 0;
};

class FileScanner {
public:
    explicit FileScanner(const std::filesystem::path& directory) noexcept;

    [[nodiscard]] bool isValid() const noexcept;
    [[nodiscard]] std::error_code getError() const noexcept;
    [[nodiscard]] std::string getErrorMessage() const noexcept;

    [[nodiscard]] std::vector<FileEntry> scan() const noexcept;

private:
    std::filesystem::path directory_;
    std::error_code error_;
};

} // namespace fileflow