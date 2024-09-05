#include <iostream>
#include "settings/settings.h"


int main(const int argc, char *argv[]) {
    auto settings = Settings();
    settings.parse(argc, argv);
    if (!settings.process()) {
        return 1;
    }

    std::cout << settings << std::endl;

    return 0;
}
