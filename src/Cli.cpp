#include "Cli.hpp"
#include "CategoryManager.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>

namespace fileflow {

Cli::Cli(int argc, char* argv[]) {
    args_.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args_.emplace_back(argv[i]);
    }
    parse();
}

const CliOptions& Cli::getOptions() const noexcept {
    return options_;
}

void Cli::parse() noexcept {
    if (args_.size() <= 1) {
        options_.command = Command::Help;
        options_.show_help = true;
        return;
    }

    options_.command = parseCommand(args_[1]);

    if (options_.command == Command::Help) {
        options_.show_help = true;
        return;
    }

    if (options_.command == Command::Invalid) {
        options_.error_message = "Unknown command: " + args_[1] + "\nUse 'fileflow help' for usage information.";
        return;
    }

    if (options_.command == Command::Organize) {
        if (args_.size() < 3) {
            options_.error_message = "Missing path argument for 'organize' command.\nUsage: fileflow organize <path> [--dry-run]";
            options_.command = Command::Invalid;
            return;
        }

        options_.target_path = args_[2];

        for (std::size_t i = 3; i < args_.size(); ++i) {
            if (args_[i] == "--dry-run" || args_[i] == "-d") {
                options_.dry_run = true;
            } else if (args_[i] == "--help" || args_[i] == "-h") {
                options_.show_help = true;
                break;
            } else {
                options_.error_message = "Unknown option: " + args_[i];
                options_.command = Command::Invalid;
                return;
            }
        }
    }
}

Command Cli::parseCommand(std::string_view cmd) const noexcept {
    if (cmd == "help" || cmd == "--help" || cmd == "-h") {
        return Command::Help;
    }
    if (cmd == "organize") {
        return Command::Organize;
    }
    return Command::Invalid;
}

void Cli::printHelp() const noexcept {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════╗
║                    FileFlow v1.0.0                           ║
║              Organize. Clean. Simplify.                      ║
╚══════════════════════════════════════════════════════════════╝

USAGE:
    fileflow <command> [arguments]

COMMANDS:
    organize <path> [--dry-run]    Organize files in the specified directory
    help                           Show this help message

OPTIONS:
    --dry-run, -d                  Simulate organization without making changes
    --help, -h                     Show help for the command

EXAMPLES:
    fileflow organize "C:\Users\Name\Downloads"
    fileflow organize /home/user/Downloads --dry-run
    fileflow help

CATEGORIES:
    Images       : jpg, jpeg, png, gif, bmp, webp, svg, ico
    Videos       : mp4, mkv, avi, mov, wmv, flv, webm
    Audio        : mp3, wav, flac, aac, ogg, m4a
    Documents    : pdf, doc, docx, txt, rtf, odt
    Spreadsheets : xls, xlsx, csv
    Presentations: ppt, pptx
    Archives     : zip, rar, 7z, tar, gz, bz2
    Code         : cpp, hpp, c, h, py, js, ts, jsx, tsx, java, rs, go, php, html, css
    Others       : Files with unknown or no extension

NOTES:
    • Only files directly in the target directory are organized (no recursion).
    • Existing category folders are not moved or scanned.
    • If a file with the same name exists in the destination, a numeric suffix
      is added (e.g., file_1.txt, file_2.txt).
    • File contents, timestamps, and permissions are never modified.
)" << std::flush;
}

void Cli::printVersion() const noexcept {
    std::cout << "FileFlow v1.0.0\n" << std::flush;
}

void Cli::printCategories() const noexcept {
    CategoryManager cm;
    std::cout << "\nSupported Categories:\n";
    std::cout << "────────────────────────────────────────────\n";

    for (const auto& cat_name : cm.getAllCategories()) {
        CategoryManager::Category cat;
        if (cat_name == "Images") cat = CategoryManager::Category::Images;
        else if (cat_name == "Videos") cat = CategoryManager::Category::Videos;
        else if (cat_name == "Audio") cat = CategoryManager::Category::Audio;
        else if (cat_name == "Documents") cat = CategoryManager::Category::Documents;
        else if (cat_name == "Spreadsheets") cat = CategoryManager::Category::Spreadsheets;
        else if (cat_name == "Presentations") cat = CategoryManager::Category::Presentations;
        else if (cat_name == "Archives") cat = CategoryManager::Category::Archives;
        else if (cat_name == "Code") cat = CategoryManager::Category::Code;
        else if (cat_name == "Others") cat = CategoryManager::Category::Others;
        else continue;

        auto exts = cm.getExtensionsForCategory(cat);
        std::cout << "  " << std::left << std::setw(16) << cat_name << ": ";
        for (std::size_t i = 0; i < exts.size(); ++i) {
            std::cout << exts[i];
            if (i + 1 < exts.size()) std::cout << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "────────────────────────────────────────────\n\n";
}

void Cli::printResult(const OrganizeResult& result, bool dry_run) const noexcept {
    std::string mode_label = dry_run ? "[DRY RUN] " : "";

    std::cout << "\n" << mode_label << "Organization Complete\n";
    std::cout << "═══════════════════════════════════════\n";

    if (dry_run) {
        std::cout << "The following changes WOULD be made:\n\n";
        for (const auto& [src, dst] : result.moves) {
            std::cout << "  " << src << "  →  " << dst << "\n";
        }
    } else {
        std::cout << "Files processed:\n";
        for (const auto& [src, dst] : result.moves) {
            std::cout << "  [OK] " << src << "  →  " << dst << "\n";
        }
    }

    std::cout << "\nSummary:\n";
    std::cout << "  Files scanned : " << result.scanned << "\n";
    std::cout << "  Files moved   : " << result.moved << "\n";
    std::cout << "  Files skipped : " << result.skipped << "\n";
    std::cout << "  Errors        : " << result.errors << "\n";

    if (!result.error_messages.empty()) {
        std::cout << "\nErrors:\n";
        for (const auto& err : result.error_messages) {
            std::cout << "  [ERROR] " << err << "\n";
        }
    }

    if (dry_run) {
        std::cout << "\n[DRY RUN] No files were actually moved.\n";
    }
    std::cout << std::flush;
}

} // namespace fileflow