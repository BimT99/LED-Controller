/**
 * @file EL2574.cpp
 *
 * @brief C++ file for Beckhoff Light array Module "EL2574"
 *        contains methods for configuring, bit packing values and status provided by module.
 *        Technical Documentation related to TC module can be found here:
 *        https://download.beckhoff.com/download/document/io/ethercat-terminals/el2574en.pdf
 *
 * @ingroup
 *
 * @author Tim Barlow
 *         timothy.barlow@singluariti.co
 *
 */

#include "EL2574.hpp"


int is_EL2574(char* name) {
  return strncmp(name,
          EL2574_STR, (sizeof(EL2574_STR)/sizeof(char))-1 ) == 0;
}

int configure_EL2574(uint16_t slave) {


  EL2574_module_confs confs;
  EL2574_ch_confs ch_confs;
  int ch_indx;

  /// @brief TxPDO is at most 1+4 uint16s
  uint16_t txpdo_assign[TxPDO_SZ_ENTRY + EL2574_TxPDO_MAX_SZ];
  uint16_t* txpdo_assign_ptr = &(txpdo_assign[0]);

  // store size in entry 0
  // [ (page 146) initially set subindex 0 to 0
  // and at the end to the number of entered values.]
  {
    int i=0;
    *(txpdo_assign_ptr++) = EL2574_TxPDO_MAX_SZ;      i=0;
    *(txpdo_assign_ptr++) = EL2574_TxPDO_SUB_STS_CH0; i++;
    *(txpdo_assign_ptr++) = EL2574_TxPDO_SUB_STS_CH1; i++;
    *(txpdo_assign_ptr++) = EL2574_TxPDO_SUB_STS_CH2; i++;
    *(txpdo_assign_ptr++) = EL2574_TxPDO_SUB_STS_CH3; i++;
    txpdo_assign[0]= i;
  }
  // Initialise TxPDO by passing pointer to buffer of max RxPDO size
  ec_SDOwrite(slave, EL2574_TxPDO_IDX,
   ECT_SDO_SUB_ZERO, TRUE,
   sizeof(txpdo_assign),&txpdo_assign,EC_TIMEOUTRXM);

  /// @brief RxPDO is at most 1+4 uint16s
  uint16_t rxpdo_assign[RxPDO_SZ_ENTRY + EL2574_RxPDO_MAX_SZ];
  uint16_t* rxpdo_assign_ptr = &(rxpdo_assign[0]);
  {
    int i=0;
    *(rxpdo_assign_ptr++) = EL2574_RxPDO_MAX_SZ;      i=0;
    *(rxpdo_assign_ptr++) = EL2574_RxPDO_SUB_EXT_CH0; i++;
    *(rxpdo_assign_ptr++) = EL2574_RxPDO_SUB_EXT_CH1; i++;
    *(rxpdo_assign_ptr++) = EL2574_RxPDO_SUB_EXT_CH2; i++;
    *(rxpdo_assign_ptr++) = EL2574_RxPDO_SUB_EXT_CH3; i++; 
    rxpdo_assign[0]= i;
  }
  for (int i = 0; i < sizeof(rxpdo_assign)/sizeof(rxpdo_assign[0]); i++) {
    std::cout << "RxPDO Entry: " << i << " = " << rxpdo_assign[i] << std::endl;  
    std::cout << "TxPDO Entry: " << i << " = " << txpdo_assign[i] << std::endl;  
  }


  // Initialise RxPDO by passing pointer to buffer of max RxPDO size
  ec_SDOwrite(slave, EL2574_RxPDO_IDX,
    ECT_SDO_SUB_ZERO, TRUE,
    sizeof(rxpdo_assign), &rxpdo_assign,EC_TIMEOUTRXM);


  for (int i = 0; i < 1; i++) {
    switch (i){
      case 0: 
        ch_indx = EL2574_STGS_CH0;
        break;
      case 1:
        ch_indx = EL2574_STGS_CH1;
        break;
      case 2:
        ch_indx = EL2574_STGS_CH2;
        break;
      case 3:
        ch_indx = EL2574_STGS_CH3;
        break;
      default:
        // TODO: not this
        continue;
    }

    //std::cout << (int) confs.ch0_confs.color_format << " color fmt" << std::endl;

    //return TRUE;
    // debug
    //ch_indx = EL2574_STGS_CH0 + 10*i;

    ec_SDOwrite(slave, ch_indx,
      EL2574_ENABLE_CTM_STGS , FALSE,
      sizeof(confs.ch0_confs.enbl_cust_stgs),
      &(ch_confs.enbl_cust_stgs),
      EC_TIMEOUTRXM);

    if(confs.ch0_confs.enbl_cust_stgs == 1) {

      std::cout << "setting custom settings..." << std::endl;
      ec_SDOwrite(slave, ch_indx,
        EL2574_NUM_PIXELS , FALSE,
        sizeof(confs.ch0_confs.num_pixels),
        &(ch_confs.num_pixels),
        EC_TIMEOUTRXM);

      ec_SDOwrite(slave, ch_indx,
        EL2575_CHIP_TYPE , FALSE,
        sizeof(confs.ch0_confs.chip_type),
        &(ch_confs.chip_type),
        EC_TIMEOUTRXM);

      ec_SDOwrite(slave, ch_indx,
        EL2574_CLR_FMT , FALSE,
        sizeof(confs.ch0_confs.color_format),
        &(ch_confs.color_format),
        EC_TIMEOUTRXM);

      ec_SDOwrite(slave, ch_indx,
        EL2574_CTM_DATA_RATE , FALSE,
        sizeof(confs.ch0_confs.data_rate),
        &(ch_confs.data_rate),
        EC_TIMEOUTRXM);
    }
  }

  return TRUE;
} // End El2574 Config

