#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#define OUTPUT_DEFAULT_EXTENSION ".out"

class Settings
{
    std::string me;
    std::string infile;
    std::string outfile;
    bool debug = false;
    bool help = false;
    std::string optimization_level = "-O0";

public:
    void parse(int argc, char* argv[]);

    void print_help() const;

    void process();

    [[nodiscard]] std::string get_infile() const;

    [[nodiscard]] bool get_debug() const;

    [[nodiscard]] std::string get_output_filename() const;

    [[nodiscard]] std::string get_optimization_level() const;

    friend std::ostream& operator<<(std::ostream& os, Settings const& settings);
};

#endif //SETTINGS_H
