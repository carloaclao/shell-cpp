cmake_minimum_required(VERSION 3.13)
project(shell-starter-cpp CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emit build/compile_commands.json so clangd stops reporting
# 'gtest/gtest.h' file not found in nvim.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Dependencies -------------------------------------------------------
# Must come before anything references gtest_main: MakeAvailable is what
# downloads GoogleTest and defines that target.
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Full path beats a bare "readline" (-lreadline), which fails on macOS where
# Homebrew keeps readline keg-only, outside the default search paths.
# REQUIRED fails at configure time with a clear message, not at link time.
find_library(READLINE_LIB readline REQUIRED)
find_path(READLINE_INCLUDE_DIR readline/readline.h REQUIRED)

# ---- Application sources ------------------------------------------------
# CONFIGURE_DEPENDS re-checks the glob each build, so a new .cpp isn't
# silently omitted until you reconfigure by hand.
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER APP_SOURCES EXCLUDE REGEX "src/tests/.*")

# main.cpp must stay out of the library: shell_tests links gtest_main, which
# brings its own main(), and two of them is a duplicate symbol error.
list(FILTER APP_SOURCES EXCLUDE REGEX "src/main\\.cpp$")

# ---- Library with the real logic ----------------------------------------
# Both executables link this, so utilities.o ends up in each of them.
add_library(shell_core ${APP_SOURCES})
target_include_directories(shell_core PUBLIC src ${READLINE_INCLUDE_DIR})
target_link_libraries(shell_core PUBLIC ${READLINE_LIB})

# ---- Executables --------------------------------------------------------
add_executable(shell src/main.cpp)
target_link_libraries(shell PRIVATE shell_core)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS src/tests/*.cpp)
add_executable(shell_tests ${TEST_SOURCES})
target_link_libraries(shell_tests PRIVATE shell_core gtest_main)

# ---- ctest --------------------------------------------------------------
# Registers each TEST() individually so `ctest --test-dir build` works.
enable_testing()
include(GoogleTest)
gtest_discover_tests(shell_tests)
