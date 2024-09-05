#include <iostream>
#include "settings.h"

void Settings::parse(int argc, char *argv[]) {
    this->me = std::string(argv[0]);
    for (int i = 1; i < argc; i++) {
        // first element is our filename
        std::string opt{argv[i]};
        if (opt == "--debug" || opt == "-g") {
            this->debug = true;
        } else if (opt == "--help" || opt == "-h") {
            this->help = true;
        } else if (this->infile.empty()) {
            this->infile = opt;
        } else if (this->outfile.empty()) {
            this->outfile = opt;
        }
    }
}

void Settings::print_help() const {
    std::cerr << "Usage: " << this->me << " [OPTIONS] [INFILE] [OUTFILE]\n";
    std::cerr << "Options:\n";
    std::cerr << "\t-g, --debug: Enable debugging information\n";
    std::cerr << "\t-h, --help: Show this help message\n";
}


bool Settings::process() const {
    if (this->help) {
        this->print_help();
        return false;
    }
    if (this->infile.empty()) {
        std::cerr << "Missing mandatory parameter INFILE" << std::endl;
        this->print_help();
        return false;
    }
    if (this->outfile.empty()) {
        std::cerr << "Missing mandatory parameter OUTFILE" << std::endl;
        this->print_help();
        return false;
    }
    return true;
}


std::ostream &operator<<(std::ostream &os, Settings const &settings) {
    return os << "{infile='" << settings.infile
           << "'; outfile='" << settings.outfile
           << "'; debug=" << settings.debug
           << "; help=" << settings.help << "}";
}
