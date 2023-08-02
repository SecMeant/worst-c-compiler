cmake_minimum_required(VERSION 3.20.0)
project(wcc CXX)

set(CMAKE_CXX_STANDARD 17)
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

set(SRC_FILES_CXX
    ${SRC_DIR}/tokenizer.cc
    ${SRC_DIR}/nfa.cc
    ${SRC_DIR}/parser.cc
)

add_library(libwcc STATIC ${SRC_FILES_CXX})
target_include_directories(libwcc PUBLIC ${INC_DIR})
add_definitions(-D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

target_link_libraries(
    libwcc
    INTERFACE ${fmt_LIBRARIES}
    INTERFACE ${mipc_LIBRARIES}
    INTERFACE spdlog::spdlog
)

add_executable(wcc ${SRC_DIR}/wcc.cc)
target_link_libraries(wcc libwcc LLVM-15)

add_executable(frontend_test test/run_tests.cc test/operator_precedence_test.cc)
target_include_directories(frontend_test PUBLIC ${INC_DIR})
target_link_libraries(frontend_test libwcc)
add_test(NAME frontend_test COMMAND frontend_test) 
enable_testing()

