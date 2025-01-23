
#include "ethercat.h"

#include <iostream>

int main(int argc, char **argv) {
    int a = 0;
    ec_find_adapters();
    std::cout << "Hello World" << std::endl;
    return 0;
    // Load Yaml Config file
    // Initialize controller (EthercatController.cpp)
    // Loop over slaves 
        // Initialize slave functions (EL2754.CPP)
    // Load Images to display on array 
    // Run images on a loop to play looping video
}