
#include <iostream>

int main(int argc, char* argv[]){
    int i = 0;
    for (char** arg = argv; *arg && i < argc; ++arg, ++i) {
        std::cout << i << " " << *arg << std::endl;
    }

    return 0;
}
