//
// Created by nai1ka on 10/23/24.
//


#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

struct SymbolTableEntry {
    std::string name;   // Identifier name (e.g., variable name)
    std::string type;   // Type (e.g., Integer, String)
    bool is_constant;   // Whether it's a constant
    // Additional metadata (scope depth, memory location, etc.)
};

class SymbolTable {
public:
    std::unordered_map<std::string, SymbolTableEntry> entries;

    // Add an entry
    void addEntry(const std::string& name, const std::string& type, bool is_constant = false) {
        entries[name] = {name, type, is_constant};
    }

    // Lookup an entry
    SymbolTableEntry* lookup(const std::string& name) {
        auto it = entries.find(name);
        if (it != entries.end()) {
            return &it->second;
        }
        return nullptr; // Not found in this scope
    }
};

class ScopedSymbolTable {
private:
    std::vector<SymbolTable> scopes;

public:
    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope() {
        scopes.push_back(SymbolTable());
    }

    // Leave a scope (pop the current symbol table)
    void leaveScope() {
        if (!scopes.empty()) {
            scopes.pop_back();
        }
    }

    // Add an entry in the current scope
    void addEntry(const std::string& name, const std::string& type, bool is_constant = false) {

        if (!scopes.empty()) {
            // Use a reference to modify the actual scope on the stack
            auto& current_scope = scopes.back();

            if (current_scope.entries.find(name) != current_scope.entries.end()) {
                throw std::runtime_error("Variable '" + name + "' is already declared in this scope.");
            }

            // Add the new entry to the current scope
            current_scope.addEntry(name, type, is_constant);
        }
    }

    // Lookup an entry across all scopes (from innermost to outermost)
    std::string lookup(const std::string& name) const
    {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            const auto& scope = *it;
            auto found = scope.entries.find(name);
            if (found != scope.entries.end()) {
                return found->second.type;
            }
        }
        throw std::runtime_error("Variable '" + name + "' used before declaration.");
    }
};
