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
    loadCustomIR(context, *module.get(), "int_list.ll");
//    loadCustomIR(context, *module.get(), "real_array.ll");
//    loadCustomIR(context, *module.get(), "real_list.ll");
//    loadCustomIR(context, *module.get(), "bool_array.ll");
//    loadCustomIR(context, *module.get(), "bool_list.ll");
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


    std::string output_filename = settings.get_output_filename();
    llvm::raw_fd_ostream dest(output_filename + ".ll", EC);
    module->print(dest, nullptr);

    std::cout<<"\nRESULT:\n";

    std::string optimizerCommand = "opt " + settings.get_optimization_level() + " "+output_filename+".ll -o "+output_filename+".bc";

    std::system(optimizerCommand.c_str());

    std::string llcCommand = "llc -filetype=obj "+output_filename+".ll -o "+output_filename+".o";
    std::system(llcCommand.c_str());

    std::string clangCommand = "clang "+output_filename+".o -o "+output_filename;
    std::system(clangCommand.c_str());


    // Remove the intermediate files
    std::string rmCommand = "rm "+output_filename+".o "+output_filename+".bc";
    std::system(rmCommand.c_str());
    if(!settings.get_debug()){
        rmCommand = "rm "+output_filename+".ll";
        std::system(rmCommand.c_str());
    }
    std::string runCommand = "./"+output_filename;
    int result = std::system(runCommand.c_str());

    if (result != 0) {
        std::cerr << "Execution failed with code: " << result << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
