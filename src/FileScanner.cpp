#include "FileScanner.hpp"
#include <algorithm>

namespace fileflow {

FileScanner::FileScanner(const std::filesystem::path& directory) noexcept
    : directory_(directory) {
    if (!std::filesystem::exists(directory_, error_)) {
        error_ = std::make_error_code(std::errc::no_such_file_or_directory);
        return;
    }

    if (!std::filesystem::is_directory(directory_, error_)) {
        error_ = std::make_error_code(std::errc::not_a_directory);
        return;
    }

    if (error_) {
        return;
    }

    error_.clear();
}

bool FileScanner::isValid() const noexcept {
    return !error_;
}

std::error_code FileScanner::getError() const noexcept {
    return error_;
}

std::string FileScanner::getErrorMessage() const noexcept {
    if (error_) {
        return error_.message();
    }
    return "";
}

std::vector<FileEntry> FileScanner::scan() const noexcept {
    std::vector<FileEntry> files;

    if (!isValid()) {
        return files;
    }

    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(directory_, ec)) {
        if (ec) {
            break;
        }

        if (!entry.is_regular_file(ec)) {
            continue;
        }

        if (ec) {
            continue;
        }

        FileEntry file_entry;
        file_entry.path = entry.path();
        file_entry.filename = entry.path().filename().string();

        std::string ext = entry.path().extension().string();
        if (!ext.empty()) {
            file_entry.extension = ext;
        }

        file_entry.size = entry.file_size(ec);
        if (ec) {
            file_entry.size = 0;
            ec.clear();
        }

        files.push_back(std::move(file_entry));
    }

    std::sort(files.begin(), files.end(),
              [](const FileEntry& a, const FileEntry& b) {
                  return a.filename < b.filename;
              });

    return files;
}

} // namespace fileflow