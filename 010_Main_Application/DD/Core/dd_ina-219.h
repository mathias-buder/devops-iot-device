/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_ina_219.h
        @details Some detailed description

*********************************************************************/
#ifndef DD_INA_219_H
#define DD_INA_219_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_types.h"
#include "../Config/dd_ina-219_cfg.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

/* Configuration Registers */
#define DD_INA_219_CONFIG                       0x00

/* Data Output Registers */
#define DD_INA_219_SHUNT_VOLTAGE_DATA           0x01
#define DD_INA_219_BUS_VOLTAGE_DATA             0x02
#define DD_INA_219_POWER_DATA                   0x03
#define DD_INA_219_CURRENT_DATA                 0x04

/* RST: System reset Flag
 * etting this bit to '1' generates a system reset that is
 * the same as power-on reset. Resets all registers to default
 * values; this bit self-clears. */
#define DD_INA_219_SYS_RST_MASK                 0x7F /* (byte)~0b10000000 */
#define DD_INA_219_SYS_RST_ENABLE               0x80

/* BRNG: Bus Voltage Range
 * 0 = 16V FSR
 * 1 = 32V FSR (default value) */
#define DD_INA_219_BRNG_MASK                    0x7F /* (byte)~0b10000000 */
#define DD_INA_219_BRNG_RANGE_16V               0x80
#define DD_INA_219_BRNG_RANGE_32V               0x80

/* PG: PGA (Shunt Voltage Only)
 * Sets PGA gain and range. Note that the PGA defaults to ÷8 (320mV range).
 * Table 4 (datasheet pg. 19) shows the gain and range for the various
 * product gain settings. */
#define DD_INA_219_PGA_MASK                     0x7F /* (byte)~0b10000000 */
#define DD_INA_219_PGA_GAIN_1                   0x80
#define DD_INA_219_PGA_GAIN_1_2                 0x80
#define DD_INA_219_PGA_GAIN_1_4                 0x80
#define DD_INA_219_PGA_GAIN_1_8                 0x80 /* default */


/* BADC: BADC Bus ADC Resolution/Averaging
 * These bits adjust the Bus ADC resolution (9-, 10-, 11-, or 12-bit)
 * or set the number of samples used when averaging results for the Bus
 * Voltage Register (02h). */
#define DD_INA_219_BADC_MASK                    0x7F /* (byte)~0b10000000 */
#define DD_INA_219_PGA_GAIN_1                   0x80
#define DD_INA_219_PGA_GAIN_1_2                 0x80
#define DD_INA_219_PGA_GAIN_1_4                 0x80
#define DD_INA_219_PGA_GAIN_1_8                 0x80 /* default */

/* MODE: Operating Mode
 * Selects continuous, triggered, or power-down mode of operation.
 * These bits default to continuous shunt and bus measurement mode.
 * The mode settings are shown in Table 6. */
#define DD_INA_219_MODE_MASK                    0x7F /* (byte)~0b10000000 */
#define DD_INA_219_MODE_POWER_DOWN              0x80
#define DD_INA_219_MODE_SHUNT_VOLTAGE_TRG       0x80 /* Shunt voltage, triggered */
#define DD_INA_219_MODE_POWER_DOWN              0x80 /* Bus voltage, triggered */
#define DD_INA_219_MODE_POWER_DOWN              0x80 /* Shunt and bus, triggered */
#define DD_INA_219_MODE_POWER_DOWN              0x80 /* ADC off (disabled) */
#define DD_INA_219_MODE_POWER_DOWN              0x80 /* Shunt voltage, continuous*/
#define DD_INA_219_MODE_POWER_DOWN              0x80 /* Bus voltage, continuous */
#define DD_INA_219_MODE_POWER_DOWN              0x80 /* Shunt and bus, continuous */


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
BOOLEAN dd_ina_219_init( void );
void    dd_ina_219_main( void );

#endif /* DD_INA_219_H */
