cmake_minimum_required(VERSION 3.20)
project(FileFlow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive- /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi /Ob0 /Od /RTC1")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2 /Ob2 /DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wdouble-promotion -Wformat=2 -Wnull-dereference")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -flto")
endif()

add_executable(fileflow
    src/main.cpp
    src/CategoryManager.cpp
    src/FileScanner.cpp
    src/FileOrganizer.cpp
    src/Cli.cpp
)

target_include_directories(fileflow PRIVATE include)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(fileflow PRIVATE /W4 /permissive- /EHsc)
else()
    target_compile_options(fileflow PRIVATE
        -Wall -Wextra -Wpedantic
        -Wconversion -Wshadow
        -Wnon-virtual-dtor -Wold-style-cast
        -Wcast-align -Wunused -Woverloaded-virtual
        -Wdouble-promotion -Wformat=2
        -Wnull-dereference
    )
endif()

set_target_properties(fileflow PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(WIN32)
    set_target_properties(fileflow PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
    )
else()
    set_target_properties(fileflow PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
    )
endif()

install(TARGETS fileflow
    RUNTIME DESTINATION bin
)

include(CTest)
enable_testing()

add_test(NAME FileFlow_Help
    COMMAND fileflow help
)

add_test(NAME FileFlow_InvalidCommand
    COMMAND fileflow invalid_command
)

# Create a test directory structure for integration testing
add_custom_target(test_integration
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/test_dir
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/test_files/test.jpg ${CMAKE_BINARY_DIR}/test_dir/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/test_files/test.pdf ${CMAKE_BINARY_DIR}/test_dir/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/test_files/test.cpp ${CMAKE_BINARY_DIR}/test_dir/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/test_files/noext ${CMAKE_BINARY_DIR}/test_dir/
    COMMAND fileflow organize ${CMAKE_BINARY_DIR}/test_dir --dry-run
    COMMENT "Running integration test"
    VERBATIM
)