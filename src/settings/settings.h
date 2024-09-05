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

public:
    void parse(int argc, char *argv[]);

    void print_help() const;

    void process();

    std::string get_infile() const;

    bool get_debug() const;

    friend std::ostream &operator<<(std::ostream &os, Settings const &settings);
};

#endif //SETTINGS_H
