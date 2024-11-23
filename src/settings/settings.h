#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#define OUTPUT_DEFAULT_EXTENSION ".o"

class Settings {
    std::string me;
    std::string infile;
    std::string outfile;
    bool debug = false;
    bool help = false;
    std::string optimizationL_level = "-O0";
    std::string output_filename = "output";


public:
    void parse(int argc, char *argv[]);

    void print_help() const;

    void process();

    std::string get_infile() const;

    bool get_debug() const;

    std::string get_output_filename() const;

    std::string get_optimization_level() const;

    friend std::ostream &operator<<(std::ostream &os, Settings const &settings);
};

#endif //SETTINGS_H
