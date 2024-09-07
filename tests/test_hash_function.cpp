#include "../src/utils/utils.h"
#include <iostream>

int main(const int argc, char *argv[]){
    std::string str = "Hello, world!";
    auto hash = calculate_hash(str);
    std::cout << hash << std::endl;
    return 0;
}