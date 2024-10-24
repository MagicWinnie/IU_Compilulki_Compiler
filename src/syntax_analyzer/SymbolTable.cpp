//
// Created by nai1ka on 10/23/24.
//


#include <stack>
#include <string>
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
    std::stack<SymbolTable> scopes;

public:
    // Enter a new scope (push a new symbol table onto the stack)
    void enterScope() {
        scopes.emplace(SymbolTable());
    }

    // Leave a scope (pop the current symbol table)
    void leaveScope() {
        if (!scopes.empty()) {
            scopes.pop();
        }
    }

    // Add an entry in the current scope
    void addEntry(const std::string& name, const std::string& type, bool is_constant = false) {
        if (!scopes.empty()) {
            scopes.top().addEntry(name, type, is_constant);
        }
    }

    // Lookup an entry across all scopes (from innermost to outermost)
    SymbolTableEntry* lookup(const std::string& name) {
        // for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        //     auto* entry = it->lookup(name);
        //     if (entry) {
        //         return entry;
        //     }
        // }
        return nullptr; // Not found in any scope
    }
};
