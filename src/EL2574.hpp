/**
 * @file EL2574.hpp
 *
 * @brief Header file for Beckhoff Light array Module "EL2574"
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


#include <stdio.h>
#include <string.h>
#include <cstdint>

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

/* Sub-Indexes */
/// Execute sub-index
#define EL2574_EXTD_CTRL_EXECUTE_SIDX   (01)
/// Send sub-index
#define EL2574_EXTD_CTRL_SEND_SIDX      (02)
/// Update sub-index
#define EL2574_EXTD_CTRL_UPDATE_SIDXX   (03)
/// Write sub-index
#define EL2574_EXTD_CTRL_WRITE_SIDXX    (04)
/// LED Index sub-index
#define EL2574_EXTD_CTRL_IDX_SIDX       (09)
/// Segment Element 0 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_0_SIDX (11)
/// Segment Element 1 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_1_SIDX (12)
/// Segment Element 2 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_2_SIDX (13)
/// Segment Element 3 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_3_SIDX (14)
/// Segment Element 4 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_4_SIDX (15)
/// Segment Element 5 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_5_SIDX (16)
/// Segment Element 6 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_6_SIDX (17)
/// Segment Element 7 Sub-Index
#define EL2574_EXTD_CTRL_SEG_ELE_7_SIDX (18)

/* Magic Number Defs */
/// Size of grid (DIMENSIONxDIMENSION)
#define GRID_DIMENSION                  (32)
/// Number of Segment Elements per LED index
#define ELEMS_SIZE                      (8)

/* String Defs */
/// EL2574 Name String
#define EL2574_STR "EL2574"

/* Variables and Data Structures */
uint8_t pixel_buffer  [GRID_DIMENSION][GRID_DIMENSION];

/// @brief Stores EL2574 custom settings confs
///        see page 138 of documentation
typedef struct PACKED
{
    /* data */
    uint8_t enbl_cust_stgs = 1;
    uint16_t num_pixels = 256;
    uint8_t chip_type = 28;
    uint8_t color_format = 26;
    uint8_t data_rate = 80;
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
int configure_EL2574(EL2574_module_confs confs);
