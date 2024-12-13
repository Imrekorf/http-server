#include <iostream>

#include "temp.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    std::cout << "starting testing..." << std::endl;

    test();

    std::cout << "testing complete!..." << std::endl;

    return 0;
}
