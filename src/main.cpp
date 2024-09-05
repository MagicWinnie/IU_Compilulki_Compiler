#include <iostream>

using namespace std;

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " [file]" << endl;
        return 1;
    }

    cout << "Compiling file " << argv[1] << endl;

    return 0;
}
