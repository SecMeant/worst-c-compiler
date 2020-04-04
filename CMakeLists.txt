project(wcc CXX)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(fmt REQUIRED fmt)

set(SRC_DIR src)
set(INC_DIR inc)

file(GLOB_RECURSE SRC_FILES_CXX ${SRC_DIR}/*.cc)

add_executable(wcc ${SRC_FILES_CXX})
target_include_directories(wcc PUBLIC ${INC_DIR})
target_link_libraries(wcc ${fmt_LIBRARIES})