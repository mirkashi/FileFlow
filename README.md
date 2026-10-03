# FileFlow

> **Organize. Clean. Simplify.**

A fast, safe, and dependency-free command-line utility written in modern C++20 that automatically organizes files in a directory into categorized subfolders based on their file extensions.

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.20%2B-orange.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

---

## Features

- **Zero Dependencies** — Uses only the C++ Standard Library (`std::filesystem`, `std::unordered_map`, etc.)
- **Safe by Default** — Never overwrites files; resolves collisions with numeric suffixes (`file_1.txt`, `file_2.txt`)
- **Dry-Run Mode** — Preview all changes before applying them
- **No Recursion** — Only processes files directly in the target directory; ignores subdirectories
- **Robust Error Handling** — Continues processing even if individual files fail (permissions, locks, etc.)
- **Cross-Platform** — Works on Windows, Linux, and macOS
- **Case-Insensitive** — Handles `.JPG`, `.jpg`, `.Jpg` identically

---

## Supported Categories

| Category | Extensions |
|----------|------------|
| **Images** | `.jpg`, `.jpeg`, `.png`, `.gif`, `.bmp`, `.webp`, `.svg`, `.ico` |
| **Videos** | `.mp4`, `.mkv`, `.avi`, `.mov`, `.wmv`, `.flv`, `.webm` |
| **Audio** | `.mp3`, `.wav`, `.flac`, `.aac`, `.ogg`, `.m4a` |
| **Documents** | `.pdf`, `.doc`, `.docx`, `.txt`, `.rtf`, `.odt` |
| **Spreadsheets** | `.xls`, `.xlsx`, `.csv` |
| **Presentations** | `.ppt`, `.pptx` |
| **Archives** | `.zip`, `.rar`, `.7z`, `.tar`, `.gz`, `.bz2` |
| **Code** | `.cpp`, `.hpp`, `.c`, `.h`, `.py`, `.js`, `.ts`, `.jsx`, `.tsx`, `.java`, `.rs`, `.go`, `.php`, `.html`, `.css` |
| **Others** | Any file with an unknown or missing extension |

---

## Installation

### Prerequisites

- **CMake** 3.20 or higher
- **C++20 compatible compiler**:
  - MSVC 19.30+ (Visual Studio 2022 17.3+)
  - GCC 11+
  - Clang 13+

### Build from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/FileFlow.git
cd FileFlow

# Configure (Debug build)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --config Debug

# Or build Release
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The executable will be located at:
- **Windows**: `build/bin/Debug/fileflow.exe` or `build/bin/Release/fileflow.exe`
- **Linux/macOS**: `build/bin/fileflow`

### Install System-Wide (Optional)

```bash
# Linux/macOS
sudo cmake --install build --prefix /usr/local

# Windows (run as Administrator)
cmake --install build --prefix "C:\Program Files\FileFlow"
```

---

## Usage

```bash
fileflow <command> [arguments]
```

### Commands

| Command | Description |
|---------|-------------|
| `organize <path> [--dry-run]` | Organize files in the specified directory |
| `help` | Show help message |

### Options

| Option | Short | Description |
|--------|-------|-------------|
| `--dry-run` | `-d` | Simulate organization without making changes |
| `--help` | `-h` | Show help for the command |

### Examples

```bash
# Organize Downloads folder
fileflow organize "C:\Users\Name\Downloads"

# Preview what would happen (safe, no changes)
fileflow organize "/home/user/Downloads" --dry-run

# Show help
fileflow help
```

---

## Output Examples

### Normal Mode

```text
╔══════════════════════════════════════════════════════════════╗
║                    FileFlow v1.0.0                           ║
║              Organize. Clean. Simplify.                      ║
╚══════════════════════════════════════════════════════════════╝

Organization Complete
═══════════════════════════════════════
Files processed:
  [OK] C:\Users\Name\Downloads\photo.jpg      →  C:\Users\Name\Downloads\Images\photo.jpg
  [OK] C:\Users\Name\Downloads\document.pdf   →  C:\Users\Name\Downloads\Documents\document.pdf
  [OK] C:\Users\Name\Downloads\script.py      →  C:\Users\Name\Downloads\Code\script.py

Summary:
  Files scanned : 3
  Files moved   : 3
  Files skipped : 0
  Errors        : 0
```

### Dry-Run Mode

```text
[DRY RUN] Organization Complete
═══════════════════════════════════════
The following changes WOULD be made:

  C:\Users\Name\Downloads\photo.jpg      →  C:\Users\Name\Downloads\Images\photo.jpg
  C:\Users\Name\Downloads\document.pdf   →  C:\Users\Name\Downloads\Documents\document.pdf
  C:\Users\Name\Downloads\script.py      →  C:\Users\Name\Downloads\Code\script.py

Summary:
  Files scanned : 3
  Files moved   : 3
  Files skipped : 0
  Errors        : 0

[DRY RUN] No files were actually moved.
```

---

## Architecture

```
FileFlow/
├── include/
│   ├── CategoryManager.hpp   # Extension-to-category mapping
│   ├── FileScanner.hpp       # Directory validation & file discovery
│   ├── FileOrganizer.hpp     # Directory creation, moving, collision handling
│   └── Cli.hpp               # Argument parsing & user interface
└── src/
    ├── main.cpp              # Entry point
    ├── CategoryManager.cpp
    ├── FileScanner.cpp
    ├── FileOrganizer.cpp
    └── Cli.cpp
```

### Component Responsibilities

| Component | Responsibility |
|-----------|----------------|
| **CategoryManager** | Normalizes extensions (case-insensitive), maps to categories |
| **FileScanner** | Validates target path, enumerates regular files (non-recursive) |
| **FileOrganizer** | Creates category dirs, moves files, resolves name collisions |
| **Cli** | Parses arguments, formats output, handles help/version |

---

## Safety Guarantees

1. **No Data Loss** — Files are moved, never deleted or modified
2. **No Overwrites** — Collisions resolved via `_1`, `_2`, ... suffixes
3. **No Recursion** — Subdirectories are completely ignored
4. **Atomic Operations** — Uses `std::filesystem::rename` for atomic moves where possible
5. **Error Resilience** — Individual file failures don't stop the entire process
6. **Metadata Preserved** — Timestamps, permissions, and contents remain unchanged

---

## Testing

```bash
# Run CTest suite
cd build
ctest --output-on-failure

# Manual integration test
mkdir -p /tmp/test_flow
cp *.jpg *.pdf *.cpp /tmp/test_flow/
fileflow organize /tmp/test_flow --dry-run
fileflow organize /tmp/test_flow
```

---

## Project Structure

```
FileFlow/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── include/                # Public headers
│   ├── CategoryManager.hpp
│   ├── FileScanner.hpp
│   ├── FileOrganizer.hpp
│   └── Cli.hpp
└── src/                    # Implementation
    ├── main.cpp
    ├── CategoryManager.cpp
    ├── FileScanner.cpp
    ├── FileOrganizer.cpp
    └── Cli.cpp
```

---

## Configuration

Currently, categories and extensions are hardcoded in `CategoryManager.cpp`. To customize, modify the `initializeMappings()` method and rebuild.

Future versions may support a configuration file (JSON/TOML) for user-defined categories.

---

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## License

Distributed under the MIT License. See `LICENSE` for more information.

---

## Acknowledgments

- Built with modern C++20 and `std::filesystem`
- Inspired by the need for a lightweight, dependency-free file organizer

---

**FileFlow** — *Organize. Clean. Simplify.*