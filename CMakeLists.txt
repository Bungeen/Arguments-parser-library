cmake_minimum_required(VERSION 3.12)

project(
    ArgumentsParser
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(lib)
add_subdirectory(examples)


enable_testing()
add_subdirectory(tests)
