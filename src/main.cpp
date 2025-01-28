/**
 * @file main.cpp
 *
 * @brief Main file for light array control 
 * @ingroup Oscillot
 *
 * @author Tim Barlow
 *         timothy.barlow@singluariti.co
 *
 */


#include "ethercat.h"
#include "EL2574.hpp"
#include "ethercat_controller.hpp"
#include <iostream>


char IOmap[4096];

int main(int argc, char **argv) {
    int a = 0;
    ec_find_adapters();
    std::cout << "Hello World" << std::endl;

    /// TODO (Tim Barlow): Get rid of all magic numbers and strings

    // Initialise SOEM, bind socket to interface
   if (!ec_init("enx7cc2c649b50c")) {
        // On Failed binding to interface 
        std::cout << "EC INIT FAILED...EXITING" << std::endl;
        exit(EXIT_FAILURE);
    } 
    // Successfuly binded to interface 
    std::cout << "EC INIT SUCCEEDED" << std::endl;

    // Configure slaves
    // Use config table = false
    if (ec_config_init(FALSE) > 0) {
        std::cout << "Slaves found: " << ec_slavecount << std::endl;
    }

    ec_config_map(&IOmap);

    // Wait for all slaves to reach SAFE_OP State
    ec_statecheck(0,EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);

    // Loop through available slaves and configure if slave is EL2574
    // For each active slave create 
    for (int i = 1; i <= ec_slavecount; i++) {
        // Check if slave is EL2574
        if (!is_EL2574(ec_slave[i].name)) {
            // Report slave is not 2574 and skip loop
            std::cout << "Slave is not EL2574" << std::endl;
            continue;
        }
        // If is EL2574, configure and create buffer for IO 
        if (!configure_EL2574(i)) {
            std::cout << "Could not configure slave: " 
                << i 
                << "Exiting..." << std::endl;
                exit(EXIT_FAILURE);
        }
        // Successful configuration of slave
        std::cout << "configured slave: " << i << std::endl;
    }

    // For all Slave modules, do a 8x32 checkerboard pattern
    for (int i = 1; i <= ec_slavecount; i++) {
        if (!EL2574_checker_board(i,GRID_DIMENSION)) {
            std::cout << "Could not print checker board for module:"
                << i 
                << "Exiting..."
                << std::endl;
                exit(EXIT_FAILURE);
        } else {
            std::cout << "Printing CheckerBoard..." << std::endl;
        }
    }

    
}