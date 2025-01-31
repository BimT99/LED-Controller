/**
 * @file EL2574.hpp
 *
 * @brief Header file for Beckhoff Light array Module "EL2574"
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


#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cstdint>
#include "ethercat.h"

/* Indexes  */
/// Extended control channel 0 index
#define EL2574_EXTD_CH0                 (0x7001)
/// Extended control channel 1 index
#define EL2574_EXTD_CH1                 (0x7011)
/// Extended control channel 2 index
#define EL2574_EXTD_CH2                 (0x7021)
/// Extended control channel 3 index
#define EL2574_EXTD_CH3                 (0x7031)
/// Settings channel 0 index
#define EL2574_STGS_CH0                 (0x8000)
/// Settings channel 1 index
#define EL2574_STGS_CH1                 (0x8010)
/// Settings channel 2 index
#define EL2574_STGS_CH2                 (0x8020)
/// Settings channel 3 index
#define EL2574_STGS_CH3                 (0x8030)

/// RxPDO Index
#define EL2574_RxPDO_IDX                (0x1c12)
/// TxPDO index
#define EL2574_TxPDO_IDX                (0x1c13)



/* Sub-Indexes */

#define ECT_SDO_SUB_ZERO                (0x00)

/// Enable Custom Settings
#define EL2574_ENABLE_CTM_STGS          (0x01)
/// Number Pixels
#define EL2574_NUM_PIXELS               (0x11)
/// Chip Type
#define EL2575_CHIP_TYPE                (0x12)
/// Color Format (See Docs pg 137)
#define EL2574_CLR_FMT                  (0x13)
/// Custom Data Rate
#define EL2574_CTM_DATA_RATE            (0x15)

/// Sub Index for reprogramming RxPDO
#define EL2574_RxPDO_SUB_IDX            (0x01)
/// Sub Index for reprogramming TxPDO
#define EL2574_TxPDO_SUB_IDX            (0x01)

/// Execute sub-index
#define EL2574_EXTD_CTRL_EXECUTE_SIDX   (0x01)
/// Send sub-index
#define EL2574_EXTD_CTRL_SEND_SIDX      (0x02)
/// Update sub-index
#define EL2574_EXTD_CTRL_UPDATE_SIDXX   (0x03)
/// Write sub-index
#define EL2574_EXTD_CTRL_WRITE_SIDXX    (0x04)
/// LED Index sub-index
#define EL2574_EXTD_CTRL_IDX_SIDX       (0x09)
/// Segment Element 0 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_0_SIDX (0x11)
/// Segment Element 1 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_1_SIDX (0x12)
/// Segment Element 2 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_2_SIDX (0x13)
/// Segment Element 3 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_3_SIDX (0x14)
/// Segment Element 4 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_4_SIDX (0x15)
/// Segment Element 5 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_5_SIDX (0x16)
/// Segment Element 6 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_6_SIDX (0x17)
/// Segment Element 7 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_7_SIDX (0x18)

/* Magic Number Defs */
/// Size of grid (DIMENSIONxDIMENSION)
#define GRID_DIMENSION                  (32)
/// Number of elements per LED segment
#define SEGM_SIZE                       (8)
/// For SOEM library, 'all slaves' represented by zero in soem methods
#define ALL_SLAVES                      (0)
/// amount of uint16s used to store txpdo size

// See manual page 122
#define TxPDO_SZ_ENTRY                  (1)
/// amount of uint16s in txpdo
#define EL2574_TxPDO_MAX_SZ             (4)
#define EL2574_TxPDO_SUB_STS_CH0        (0x1A00)
#define EL2574_TxPDO_SUB_STS_CH1        (0x1A10)
#define EL2574_TxPDO_SUB_STS_CH2        (0x1A20)
#define EL2574_TxPDO_SUB_STS_CH3        (0x1A30)

/// amount of uint16s used to store rxpdo size
#define RxPDO_SZ_ENTRY                  (1)
/// amount of uint16s in rxpdo
#define EL2574_RxPDO_MAX_SZ             (4)
#define EL2574_RxPDO_SUB_EXT_CH0        (0x1601)
#define EL2574_RxPDO_SUB_EXT_CH1        (0x1611)
#define EL2574_RxPDO_SUB_EXT_CH2        (0x1621)
#define EL2574_RxPDO_SUB_EXT_CH3        (0x1631)


/* String Defs */
/// EL2574 Name String
#define EL2574_STR "EL2574"

/// @brief Stores EL2574 custom settings confs
///        see page 138 of documentation
typedef struct
{
    /* data */

    //TODO(TIM BARLOW): Fix magic numbers
    uint8_t enbl_cust_stgs {1};
    uint16_t num_pixels {256}; // def = 256
    uint8_t chip_type {28}; // 28 = WS2812(B)
    uint8_t color_format {26}; // 26 = RGB
    uint8_t data_rate {80}; // 80 = 800 kbit/s
} EL2574_ch_confs;

/// @brief Stores confs for all 4 channels
typedef struct PACKED
{
    EL2574_ch_confs ch0_confs;
    EL2574_ch_confs ch1_confs;
    EL2574_ch_confs ch2_confs;
    EL2574_ch_confs ch3_confs;
} EL2574_module_confs;

/// @brief Used to store values that will be passed to pixel frame buffer of EL2574
typedef struct PACKED 
{ 
    uint8_t seg_idx;
    uint32_t ele_0;
    uint32_t ele_1;
    uint32_t ele_2;
    uint32_t ele_3;
    uint32_t ele_4;
    uint32_t ele_5;
    uint32_t ele_6;
    uint32_t ele_7;
} EL2574_extd_ctrl_ch_buf;

/// @brief Used to store all channel buffers per module
typedef struct PACKED
{
    EL2574_extd_ctrl_ch_buf ch0_buf;
    EL2574_extd_ctrl_ch_buf ch1_buf;
    EL2574_extd_ctrl_ch_buf ch2_buf;
    EL2574_extd_ctrl_ch_buf ch3_buf;
} EL2574_extd_ctrl_mod_buf;


/* Methods */

/// @brief Checks if slave module is EL2574
/// @param name 
/// @return 0 is not EL2574, >1 if is EL2574
int is_EL2574(char* name);

/// @brief Configures EL2574 slaves 
/// @param confs 
/// @return 
int configure_EL2574(int slave);

/// @brief Takes pointer to 8 elements which are written into the pixel buffer
/// @param slave Slave number
/// @param channel Channel number (zero indexed)
/// @param index Segment index
/// @param element_ptr Pointer to 1st element of a segment 
/// @return 1 for successful writing, 0 otherwise
int EL2574_write_index(int slave, int channel, int index, uint32_t* element_ptr);

/// @brief Updates pixel buffer to frame buffer
/// @param slave Slave number
/// @param channel Channel number
/// @return Returns 1 for success, 0 otherwise
int EL2574_update_frame_buffer(int slave, int channel);

/// @brief Sends Frame buffer to LED array 
/// @param slave Slave Number
/// @param channel Channel Number
/// @return Returns 1 for success, 0 otherwise
int EL2574_send_frame_buffer(int slave, int channel);

/// @brief Sends checkerboard grid to light array
/// @param grid_size length/width of pixels on square grid
/// @return 1 if succesful, 0 if not
int EL2574_checker_board(int slave, int channel, int segments);
