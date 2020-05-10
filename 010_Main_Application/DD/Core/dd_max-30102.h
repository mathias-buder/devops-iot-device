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
#include "../Config/dd_max-30102_Cfg.h"


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

// Status Registers
#define DD_MAX_30102_INT_STAT_1                 0x00
#define DD_MAX_30102_INT_STAT_2                 0x01
#define DD_MAX_30102_INT_ENABLE_1               0x02
#define DD_MAX_30102_INT_ENABLE_2               0x03

// FIFO Registers
#define DD_MAX_30102_FIFO_WRITE_PTR             0x04
#define DD_MAX_30102_FIFO_OVER_FLOW             0x05
#define DD_MAX_30102_FIFO_READ_PTR              0x06
#define DD_MAX_30102_FIFO_DATA                  0x07

// Configuration Registers
#define DD_MAX_30102_FIFO_CONFIG                0x08
#define DD_MAX_30102_MODE_CONFIG                0x09
#define DD_MAX_30102_SPO2_CONFIG                0x0A
#define DD_MAX_30102_LED1_PULSE_AMP             0x0C
#define DD_MAX_30102_LED2_PULSE_AMP             0x0D
#define DD_MAX_30102_LED_PROX_AMP               0x10
#define DD_MAX_30102_MULTI_LED_CONFIG_1         0x11
#define DD_MAX_30102_MULTI_LED_CONFIG_2         0x12

// Die Temperature Registers
#define DD_MAX_30102_DIE_TEMP_INT               0x1F
#define DD_MAX_30102_DIE_TEMP_FRAC              0x20
#define DD_MAX_30102_DIE_TEMP_CONFIG            0x21
#define DD_MAX_30102_DIE_TEMP_FRAC_RES          0.0625F

// Proximity Function Registers
#define DD_MAX_30102_PROX_INT_THRESH            0x30

// Part ID Registers
#define DD_MAX_30102_REVISION_ID                0xFE
#define DD_MAX_30102_PART_ID                    0xFF    // Should always be 0x15. Identical to MAX30102.

/* Interrupt configuration for Interrupt Status 1
 * and Interrupt Status 2*/

/* A_FULL: FIFO Almost Full Flag */
#define DD_MAX_30102_INT_A_FULL_MASK            0x7F /* (byte)~0b10000000 */
#define DD_MAX_30102_INT_A_FULL_ENABLE          0x80
#define DD_MAX_30102_INT_A_FULL_DISABLE         0x00

/* PPG_RDY: New FIFO Data Ready */
#define DD_MAX_30102_INT_PPG_RDY_MASK           0xBF /* (byte)~0b01000000 */
#define DD_MAX_30102_INT_PPG_RDY_ENABLE         0x40
#define DD_MAX_30102_INT_PPG_RDY_DISABLE        0x00

/* ALC_OVF: Ambient Light Cancellation Overflow */
#define DD_MAX_30102_INT_ALC_OVF_MASK           0xDF /*(byte)~0b00100000 */
#define DD_MAX_30102_INT_ALC_OVF_ENABLE         0x20
#define DD_MAX_30102_INT_ALC_OVF_DISABLE        0x00

/* PROX_INT: Proximity Threshold Triggered */
#define DD_MAX_30102_INT_PROX_INT_MASK          0xEF /* (byte)~0b00010000 */
#define DD_MAX_30102_INT_PROX_INT_ENABLE        0x10
#define DD_MAX_30102_INT_PROX_INT_DISABLE       0x00

/* PWR_RDY: Power Ready Flag */
#define DD_MAX_30102_INT_PWR_RDY_MASK           0xFE /* (byte)~0b00000001 */
#define DD_MAX_30102_INT_PWR_RDY_ENABLE         0x01
#define DD_MAX_30102_INT_PWR_RDY_DISABLE        0x00

/* DIE_TEMP_RDY: Internal Temperature Ready Flag */
#define DD_MAX_30102_INT_DIE_TEMP_RDY_MASK      0xFD /* (byte)~0b00000010 */
#define DD_MAX_30102_INT_DIE_TEMP_RDY_ENABLE    0x02
#define DD_MAX_30102_INT_DIE_TEMP_RDY_DISABLE   0x00

/* Sample Averaging (SMP_AVE) */
#define DD_MAX_30102_SMP_AVG_MASK               0x1F /* (byte)~0b11100000 */
#define DD_MAX_30102_SMP_AVG_1                  0x00
#define DD_MAX_30102_SMP_AVG_2                  0x20
#define DD_MAX_30102_SMP_AVG_4                  0x40
#define DD_MAX_30102_SMP_AVG_8                  0x60
#define DD_MAX_30102_SMP_AVG_16                 0x80
#define DD_MAX_30102_SMP_AVG_32                 0xA0

