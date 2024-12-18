#include <iostream>
#include <filesystem>
#include "settings.h"

void Settings::parse(const int argc, char* argv[])
{
    this->me = std::string(argv[0]);
    for (int i = 1; i < argc; i++)
    {
        // first element is our filename
        std::string opt{argv[i]};
        if (opt.rfind("--", 0) == 0 || opt.rfind('-', 0) == 0)
        {
            if (opt == "--debug" || opt == "-g")
            {
                this->debug = true;
            }
            else if (opt == "--help" || opt == "-h")
            {
                this->help = true;
            }
            // Check if starts with "-O"
            else if (opt.rfind("-O", 0) == 0)
            {
                this->optimization_level = opt;
            }
            else if (opt == "--outfile" || opt == "-o")
            {
                this->outfile = argv[++i];
            }
            else
            {
                std::cerr << "Unknown option: " << opt << "\n";
                this->print_help();

                std::exit(EXIT_FAILURE);
            }
        }
        else
        {
            if (this->infile.empty())
            {
                this->infile = opt;
            }
            else
            {
                std::cerr << "Unknown argument: " << opt << "\n";
                this->print_help();

                std::exit(EXIT_FAILURE);
            }
        }
    }
}

void Settings::print_help() const
{
    std::cerr << "Usage: " << this->me << " [OPTIONS] INFILE\n";
    std::cerr << "Options:\n";
    std::cerr << "\t-g, --debug: Enable debugging information\n";
    std::cerr << "\t-h, --help: Show this help message\n";
    std::cerr << "\t-O<level>: Set optimization level (default: -O0)\n";
    std::cerr << "\t-o, --outfile: Set output filename\n";
}


void Settings::process()
{
    if (this->help)
    {
        this->print_help();

        std::exit(EXIT_FAILURE);
    }
    if (this->infile.empty())
    {
        std::cerr << "Missing mandatory parameter INFILE" << std::endl;
        this->print_help();

        std::exit(EXIT_FAILURE);
    }
    if (this->outfile.empty())
    {
        std::filesystem::path path(this->infile);
        path.replace_extension(OUTPUT_DEFAULT_EXTENSION);
        this->outfile = path.string();
    }
}


std::string Settings::get_infile() const
{
    return this->infile;
}

bool Settings::get_debug() const
{
    return this->debug;
}

std::string Settings::get_output_filename() const
{
    return this->outfile;
}

std::string Settings::get_optimization_level() const
{
    return this->optimization_level;
}

std::ostream& operator<<(std::ostream& os, Settings const& settings)
{
    return os << "{infile='" << settings.infile
        << "'; outfile='" << settings.outfile
        << "'; debug=" << settings.debug
        << "; help=" << settings.help
        << "; optimization_level=" << settings.optimization_level << "}";
}
