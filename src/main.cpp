#include "CategoryManager.hpp"
#include "Cli.hpp"
#include "FileOrganizer.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        fileflow::Cli cli(argc, argv);
        const auto& options = cli.getOptions();

        if (options.show_help || options.command == fileflow::Command::Help) {
            cli.printHelp();
            return 0;
        }

        if (options.command == fileflow::Command::Invalid) {
            if (!options.error_message.empty()) {
                std::cerr << "Error: " << options.error_message << "\n\n";
            }
            cli.printHelp();
            return 1;
        }

        if (options.command == fileflow::Command::Organize) {
            if (!std::filesystem::exists(options.target_path)) {
                std::cerr << "Error: Path does not exist: " << options.target_path << "\n";
                return 1;
            }

            fileflow::CategoryManager category_manager;
            fileflow::FileOrganizer organizer(options.target_path, category_manager);

            auto result = organizer.organize(options.dry_run);
            cli.printResult(result, options.dry_run);

            return result.errors > 0 ? 1 : 0;
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred\n";
        return 1;
    }
}