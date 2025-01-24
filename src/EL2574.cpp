/**
 * @file EL2574.cpp
 *
 * @brief C++ file for Beckhoff Light array Module "EL2574"
 *        contains methods for configuring, bit packing values and status provided by module.
 *        Technical Documentation related to TC module can be found here:
 *        https://download.beckhoff.com/download/document/io/ethercat-terminals/el2574en.pdf
 *
 * @ingroup Oscillot
 *
 * @author Tim Barlow
 *         timothy.barlow@singluariti.co
 *
 */

#include "EL2574.hpp"

int is_EL2574(char* name) {
  return strncmp(name,
          EL2574_STR, sizeof(EL2574_STR)/(sizeof(char)-1) == 0);
}

int configure_EL2574(EL2574_module_confs confs){
    EL2574_ch_confs ch_confs;
    confs.ch0_confs = ch_confs;
    confs.ch1_confs = ch_confs;
    confs.ch2_confs = ch_confs;
    confs.ch3_confs = ch_confs;
}