#pragma once

#include "CategoryManager.hpp"
#include "FileScanner.hpp"
#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

namespace fileflow {

struct OrganizeResult {
    std::size_t scanned = 0;
    std::size_t moved = 0;
    std::size_t skipped = 0;
    std::size_t errors = 0;
    std::vector<std::string> error_messages;
    std::vector<std::pair<std::string, std::string>> moves;
};

class FileOrganizer {
public:
    explicit FileOrganizer(const std::filesystem::path& base_directory,
                           const CategoryManager& category_manager) noexcept;

    OrganizeResult organize(bool dry_run = false) noexcept;

private:
    std::filesystem::path base_directory_;
    const CategoryManager& category_manager_;

    [[nodiscard]] std::filesystem::path resolveDestination(const FileEntry& file,
                                                            bool dry_run) const noexcept;
    [[nodiscard]] std::filesystem::path generateUniquePath(const std::filesystem::path& destination) const noexcept;
    [[nodiscard]] bool createDirectoryIfNeeded(const std::filesystem::path& path, std::error_code& ec) const noexcept;
};

} // namespace fileflow