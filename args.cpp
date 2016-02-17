
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]){
    int i = 0;
    for (char** arg = argv; *arg && i < argc; ++arg, ++i) {
        std::cout << i << " " << std::quoted(*arg) << std::endl;
    }

    return 0;
}
