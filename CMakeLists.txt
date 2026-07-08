cmake_minimum_required(VERSION 3.20)

project(ish VERSION 0.1.0 LANGUAGES C)

add_executable(ish
    Compilers.c
    MainReader.c
    MainScanner.c
    MainParser.c
    Parser.c
    Reader.c
    Scanner.c
    SymbolTable.c
)

target_compile_features(ish PRIVATE c_std_11)

if(MSVC)
    target_compile_options(ish PRIVATE /W4)
else()
    target_compile_options(ish PRIVATE -Wall -Wextra -pedantic)
endif()

enable_testing()

add_test(NAME reader_hello COMMAND ish r ${CMAKE_CURRENT_SOURCE_DIR}/examples/hello.ish)
add_test(NAME scanner_smoke COMMAND ish s ${CMAKE_CURRENT_SOURCE_DIR}/examples/scanner-smoke.ish)
add_test(NAME parser_hello COMMAND ish p ${CMAKE_CURRENT_SOURCE_DIR}/examples/hello.ish)
add_test(NAME parser_volume COMMAND ish p ${CMAKE_CURRENT_SOURCE_DIR}/examples/volume.ish)
add_test(NAME parser_factorial COMMAND ish p ${CMAKE_CURRENT_SOURCE_DIR}/examples/factorial.ish)
add_test(NAME semantic_undefined_variable COMMAND ish p ${CMAKE_CURRENT_SOURCE_DIR}/examples/semantic-error.ish)
set_tests_properties(semantic_undefined_variable PROPERTIES PASS_REGULAR_EXPRESSION "undefined variable 'missingValue'")
