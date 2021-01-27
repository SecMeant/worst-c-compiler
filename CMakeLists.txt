project(wcc CXX)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)

add_compile_definitions(
  SPDLOG_FMT_EXTERNAL
)

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

find_package(PkgConfig REQUIRED)
find_package(spdlog REQUIRED)
pkg_check_modules(fmt REQUIRED fmt)
pkg_check_modules(mipc REQUIRED mipc>0.19)

set(SRC_DIR src)
set(INC_DIR inc)

set(CMAKE_CXX_FLAGS "-Wno-c99-designator")

file(GLOB_RECURSE SRC_FILES_CXX ${SRC_DIR}/*.cc)

add_executable(wcc ${SRC_FILES_CXX})
target_include_directories(wcc PUBLIC ${INC_DIR})
target_link_libraries(wcc ${fmt_LIBRARIES} ${mipc_LIBRARIES} spdlog::spdlog)
