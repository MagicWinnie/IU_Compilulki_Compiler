cmake_minimum_required(VERSION 3.29)
project(IU_Compilulki_Compiler)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_executable(compilulki src/main.cpp
        src/settings/settings.cpp
        src/settings/settings.h
        src/lexical_analyzer/lexer.cpp
        src/lexical_analyzer/lexer.h
        src/lexical_analyzer/token.h
        src/lexical_analyzer/token.cpp
        src/lexical_analyzer/span.cpp
        src/lexical_analyzer/span.h
        src/lexical_analyzer/token_code.h
        src/syntax_analyzer/AST.h
        src/syntax_analyzer/Parser.h
        src/syntax_analyzer/Parser.cpp
        src/syntax_analyzer/AST.cpp
        src/syntax_analyzer/PrintVisitor.cpp
        src/syntax_analyzer/PrintVisitor.h
        src/utils/helper.cpp
        src/semantics_analyzer/SymbolTable.cpp
        src/semantics_analyzer/SymbolTable.h
        src/semantics_analyzer/SymbolTableVisitor.cpp
        src/semantics_analyzer/SymbolTableVisitor.h
        src/semantics_analyzer/OptimizeVisitor.cpp
        src/code_generator/CodegenVisitor.cpp
        src/code_generator/CodegenVisitor.h
)

llvm_map_components_to_libnames(llvm_libs support core irreader linker)
target_link_libraries(compilulki ${llvm_libs})