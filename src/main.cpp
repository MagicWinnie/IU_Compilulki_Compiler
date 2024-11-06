#include <iostream>


#include "lexical_analyzer/lexer.h"
#include "semantics_analyzer/OptimizeVisitor.h"
#include "syntax_analyzer/Parser.h"
#include "settings/settings.h"
#include "semantics_analyzer/SymbolTableVisitor.h"


//llvm::Function *declarePrintf(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module) {
//    llvm::FunctionType *printfType = llvm::FunctionType::get(
//            builder.getInt32Ty(),                           // printf returns an int
//            llvm::PointerType::get(builder.getInt8Ty(), 0), // printf takes a char* (i8*)
//            true);                                          // printf is variadic
//    return llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module);
//}
//
//llvm::Function *createMainFunctionWithPrint(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module) {
//    // Declare `printf`
//    llvm::Function *printfFunc = declarePrintf(context,builder,module);
//
//    // Create the `main` function
//    llvm::FunctionType *mainType = llvm::FunctionType::get(builder.getInt32Ty(), false);
//    llvm::Function *mainFunc = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, "main", module);
//
//    // Create entry block and set insertion point
//    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
//    builder.SetInsertPoint(entry);
//
//    // Create a format string: "%f\n"
//    llvm::Value *formatStr = builder.CreateGlobalStringPtr("%f\n");
//
//    // Value to print (for example, 3.14)
//    llvm::Value *floatValue = llvm::ConstantFP::get(context, llvm::APFloat(3.14));
//
//    // Call `printf` with the format string and the value
//    builder.CreateCall(printfFunc, {formatStr, floatValue});
//
//    // Return 0
//    builder.CreateRet(builder.getInt32(0));
//
//    return mainFunc;
//}
//
//int main(const int argc, char* argv[])
//{
//    llvm::LLVMContext context;
//    llvm::IRBuilder<> builder(context);
//    auto module = std::make_unique<llvm::Module>("compilul'ki", context);
//
//    createMainFunctionWithPrint(context,builder,*module.get());
//
//
//
//
//    // Emit the IR
//    std::error_code EC;
//    llvm::raw_fd_ostream dest("output.ll", EC);
//    module->print(dest, nullptr);
//}

void createBooleanClass(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module &module) {
    // Define the Boolean class structure with one field (bool value)
    llvm::StructType *BooleanType = llvm::StructType::create(context, "Boolean");
    BooleanType->setBody(llvm::Type::getInt1Ty(context)); // i1 (boolean) field

    // Create the Boolean constructor: Boolean(Boolean)
    llvm::FunctionType *constructorType = llvm::FunctionType::get(
            llvm::Type::getVoidTy(context),        // Return statementType: void
            { BooleanType->getPointerTo(), llvm::Type::getInt1Ty(context) }, // Parameters: this pointer, boolean value
            false
    );

    llvm::Function *constructorFunc = llvm::Function::Create(
            constructorType,
            llvm::Function::ExternalLinkage,
            "Boolean_Constructor",
            module
    );

    // Entry block for constructor
    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context, "entry", constructorFunc);
    builder.SetInsertPoint(entryBlock);

    // Access 'this' pointer and boolean argument
    llvm::Value *thisPtr = constructorFunc->getArg(0);
    llvm::Value *boolValue = constructorFunc->getArg(1);

    // Store the boolean value in the class instance (this->value)
    llvm::Value *fieldPtr = builder.CreateStructGEP(BooleanType, thisPtr, 0, "boolFieldPtr");
    builder.CreateStore(boolValue, fieldPtr);

    // Return from constructor
    builder.CreateRetVoid();

    // Define the print method: void print()
    llvm::FunctionType *printType = llvm::FunctionType::get(
            llvm::Type::getVoidTy(context),        // Return statementType: void
            { BooleanType->getPointerTo() },       // Parameter: this pointer
            false
    );

    llvm::Function *printFunc = llvm::Function::Create(
            printType,
            llvm::Function::ExternalLinkage,
            "Boolean_print",
            module
    );

    // Create entry block for print method
    llvm::BasicBlock *printEntryBlock = llvm::BasicBlock::Create(context, "entry", printFunc);
    builder.SetInsertPoint(printEntryBlock);

    // Get the Boolean field value to print
    llvm::Value *thisPrintPtr = printFunc->getArg(0);
    llvm::Value *boolFieldPtr = builder.CreateStructGEP(BooleanType, thisPrintPtr, 0, "boolFieldPtr");
    llvm::Value *boolFieldVal = builder.CreateLoad(llvm::Type::getInt1Ty(context),boolFieldPtr, "loadBool");

    // Convert the bool (i1) to i32 (for printf)
    llvm::Value *boolAsInt = builder.CreateZExt(boolFieldVal, llvm::Type::getInt32Ty(context), "extBool");

    // Print the boolean value (using printf)
    llvm::Function *printfFunc = module.getFunction("printf");
    if (!printfFunc) {
        llvm::FunctionType *printfType = llvm::FunctionType::get(
                llvm::Type::getInt32Ty(context),
                llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), // printf takes a format string
                true // printf is variadic
        );
        printfFunc = llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module);
    }

    // Create format string for printf
    llvm::Value *formatStr = builder.CreateGlobalStringPtr("%d\n", "fmt");

    // Call printf
    builder.CreateCall(printfFunc, { formatStr, boolAsInt });

    // Return from print method
    builder.CreateRetVoid();
}


void define_standart_classes(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module){
    createBooleanClass(context, builder, module);

}

int main(const int argc, char* argv[])
{

    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    auto module = std::make_unique<llvm::Module>("compilul'ki", context);
    define_standart_classes(context, builder, *module.get());
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

    program->codegen(context, builder, *module.get());

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
