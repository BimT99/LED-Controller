/**
 * @file main.cpp
 *
 * @brief Main file for light array control 
 * @ingroup 
 *
 * @author Tim Barlow
 *         timothy.barlow@singluariti.co
 *
 */


#include "ethercat.h"
#include "EL2574.hpp"
#include "ethercat_controller.hpp"
#include <iostream>
#include <unistd.h>


char IOmap[4096];

void print_slave_states(std::string msg) {
  /* read individual slave state and store in ec_slave[] */
  ec_readstate();
  printf("States at: %s\n", msg.c_str());
  for (int i = 0; i <= ec_slavecount ; i++) {\
    // printf("Slave %d State=0x%2.2x %s, StatusCode=0x%4.4x : %s\n",
    //   i, ec_slave[i].state, state_to_string(ec_slave[i].state).c_str(),
    //   ec_slave[i].ALstatuscode,
    //   ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
    printf("Slave %d State=%s, StatusCode=%s\n",
      i, state_to_string(ec_slave[i].state).c_str(),
      ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
  }
}

std::string state_to_string(uint16 state) {
  std::string err = (state & 0xF0)?"ERROR+":"";
  switch (state & 0x0F) {
  case EC_STATE_NONE:
    return err+"NONE";
    break;
  case EC_STATE_INIT:
    return err+"INIT";
    break;
  case EC_STATE_PRE_OP:
    return err+"PRE_OP";
    break;
  case EC_STATE_BOOT:
    return err+"BOOT";
    break;
  case EC_STATE_SAFE_OP:
    return err+"SAFE_OP";
    break;
  case EC_STATE_OPERATIONAL:
    return err+"OPERATIONAL";
    break;
  default:
    return std::string("state?: ") + std::to_string(state);
    break;
  }
}

int main(int argc, char **argv) {

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

    ec_configdc();

    // Wait for all slaves to reach SAFE_OP State
    ec_statecheck(0,EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);

    int el2574_slave = 0;

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
        std::cout << "configured  EL2574 slave: " << i << std::endl;
        el2574_slave = i;
    }

    ec_readstate();
    ec_send_processdata();
    ec_receive_processdata(EC_TIMEOUTRET);
    std::cout << "State t0 = " << ec_slave[el2574_slave].state << std::endl;
    ec_slave[el2574_slave].state = EC_STATE_OPERATIONAL;
    /* request OP state for all slaves */
    ec_writestate(el2574_slave);
    sleep(2);
    /* wait for all slaves to reach OP state */
    ec_statecheck(el2574_slave, EC_STATE_OPERATIONAL,  EC_TIMEOUTSTATE);
   

    if (ec_slave[el2574_slave].state != EC_STATE_OPERATIONAL) {
        std::cout << "Slave states not all operational \n"
                << "Slave State: "
                << ec_slave[el2574_slave].state
                << "\n Exiting..."
                << std::endl;
        exit(EXIT_FAILURE);
    }
    // For all Slave modules, do a 8x32 checkerboard pattern
    for (int i = 1; i <= ec_slavecount; i++) {
        // TODO (TIM): Magic number
        if (i == el2574_slave) {
            for (int i = 0; i < 10; i++) {
                sleep(2);
                if (!EL2574_checker_board(i,0,GRID_DIMENSION)) {
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
    }  
}