/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

*********************************************************************/

/**
 * @file dd_icm-20600.h
 * @details Some detailed description
 */

#ifndef DD_MAX_30102_H
#define DD_MAX_30102_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_types.h"


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

// Status Registers
#define DD_MAX_30105_INTSTAT1                   0x00
#define DD_MAX_30105_INTSTAT2                   0x01
#define DD_MAX_30105_INTENABLE1                 0x02
#define DD_MAX_30105_INTENABLE2                 0x03

// FIFO Registers
#define DD_MAX_30105_FIFOWRITEPTR               0x04
#define DD_MAX_30105_FIFOOVERFLOW               0x05
#define DD_MAX_30105_FIFOREADPTR                0x06
#define DD_MAX_30105_FIFODATA                   0x07

// Configuration Registers
#define DD_MAX_30105_FIFOCONFIG                 0x08
#define DD_MAX_30105_MODECONFIG                 0x09
#define DD_MAX_30105_PARTICLECONFIG             0x0A    // Note, sometimes listed as "SPO2" config in datasheet (pg. 11)
#define DD_MAX_30105_LED1_PULSEAMP              0x0C
#define DD_MAX_30105_LED2_PULSEAMP              0x0D
#define DD_MAX_30105_LED3_PULSEAMP              0x0E
#define DD_MAX_30105_LED_PROX_AMP               0x10
#define DD_MAX_30105_MULTILEDCONFIG1            0x11
#define DD_MAX_30105_MULTILEDCONFIG2            0x12

// Die Temperature Registers
#define DD_MAX_30105_DIETEMPINT                 0x1F
#define DD_MAX_30105_DIETEMPFRAC                0x20
#define DD_MAX_30105_DIETEMPCONFIG              0x21

// Proximity Function Registers
#define DD_MAX_30105_PROXINTTHRESH              0x30

// Part ID Registers
#define DD_MAX_30105_REVISIONID                 0xFE
#define DD_MAX_30105_PARTID                     0xFF    // Should always be 0x15. Identical to MAX30102.

// DD_MAX_30105 Commands
// Interrupt configuration (pg 13, 14)
#define DD_MAX_30105_INT_A_FULL_MASK            0x7F /* (byte)~0b10000000 */
#define DD_MAX_30105_INT_A_FULL_ENABLE          0x80
#define DD_MAX_30105_INT_A_FULL_DISABLE         0x00

#define DD_MAX_30105_INT_DATA_RDY_MASK          0xBF /* (byte)~0b01000000 */
#define DD_MAX_30105_INT_DATA_RDY_ENABLE        0x40
#define DD_MAX_30105_INT_DATA_RDY_DISABLE       0x00

#define DD_MAX_30105_INT_ALC_OVF_MASK           0xDF /*(byte)~0b00100000 */
#define DD_MAX_30105_INT_ALC_OVF_ENABLE         0x20
#define DD_MAX_30105_INT_ALC_OVF_DISABLE        0x00

#define DD_MAX_30105_INT_PROX_INT_MASK          0xEF /* (byte)~0b00010000 */
#define DD_MAX_30105_INT_PROX_INT_ENABLE        0x10
#define DD_MAX_30105_INT_PROX_INT_DISABLE       0x00

#define DD_MAX_30105_INT_DIE_TEMP_RDY_MASK      0xFD /* (byte)~0b00000010 */
#define DD_MAX_30105_INT_DIE_TEMP_RDY_ENABLE    0x02
#define DD_MAX_30105_INT_DIE_TEMP_RDY_DISABLE   0x00

#define DD_MAX_30105_SAMPLEAVG_MASK             0x1F /* (byte)~0b11100000 */
#define DD_MAX_30105_SAMPLEAVG_1                0x00
#define DD_MAX_30105_SAMPLEAVG_2                0x20
#define DD_MAX_30105_SAMPLEAVG_4                0x40
#define DD_MAX_30105_SAMPLEAVG_8                0x60
#define DD_MAX_30105_SAMPLEAVG_16               0x80
#define DD_MAX_30105_SAMPLEAVG_32               0xA0

#define DD_MAX_30105_ROLLOVER_MASK              0xEF
#define DD_MAX_30105_ROLLOVER_ENABLE            0x10
#define DD_MAX_30105_ROLLOVER_DISABLE           0x00

#define DD_MAX_30105_A_FULL_MASK                0xF0

// Mode configuration commands (page 19)
#define DD_MAX_30105_SHUTDOWN_MASK              0x7F
#define DD_MAX_30105_SHUTDOWN                   0x80
#define DD_MAX_30105_WAKEUP                     0x00

#define DD_MAX_30105_RESET_MASK                 0xBF
#define DD_MAX_30105_RESET                      0x40

#define DD_MAX_30105_MODE_MASK                  0xF8
#define DD_MAX_30105_MODE_REDONLY               0x02
#define DD_MAX_30105_MODE_REDIRONLY             0x03
#define DD_MAX_30105_MODE_MULTILED              0x07

// Particle sensing configuration commands (pgs 19-20)
#define DD_MAX_30105_ADCRANGE_MASK              0x9F
#define DD_MAX_30105_ADCRANGE_2048              0x00
#define DD_MAX_30105_ADCRANGE_4096              0x20
#define DD_MAX_30105_ADCRANGE_8192              0x40
#define DD_MAX_30105_ADCRANGE_16384             0x60

#define DD_MAX_30105_SAMPLERATE_MASK            0xE3
#define DD_MAX_30105_SAMPLERATE_50              0x00
#define DD_MAX_30105_SAMPLERATE_100             0x04
#define DD_MAX_30105_SAMPLERATE_200             0x08
#define DD_MAX_30105_SAMPLERATE_400             0x0C
#define DD_MAX_30105_SAMPLERATE_800             0x10
#define DD_MAX_30105_SAMPLERATE_1000            0x14
#define DD_MAX_30105_SAMPLERATE_1600            0x18
#define DD_MAX_30105_SAMPLERATE_3200            0x1C

#define DD_MAX_30105_PULSEWIDTH_MASK            0xFC
#define DD_MAX_30105_PULSEWIDTH_69              0x00
#define DD_MAX_30105_PULSEWIDTH_118             0x01
#define DD_MAX_30105_PULSEWIDTH_215             0x02
#define DD_MAX_30105_PULSEWIDTH_411             0x03

//Multi-LED Mode configuration (pg 22)
#define DD_MAX_30105_SLOT1_MASK                 0xF8
#define DD_MAX_30105_SLOT2_MASK                 0x8F
#define DD_MAX_30105_SLOT3_MASK                 0xF8
#define DD_MAX_30105_SLOT4_MASK                 0x8F

#define DD_MAX_30105_SLOT_NONE                  0x00
#define DD_MAX_30105_SLOT_RED_LED               0x01
#define DD_MAX_30105_SLOT_IR_LED                0x02
#define DD_MAX_30105_SLOT_GREEN_LED             0x03
#define DD_MAX_30105_SLOT_NONE_PILOT            0x04
#define DD_MAX_30105_SLOT_RED_PILOT             0x05
#define DD_MAX_30105_SLOT_IR_PILOT              0x06
#define DD_MAX_30105_SLOT_GREEN_PILOT           0x07

#define MAX_30105_EXPECTEDPARTID                0x15


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/**
 * @details This function returns pointer to the osl object for the given id
 * of the linked environment object.
 * @param[in] linked_env_id_u8 is id of the linked environment object of the requested osl object
 * @return pointer to osl object or NULL if no appropriate object exists.
 */

#endif /* DD_MAX_30102_H */
