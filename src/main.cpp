#include <iostream>

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [file]" << std::endl;
        return 1;
    }

    std::cout << "Compiling file " << argv[1] << std::endl;

    return 0;
}
