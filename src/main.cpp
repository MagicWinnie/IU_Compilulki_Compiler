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


void loadCustomIR(llvm::LLVMContext& context, llvm::Module& module, const std::string& filePath)
{
    llvm::SMDiagnostic err;
    auto loadedModule = parseIRFile("std/" + filePath, err, context);

    if (!loadedModule)
    {
        err.print("IR Loading Error", llvm::errs());
        return;
    }

    // Merge loaded module with your existing module
    if (llvm::Linker::linkModules(module, std::move(loadedModule)))
    {
        llvm::errs() << "Error linking module!\n";
    }
}

int main(const int argc, char* argv[])
{
    llvm::LLVMContext context;
    llvm::IRBuilder builder(context);
    auto module = std::make_unique<llvm::Module>("compilul'ki", context);

    loadCustomIR(context, *module.get(), "memory.ll");
    loadCustomIR(context, *module.get(), "boolean.ll");
    loadCustomIR(context, *module.get(), "integer.ll");
    loadCustomIR(context, *module.get(), "real.ll");
    loadCustomIR(context, *module.get(), "int_array.ll");
    loadCustomIR(context, *module.get(), "int_list.ll");
    // loadCustomIR(context, *module.get(), "real_array.ll");
    // loadCustomIR(context, *module.get(), "real_list.ll");
    // loadCustomIR(context, *module.get(), "bool_array.ll");
    // loadCustomIR(context, *module.get(), "bool_list.ll");

    auto settings = Settings();
    settings.parse(argc, argv);
    settings.process();
    if (settings.get_debug())
    {
        std::cout << settings << std::endl;
    }

    // LEXER
    auto lexer = Lexer(settings.get_infile(), settings.get_debug());
    auto tokens = lexer.parse();

    // PARSER
    auto parser = Parser(std::move(tokens), settings.get_infile(), settings.get_debug());
    std::unique_ptr<Program> program = parser.parse();

    // SEMANTICS
    SymbolTableVisitor symbolTableVisitor;
    program->accept(symbolTableVisitor);

    std::cout << "\033[0m" << std::endl;

    // OPTIMIZER
    OptimizeVisitor optimizeVisitor(symbolTableVisitor.symbolTable);
    program->accept(optimizeVisitor);

    parser.toFile(".semantics", program);

    program->codegen(context, builder, *module.get(), symbolTableVisitor.symbolTable);

    std::error_code EC;

    std::filesystem::path ll_path(settings.get_output_filename());
    ll_path.replace_extension(".ll");
    std::string ll_path_str = ll_path.string();

    std::filesystem::path bc_path(settings.get_output_filename());
    bc_path.replace_extension(".bc");
    std::string bc_path_str = bc_path.string();

    std::filesystem::path o_path(settings.get_output_filename());
    o_path.replace_extension(".o");
    std::string o_path_str = o_path.string();

    llvm::raw_fd_ostream dest(ll_path_str, EC);
    module->print(dest, nullptr);

    // Run optimizer
    std::string optimizerCommand = "opt " + settings.get_optimization_level() + " " +
        ll_path_str + " -o " + bc_path_str;
    // std::cout << optimizerCommand << std::endl;
    if (int result = std::system(optimizerCommand.c_str()); result != 0)
    {
        std::cerr << "Optimizer failed with code: " << result << std::endl;
        return EXIT_FAILURE;
    }

    // Run llc
    std::string llcCommand = "llc -filetype=obj " + ll_path_str + " -o " + o_path_str;
    // std::cout << llcCommand << std::endl;
    if (int result = std::system(llcCommand.c_str()); result != 0)
    {
        std::cerr << "LLC failed with code: " << result << std::endl;
        return EXIT_FAILURE;
    }

    // Run compile
    std::string clangCommand = "g++ -no-pie " + o_path_str + " -o " + settings.get_output_filename();
    // std::cout << clangCommand << std::endl;
    if (int result = std::system(clangCommand.c_str()); result != 0)
    {
        std::cerr << "Object compile failed with code: " << result << std::endl;
        return EXIT_FAILURE;
    }

    // Remove the intermediate files
    remove(o_path);
    remove(bc_path);
    if (!settings.get_debug())
    {
        remove(ll_path);
    }

    // Run binary file
    std::string runCommand = settings.get_output_filename();
    // std::cout << runCommand << std::endl;
    std::cout << "\nRESULT:\n";

    if (int result = std::system(runCommand.c_str()); result != 0)
    {
        std::cerr << "Execution failed with code: " << result << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
