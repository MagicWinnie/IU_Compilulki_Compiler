#include <iostream>
#include <utility>
#include "SymbolTable.h"


VariableEntry::VariableEntry(std::string name, std::string type): name(std::move(name)), type(std::move(type))
{
}

VariableEntry::VariableEntry(std::string name, std::string type, const bool is_used): name(std::move(name)),
    type(std::move(type)),
    is_used(is_used)
{
}

MethodSignature::MethodSignature(std::string methodName, std::vector<std::string> parameterTypes):
    methodName(std::move(methodName)), parameterTypes(std::move(parameterTypes))
{
}

bool MethodSignature::operator==(const MethodSignature& other) const
{
    return methodName == other.methodName && parameterTypes == other.parameterTypes;
}

std::size_t MethodSignatureHash::operator()(const MethodSignature& signature) const
{
    std::size_t hash = std::hash<std::string>{}(signature.methodName);
    for (const auto& paramType : signature.parameterTypes)
    {
        hash ^= std::hash<std::string>{}(paramType) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

ClassEntry::ClassEntry(std::string className): name(std::move(className))
{
}

std::string ClassEntry::getName() const
{
    return name;
}

void ClassEntry::addField(const VariableEntry& field)
{
    fields.push_back(field);
}

std::vector<VariableEntry> ClassEntry::getFields() const
{
    return fields;
}

void ClassEntry::addMethod(const MethodSignature& signature, const MethodEntry& method)
{
    methods[signature] = method;
    // Get method name before _
    const std::string methodName = signature.methodName.substr(0, signature.methodName.find('_'));
    methodReturnTypes[methodName] = method.returnType;
}

void ClassEntry::addMethodValue(const MethodSignature& signature, llvm::Function* value)
{
    methodValues[signature] = value;
}

llvm::Function* ClassEntry::getMethodValue(const MethodSignature& signature)
{
    return methodValues[signature];
}

llvm::Function* ClassEntry::getMethodValue(const std::string& funcName, const std::vector<std::string>& argTypes)
{
    const MethodSignature signature(funcName, argTypes);
    return methodValues[signature];
}

bool ClassEntry::doesMethodExists(const std::string& name)
{
    std::vector<MethodEntry> classMethods = getMethods(name);
    return std::any_of(
        classMethods.begin(),
        classMethods.end(),
        [name](const MethodEntry& entry) { return entry.signature.methodName == name; }
    );
}

std::vector<MethodEntry> ClassEntry::getMethods(const std::string& name)
{
    std::vector<MethodEntry> methodsList;
    for (const auto& [signature, method] : methods)
    {
        if (signature.methodName == name)
        {
            methodsList.push_back(method);
        }
    }
    return methodsList;
}

std::vector<MethodEntry> ClassEntry::getMethodsByNameWithoutTypes(const std::string& prefix)
{
    std::vector<MethodEntry> methodsList;
    for (const auto& [signature, method] : methods)
    {
        const std::string methodName = signature.methodName.substr(0, signature.methodName.find('_'));
        if (methodName == prefix)
        {
            methodsList.push_back(method);
        }
    }
    return methodsList;
}

void ClassEntry::setParentClass(ClassEntry* parent)
{
    parentClass = parent;
}

ClassEntry* ClassEntry::getParentClass() const
{
    return parentClass;
}

std::string ClassEntry::getMethodReturnType(const std::string& name)
{
    return methodReturnTypes[name];
}

const VariableEntry* ClassEntry::lookupField(const std::string& name) const
{
    for (const auto& field : fields)
    {
        if (field.name == name)
        {
            return &field;
        }
    }
    if (parentClass)
    {
        return parentClass->lookupField(name);
    }
    return nullptr;
}

MethodEntry* ClassEntry::lookupMethod(const MethodSignature& signature)
{
    const auto it = methods.find(signature);
    if (it != methods.end())
    {
        return &it->second;
    }

    if (parentClass)
    {
        return parentClass->lookupMethod(signature);
    }
    return nullptr;
}

int ClassEntry::getFieldIndex(const std::string& varName) const
{
    for (int i = 0; i < fields.size(); i++)
    {
        if (fields[i].name == varName)
        {
            return i;
        }
    }
    if (parentClass)
    {
        return parentClass->getFieldIndex(varName);
    }
    return -1;
}

void SymbolTable::addVariableEntry(const std::string& name, const std::string& type)
{
    varEntries[name] = {name, type};
}


VariableEntry* SymbolTable::lookupVariable(const std::string& name)
{
    const auto it = varEntries.find(name);
    if (it != varEntries.end())
    {
        return &it->second;
    }
    return nullptr; // Not found
}

std::vector<std::string> SymbolTable::getUnusedVariables()
{
    // Find all unused variables in the current scope
    std::vector<std::string> unusedVariables;
    for (const auto& [name, entry] : varEntries)
    {
        if (!entry.is_used)
        {
            unusedVariables.push_back(name);
        }
    }
    return unusedVariables;
}

SymbolTable ScopedSymbolTable::getCurrentScope()
{
    return scopes.back();
}


void ScopedSymbolTable::enterScope()
{
    scopes.emplace_back();
}


void ScopedSymbolTable::leaveScope()
{
    if (!scopes.empty())
    {
        scopes.pop_back();
    }
}

void ScopedSymbolTable::addVariableEntry(const std::string& name, const std::string& type, const Span& span,
                                         const bool isClassField)
{
    if (!scopes.empty())
    {
        // Use a reference to modify the actual scope on the stack
        auto& current_scope = scopes.back();

        if (current_scope.varEntries.find(name) != current_scope.varEntries.end())
        {
            throw std::runtime_error(
                "Variable '" + name + "' is already declared in this scope " +
                "at line: " + std::to_string(span.get_line_num()) +
                " column: " + std::to_string(span.get_pos_begin())
            );
        }

        // Add the new entry to the current scope
        identifierTypes[name] = ID_VARIABLE;
        current_scope.addVariableEntry(name, type);
        const auto classEntry = lookupClass(currClassName, span, true);
        if (isClassField)
        {
            const VariableEntry field = {name, type};
            classEntry->addField(field);
        }
    }
}

void ScopedSymbolTable::addFunctionEntry(const std::string& name, const std::string& className,
                                         const std::string& returnType, const Span& span,
                                         const std::vector<std::string>& paramTypes)
{
    std::string funcName = name;
    for (const auto& paramType : paramTypes)
    {
        funcName += "_" + paramType;
    }

    const MethodSignature signature(funcName, paramTypes);
    ClassEntry* classEntry = lookupClass(className, span, true);
    identifierTypes[name] = ID_FUNCTION;

    if (classEntry->doesMethodExists(signature.methodName))
    {
        throw std::runtime_error(
            "Method '" + name + "' is already declared in this scope" +
            " at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    classEntry->addMethod(signature, {signature, returnType});
}


void ScopedSymbolTable::addFunctionValue(const std::string& name, const std::string& className,
                                         const std::vector<std::string>& argTypes, llvm::Function* func)
{
    const MethodSignature signature(name, argTypes);
    ClassEntry* classEntry = lookupClass(className, Span(0, 0, 0), true);
    classEntry->addMethodValue(signature, func);
}

void ScopedSymbolTable::addClassEntry(const std::string& name, const Span& span)
{
    if (classEntries.find(name) != classEntries.end())
    {
        throw std::runtime_error(
            "Class '" + name + "' is already declared in this scope " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    identifierTypes[name] = ID_CLASS;
    classEntries[name] = ClassEntry(name);
}

std::unique_ptr<VariableEntry> ScopedSymbolTable::lookupVariable(const std::string& name, const Span& span,
                                                                 const bool throw_error)
{

    // Iterate over all scopes from the innermost to the outermost
    for (auto it = scopes.size(); it-- > 0;)
    {
        auto& varEntries = scopes[it].varEntries;
        auto found = varEntries.find(name);
        if (found != varEntries.end())
        {
            return std::make_unique<VariableEntry>(name, found->second.type, found->second.is_used);
        }
    }
    if (throw_error)
    {
        throw std::runtime_error(
            "Variable '" + name + "' used before declaration " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    return nullptr;
}

void ScopedSymbolTable::makeVariableUsed(const std::string& name)
{
    for (auto it = scopes.size(); it-- > 0;)
    {
        auto& varEntries = scopes[it].varEntries;
        auto found = varEntries.find(name);
        if (found != varEntries.end())
        {
            found->second.is_used = true;
            return;
        }
    }
}

std::vector<std::string> getAllAncestors(ScopedSymbolTable* symbolTable, const std::string& className)
{
    std::vector<std::string> ancestors;
    const ClassEntry* current = symbolTable->lookupClass(className, Span(0, 0, 0), false);
    while (current && current->parentClass)
    {
        ancestors.push_back(current->parentClass->name);
        current = current->parentClass;
    }
    return ancestors;
}

MethodEntry* ScopedSymbolTable::lookupFunction(const std::string& className, const std::string& methodName,
                                               const std::vector<std::string>& params,
                                               const Span& span, const bool throw_error)
{
    std::vector<std::vector<std::string>> paramOptions;
    paramOptions.emplace_back(); // Start with an empty combination

    for (const auto& param : params)
    {
        std::vector<std::string> currentTypes;
        currentTypes.push_back(param); // The type itself

        // Get all ancestors of this parameter type
        std::vector<std::string> ancestors = getAllAncestors(this, param);
        currentTypes.insert(currentTypes.end(), ancestors.begin(), ancestors.end());

        // Generate new combinations by appending each possible type
        std::vector<std::vector<std::string>> newParamOptions;
        for (const auto& existingCombination : paramOptions)
        {
            for (const auto& typeOption : currentTypes)
            {
                std::vector<std::string> newCombination = existingCombination;
                newCombination.push_back(typeOption);
                newParamOptions.push_back(newCombination);
            }
        }
        paramOptions = std::move(newParamOptions);
    }

    ClassEntry* classEntry = lookupClass(className, span, true);
    for (const auto& option : paramOptions)
    {
        // Construct the function name with the current parameter types
        std::string funcName = methodName;
        for (const auto& paramType : option)
        {
            funcName += "_" + paramType;
        }

        MethodSignature signature{funcName, option};
        MethodEntry* methodEntry = classEntry->lookupMethod(signature);
        if (methodEntry)
        {
            return methodEntry;
        }
    }

    if (throw_error)
    {
        std::string originalFuncName = methodName;
        for (const auto& param : params)
        {
            originalFuncName += "_" + param;
        }
        throw std::runtime_error(
            "Method '" + className + "::" + originalFuncName + "' used before declaration " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    return nullptr;
}

bool ScopedSymbolTable::doesMethodExists(std::string& name, const std::string& className)
{
    ClassEntry classEntry = classEntries[className];
    return classEntry.doesMethodExists(className);
}


ClassEntry* ScopedSymbolTable::lookupClass(const std::string& name, const Span& span,
                                           const bool throw_error)
{
    if (classEntries.find(name) != classEntries.end())
    {
        return &classEntries[name];
    }
    if (throw_error)
    {
        throw std::runtime_error(
            "Class '" + name + "' used before declaration " +
            "at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
        );
    }
    return nullptr;
}

IdentifierType ScopedSymbolTable::getIdentifierType(const std::string& identifier)
{
    if (identifierTypes.find(identifier) != identifierTypes.end())
    {
        auto identifierType = identifierTypes[identifier];
        if(identifierType==ID_VARIABLE){

        }
        return identifierType;

    }
    return ID_UNDEFINED;
}

llvm::Value* ScopedSymbolTable::getLocalVariable(const std::string& varName)
{
    return varEntries[varName];
}

void ScopedSymbolTable::addLocalVariable(const std::string& varName, llvm::Value* pInst, const std::string& type)
{
    varEntries[varName] = pInst;
}


std::string ScopedSymbolTable::getFunctionType(const std::string& name, const std::string& className)
{
    const auto classEntry = lookupClass(className, Span(0, 0, 0), false);
    return classEntry->getMethodReturnType(name);
}

std::string ScopedSymbolTable::getIdentifierStringType(const std::string& identifier, const Span& span)
{
    switch (getIdentifierType(identifier))
    {
    case ID_VARIABLE:
        return lookupVariable(identifier, span)->type;
    case ID_CLASS:
        return identifier;
    }
    throw std::runtime_error(
            "Identifier " + identifier + " is not declared" +
            " at line: " + std::to_string(span.get_line_num()) +
            " column: " + std::to_string(span.get_pos_begin())
    );
}

llvm::Function* ScopedSymbolTable::getMethodValue(const std::string& className, const std::string& funcName,
                                                  const std::vector<std::string>& argTypes)
{
    const auto classEntry = lookupClass(className, Span(0, 0, 0), false);
    return classEntry->getMethodValue(funcName, argTypes);
}

int ScopedSymbolTable::getFieldIndex(const std::string& className, const std::string& varName)
{
    const auto classEntry = lookupClass(className, Span(0, 0, 0), false);
    return classEntry->getFieldIndex(varName);
}

llvm::Value* ScopedSymbolTable::getThisPointer() const
{
    return thisPtr;
}

void ScopedSymbolTable::setThisPointer(llvm::Value* thisPtr)
{
    this->thisPtr = thisPtr;
}

void ScopedSymbolTable::changeVariableType(std::string varName, const std::string newType) {
    for (auto it = scopes.size(); it-- > 0;)
    {
        auto& varEntries = scopes[it].varEntries;
        auto found = varEntries.find(varName);
        if (found != varEntries.end())
        {
            found->second.type = newType;
            return;
        }
    }

}
