#include <iostream>


#include "lexical_analyzer/lexer.h"
#include "semantics_analyzer/OptimizeVisitor.h"
#include "syntax_analyzer/Parser.h"
#include "settings/settings.h"
#include "semantics_analyzer/SymbolTableVisitor.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Linker/Linker.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"


void loadCustomIR(llvm::LLVMContext &context, llvm::Module &module, const std::string &filePath) {
    llvm::SMDiagnostic err;
    auto loadedModule = llvm::parseIRFile(filePath, err, context);

    if (!loadedModule) {
        err.print("IR Loading Error", llvm::errs());
        return;
    }

    // Merge loaded module with your existing module
    if (llvm::Linker::linkModules(module, std::move(loadedModule))) {
        llvm::errs() << "Error linking module!\n";
    }
}

int main(const int argc, char* argv[])
{

    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    auto module = std::make_unique<llvm::Module>("compilul'ki", context);

    loadCustomIR(context, *module.get(), "memory.ll");
    loadCustomIR(context, *module.get(), "boolean.ll");
    loadCustomIR(context, *module.get(), "integer.ll");
    loadCustomIR(context, *module.get(), "real.ll");
    loadCustomIR(context, *module.get(), "int_array.ll");
    auto settings = Settings();
    settings.parse(argc, argv);
    settings.process();

    if (settings.get_debug())
    {
        std::cout << settings << std::endl;
    }

    auto lexer = Lexer(settings.get_infile(), settings.get_debug());
    auto tokens = lexer.parse();

    auto parser = Parser(std::move(tokens), settings.get_infile(), settings.get_debug());
    std::unique_ptr<Program> program = parser.parse();

    SymbolTableVisitor symbolTableVisitor;
    program->accept(symbolTableVisitor);

    std::cout << "\033[33mUnused variables in this scope: ";
    for (const auto& name : symbolTableVisitor.symbolTable.unusedVariables)
    {
        std::cout << name << " ";
    }
    std::cout << "\033[0m" << std::endl;

    OptimizeVisitor optimizeVisitor(symbolTableVisitor.symbolTable);
    program->accept(optimizeVisitor);

    parser.toFile(".semantics", program);

    program->codegen(context, builder, *module.get(), symbolTableVisitor.symbolTable);

    std::error_code EC;
    if(settings.get_debug())
    {
        std::cout << "\n\n\nCode generated" << std::endl;
        module->print(llvm::outs(), nullptr);
    }


    llvm::raw_fd_ostream dest("output.ll", EC);
    module->print(dest, nullptr);

    std::cout<<"\nRESULT:\n";

    int result = std::system("lli output.ll");

    if (result != 0) {
        std::cerr << "Execution failed with code: " << result << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
