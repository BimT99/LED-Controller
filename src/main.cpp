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



// Helper Methods
std::string state_to_string(uint16 state);
void print_slave_states(std::string msg);

// Start Main, TODO (Tim Barlow): Get rid of all magic numbers and strings
int main(int argc, char **argv) {

  // Initialise SOEM, bind socket to interface
  if (!ec_init("enx7cc2c649b50c")) {
        // On Failed binding to interface 
        std::cout << "EC INIT FAILED...EXITING" << std::endl;
        exit(EXIT_FAILURE);
  } 

  // Configure slaves
  // Use config table = false
  if (ec_config_init(FALSE) > 0) {
      std::cout << "Slaves found: " << ec_slavecount << std::endl;
  }

  // Loop through available slaves and configure if slave is EL2574
  // For each active slave create 
  ec_slave[0].state = EC_STATE_PRE_OP;
  ec_writestate(0);
  ec_statecheck(ALL_SLAVES,EC_STATE_PRE_OP,EC_TIMEOUTRET);

  print_slave_states("Pre-Config");
  
  int el2574_slave = 0;

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

      // set PO->SO hook
      ec_slave[i].PO2SOconfig = configure_EL2574;

      // Successful configuration of slave
      std::cout << "configured  EL2574 slave: " << i << std::endl;
      el2574_slave = i;
  }

  char IOmap[4096];

  std::cout <<  "ec_config_map(&IOmap);" << std::endl;
  ec_config_map(&IOmap);
  ec_configdc();

  // DEBUGS 
  //ec_dcsync0(el2574_slave, TRUE, 4000000, 0);
  std::cout << "RxPDO address start: " 
            << ec_slave[el2574_slave].SM[2].StartAddr
            << " TxPDO address start: " 
            << ec_slave[el2574_slave].SM[3].StartAddr
            << std::endl;

  print_slave_states("Post-Config");

  // ec_slave[ALL_SLAVES].state = EC_STATE_INIT;
  ec_slave[ALL_SLAVES].state = EC_STATE_SAFE_OP;
  /* request SAFE-OP state for all slaves */
  ec_writestate(ALL_SLAVES);;

  // Wait for EL2574 slaves to reach SAFE_OP State
  ec_statecheck(ALL_SLAVES, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);


  print_slave_states("ec_statecheck(el2574_slave,EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);");

  // Send and recieve one data cycle to init SM in slaves
  //ec_send_processdata();
  //ec_receive_processdata(EC_TIMEOUTRET);

  //ec_readstate();
  //print_slave_states("Before setting state");

  ec_slave[ALL_SLAVES].state = EC_STATE_OPERATIONAL;
  /* request OP state for all slaves */
  ec_writestate(ALL_SLAVES);

  ec_statecheck(ALL_SLAVES, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE);

  ec_readstate();
  print_slave_states("After Writing state");
  /* wait for all slaves to reach OP state */

  print_slave_states("After Statecheck");

  if (ec_slave[el2574_slave].state != EC_STATE_OPERATIONAL) {
      std::cout << "Slave states not all operational \n"
              << "Slave State: "
              << ec_slave[el2574_slave].state
              << "\n Exiting..."
              << std::endl;
      sleep(4);
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
// End Main

// Helper Method definitions
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

void print_slave_states(std::string location_msg) {
 for (int i = 0; i <= ec_slavecount; i++) {
    std::cout << "Slave: " << i
              << " - Location: " << location_msg
              << " - State: " << ec_slave[i].state
              << " - Status: "
              << ec_ALstatuscode2string(ec_slave[i].ALstatuscode)
              << std::endl;
  }
}

