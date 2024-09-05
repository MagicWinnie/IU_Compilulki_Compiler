#include <iostream>
#include "settings/settings.h"


int main(const int argc, char *argv[]) {
    auto settings = Settings();
    settings.parse(argc, argv);
    settings.process();

    std::cout << settings << std::endl;

    // TODO: add lexer call here

    return 0;
}