/* FIFO Rolls on Full (FIFO_ROLLOVER_EN) */
#define DD_MAX_30102_ROLL_OVER_MASK             0xEF /* (byte)~0b00010000 */
#define DD_MAX_30102_ROLL_OVER_ENABLE           0x10
#define DD_MAX_30102_ROLL_OVER_DISABLE          0x00

/* FIFO Almost Full Value (FIFO_A_FULL) */
#define DD_MAX_30102_A_FULL_MASK                0xF0 /* (byte)~0b00001111 */
#define DD_MAX_30102_A_FULL_MAX_VAL             0x0F /* See datasheet, pg. 17 */

/* Mode configuration commands */

/* Shutdown Control (SHDN) */
#define DD_MAX_30102_SHUTDOWN_MASK              0x7F
#define DD_MAX_30102_SHUTDOWN                   0x80
#define DD_MAX_30102_WAKEUP                     0x00

/* Reset Control (RESET) */
#define DD_MAX_30102_RESET_MASK                 0xBF
#define DD_MAX_30102_RESET                      0x40

/* Mode Control */
#define DD_MAX_30102_LED_MODE_MASK              0xF8
#define DD_MAX_30102_LED_MODE_HR                0x02
#define DD_MAX_30102_LED_MODE_SPO2              0x03
#define DD_MAX_30102_LED_MODE_MULTI             0x07

/* SpO2 Configuration */

/* SpO2 ADC Range Control */
#define DD_MAX_30102_SPO2_ADC_RANGE_MASK        0x9F
#define DD_MAX_30102_SPO2_ADC_RANGE_2048        0x00
#define DD_MAX_30102_SPO2_ADC_RANGE_4096        0x20
#define DD_MAX_30102_SPO2_ADC_RANGE_8192        0x40
#define DD_MAX_30102_SPO2_ADC_RANGE_16384       0x60

/* SpO2 Sample Rate Control */
#define DD_MAX_30102_SPO2_SMP_RATE_MASK         0xE3
#define DD_MAX_30102_SPO2_SMP_RATE_50           0x00
#define DD_MAX_30102_SPO2_SMP_RATE_100          0x04
#define DD_MAX_30102_SPO2_SMP_RATE_200          0x08
#define DD_MAX_30102_SPO2_SMP_RATE_400          0x0C
#define DD_MAX_30102_SPO2_SMP_RATE_800          0x10
#define DD_MAX_30102_SPO2_SMP_RATE_1000         0x14
#define DD_MAX_30102_SPO2_SMP_RATE_1600         0x18
#define DD_MAX_30102_SPO2_SMP_RATE_3200         0x1C

/* LED Pulse Width Control and ADC Resolution */
#define DD_MAX_30102_LED_PULSE_WIDTH_MASK       0xFC /* (byte)~0b00000011 */
#define DD_MAX_30102_LED_PULSE_WIDTH_69         0x00
#define DD_MAX_30102_LED_PULSE_WIDTH_118        0x01
#define DD_MAX_30102_LED_PULSE_WIDTH_215        0x02
#define DD_MAX_30102_LED_PULSE_WIDTH_411        0x03

/* Multi-LED Mode Control */
#define DD_MAX_30102_SLOT_1_MASK                0xF8
#define DD_MAX_30102_SLOT_2_MASK                0x8F
#define DD_MAX_30102_SLOT_3_MASK                0xF8
#define DD_MAX_30102_SLOT_4_MASK                0x8F

#define DD_MAX_30102_SLOT_NONE                  0x00
#define DD_MAX_30102_SLOT_RED_LED               0x01
#define DD_MAX_30102_SLOT_IR_LED                0x02
#define DD_MAX_30102_SLOT_RED_PILOT             0x05
#define DD_MAX_30102_SLOT_IR_PILOT              0x06


#define DD_MAX_30102_FIFO_SIZE                  32U /* FIFO size in bytes */

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/**
 * @details This function returns pointer to the osl object for the given id
 * of the linked environment object.
 * @param[in] linked_env_id_u8 is id of the linked environment object of the requested osl object
 * @return pointer to osl object or NULL if no appropriate object exists.
 */
BOOLEAN dd_max_30102_init( void );
void    dd_max_30102_main( void );

#endif /* DD_MAX_30102_H */
