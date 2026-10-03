#include "FileOrganizer.hpp"
#include <algorithm>

namespace fileflow {

FileOrganizer::FileOrganizer(const std::filesystem::path& base_directory,
                             const CategoryManager& category_manager) noexcept
    : base_directory_(base_directory)
    , category_manager_(category_manager) {}

OrganizeResult FileOrganizer::organize(bool dry_run) noexcept {
    OrganizeResult result;

    FileScanner scanner(base_directory_);
    if (!scanner.isValid()) {
        result.errors = 1;
        result.error_messages.push_back(scanner.getErrorMessage());
        return result;
    }

    auto files = scanner.scan();
    result.scanned = files.size();

    if (files.empty()) {
        return result;
    }

    for (const auto& file : files) {
        std::filesystem::path destination = resolveDestination(file, dry_run);

        if (dry_run) {
            result.moves.emplace_back(file.path.string(), destination.string());
            ++result.moved;
            continue;
        }

        std::error_code ec;

        if (!createDirectoryIfNeeded(destination.parent_path(), ec)) {
            ++result.errors;
            result.error_messages.emplace_back(
                "Failed to create directory for '" + file.filename + "': " + ec.message());
            continue;
        }

        std::filesystem::rename(file.path, destination, ec);
        if (ec) {
            ++result.errors;
            result.error_messages.emplace_back(
                "Failed to move '" + file.filename + "': " + ec.message());
            continue;
        }

        result.moves.emplace_back(file.path.string(), destination.string());
        ++result.moved;
    }

    return result;
}

std::filesystem::path FileOrganizer::resolveDestination(const FileEntry& file,
                                                         bool dry_run) const noexcept {
    CategoryManager::Category category = category_manager_.getCategory(file.extension);
    std::string category_name = std::string(category_manager_.getCategoryName(category));

    std::filesystem::path category_dir = base_directory_ / category_name;

    std::filesystem::path destination = category_dir / file.filename;

    if (!dry_run && std::filesystem::exists(destination)) {
        destination = generateUniquePath(destination);
    }

    return destination;
}

std::filesystem::path FileOrganizer::generateUniquePath(const std::filesystem::path& destination) const noexcept {
    std::filesystem::path parent = destination.parent_path();
    std::string stem = destination.stem().string();
    std::string extension = destination.extension().string();

    std::size_t counter = 1;
    std::filesystem::path new_path;

    do {
        std::string new_filename = stem + "_" + std::to_string(counter) + extension;
        new_path = parent / new_filename;
        ++counter;
    } while (std::filesystem::exists(new_path));

    return new_path;
}

bool FileOrganizer::createDirectoryIfNeeded(const std::filesystem::path& path,
                                             std::error_code& ec) const noexcept {
    if (std::filesystem::exists(path, ec)) {
        if (ec) {
            return false;
        }
        return std::filesystem::is_directory(path, ec);
    }

    std::filesystem::create_directories(path, ec);
    return !ec;
}

} // namespace fileflow