int EL2574_write_index(int slave, int channel, int index, uint32_t* element_ptr) {
  // Variable to hold channel index
  int ch_indx;
  uint8_t _true = (1);
  uint8_t _false = (0);

  // Switch statement to get correct channel index
  switch(channel) {
    case 0: 
      ch_indx = EL2574_EXTD_CH0;
      break;
    case 1:
      ch_indx = EL2574_EXTD_CH1;
      break;
    case 2:
      ch_indx = EL2574_EXTD_CH2;
      break;
    case 3:
      ch_indx = EL2574_EXTD_CH3;
      break;
    default:
      // TODO (Tim Barlow): Error Handling
      return FALSE;
  }

  // execute command (EXECUTE+WRITE)
  ec_SDOwrite(slave, ch_indx,
    EL2574_EXTD_CTRL_EXECUTE_SIDX, FALSE,
    sizeof(_true), &_true,
    EC_TIMEOUTRXM);

  ec_SDOwrite(slave, ch_indx,
    EL2574_EXTD_CTRL_WRITE_SIDXX, FALSE,
    sizeof(_true), &_true,
    EC_TIMEOUTRXM);

  // Write index to indicate segment number 
  ec_SDOwrite(slave, ch_indx,
    EL2574_EXTD_CTRL_IDX_SIDX, FALSE,
    sizeof(index), &index,
    EC_TIMEOUTRXM);

  // Create iterator variable
  uint32_t* seg_iter = element_ptr;
  for (int element_sub_idx = EL2574_EXTD_CTRL_SEG_ELE_0_SIDX; 
    element_sub_idx < EL2574_EXTD_CTRL_SEG_ELE_0_SIDX + SEGM_SIZE; 
    element_sub_idx++) {
   
    // Write to element[i]
    ec_SDOwrite(slave, ch_indx,
      element_sub_idx, FALSE,
      sizeof(uint32_t), seg_iter,
      EC_TIMEOUTRXM);
    
    // Increment iterator
    seg_iter ++;
  }

  return TRUE;
}

int EL2574_update_frame_buffer(int slave, int channel) {
  // Variable for tracking channel index
  int ch_indx;
  uint8_t _true = (1);
  uint8_t _false = (0);

  // Switch statement to get correct channel index
  switch(channel) {
    case 0: 
      ch_indx = EL2574_EXTD_CH0;
      break;
    case 1:
      ch_indx = EL2574_EXTD_CH1;
      break;
    case 2:
      ch_indx = EL2574_EXTD_CH2;
      break;
    case 3:
      ch_indx = EL2574_EXTD_CH3;
      break;
    default:
      // TODO (Tim Barlow): Error Handling
      return FALSE;
  }
  // Set execute and update to true
  ec_SDOwrite(slave, ch_indx,
    EL2574_EXTD_CTRL_EXECUTE_SIDX, FALSE,
    sizeof(_true), &_true,
    EC_TIMEOUTRXM);

  ec_SDOwrite(slave, ch_indx,
    EL2574_EXTD_CTRL_UPDATE_SIDXX, FALSE,
    sizeof(_true), &_true,
    EC_TIMEOUTRXM);

  return TRUE;
}

int EL2574_send_frame_buffer(int slave, int channel) {
   // Variable for tracking channel index
  int ch_indx;
  uint8_t _true = (1);
  uint8_t _false = (0);

  // Switch statement to get correct channel index
  switch(channel) {
    case 0: 
      ch_indx = EL2574_EXTD_CH0;
      break;
    case 1:
      ch_indx = EL2574_EXTD_CH1;
      break;
    case 2:
      ch_indx = EL2574_EXTD_CH2;
      break;
    case 3:
      ch_indx = EL2574_EXTD_CH3;
      break;
    default:
      // TODO (Tim Barlow): Error Handling
      return FALSE;
  }
  // Set execute and update to true
  ec_SDOwrite(slave, ch_indx,
    EL2574_EXTD_CTRL_EXECUTE_SIDX, FALSE,
    sizeof(_true), &_true,
    EC_TIMEOUTRXM);

  ec_SDOwrite(slave, ch_indx,
    EL2574_EXTD_CTRL_SEND_SIDX, FALSE,
    sizeof(_true), &_true,
    EC_TIMEOUTRXM);

  return TRUE;
}

int EL2574_checker_board(int slave, int channel, int segments) {
  uint32_t even[8] = {0,255,0,255,0,255,0,255};
  uint32_t odd[8] = {255,0,255,0,255,0,255,0};

  // Loop to cover all segments to be written 
  for (int seg_num = 0; seg_num < segments; seg_num++) {

    // Alternating checker pattern
    if (seg_num % 2 == 0) {
      EL2574_write_index(slave, channel, seg_num, &even[0]);
    } else {
      EL2574_write_index(slave, channel, seg_num, &odd[0]);
    }
    
  }
  // Update and send frame buffer
  EL2574_update_frame_buffer(slave,channel);
  EL2574_send_frame_buffer(slave,channel);

  return TRUE;
}
