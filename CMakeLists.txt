cmake_minimum_required(VERSION 3.5)
project(mymhorse C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

# out-of-source编译: CMAKE_CURRENT_BINARY_DIR指目标编译目录
# include_directories((${CMAKE_CURRENT_BINARY_DIR}/include))

add_subdirectory(lib/)
add_subdirectory(mid/)