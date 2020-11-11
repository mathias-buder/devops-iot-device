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
#define DD_INA_219_CONFIG                       0x00   /**< All-register reset, settings for bus voltage range, PGA Gain, ADC resolution/averaging (16 bit)*/

/* Data Output Registers */
#define DD_INA_219_SHUNT_VOLTAGE_DATA           0x01   /**< Shunt voltage measurement data */
#define DD_INA_219_BUS_VOLTAGE_DATA             0x02   /**< Bus voltage measurement data */
#define DD_INA_219_POWER_DATA                   0x03   /**< Power measurement data */
#define DD_INA_219_CURRENT_DATA                 0x04   /**< Contains the value of the current flowing through the shunt resistor */

/* Calibration Register */
#define DD_INA_219_CALIB_DATA                   0x05   /**< Sets full-scale range and LSB of current and power measurements. Overall system calibration.*/

/* RST: System reset Flag
 * setting this bit to '1' generates a system reset that is
 * the same as power-on reset. Resets all registers to default
 * values; this bit self-clears. */
#define DD_INA_219_SYS_RST_MASK                 0x7FFF /**< (word) ~0b 10000000 00000000 */
#define DD_INA_219_SYS_RST_ENABLE               0x8000

/* BRNG: Bus Voltage Range
 * 0 = 16V FSR
 * 1 = 32V FSR (default value) */
#define DD_INA_219_BRNG_MASK                    0xDFFF /**< (word) ~0b 00100000 00000000 */
#define DD_INA_219_BRNG_RANGE_16V               0x0000 /**< 16V FSR */
#define DD_INA_219_BRNG_RANGE_32V               0x2000 /**< 32V FSR, default */

/* PG: PGA (Shunt Voltage Only)
 * Sets PGA gain and range. Note that the PGA defaults to ÷8 (320mV range).
 * Table 4 (datasheet pg. 19) shows the gain and range for the various
 * product gain settings. */
#define DD_INA_219_PG_MASK                      0xE7FF /**< (word) ~0b 00110000 00000000 */
#define DD_INA_219_PG_GAIN_1_40MV               0x0000 /**< Full-scale current sense (input) voltage range: +/- 40 mV */
#define DD_INA_219_PG_GAIN_1_2_80MV             0x0800 /**< Full-scale current sense (input) voltage range: +/- 80 mV */
#define DD_INA_219_PG_GAIN_1_4_160MV            0x1000 /**< Full-scale current sense (input) voltage range: +/- 160 mV */
#define DD_INA_219_PG_GAIN_1_8_320MV            0x1800 /**< Full-scale current sense (input) voltage range: +/- 320 mV, default */

/* BADC: BADC Bus ADC Resolution/Averaging
 * These bits adjust the Bus ADC resolution (9-, 10-, 11-, or 12-bit)
 * or set the number of samples used when averaging results for the Bus
 * Voltage Register (02h). */
#define DD_INA_219_BADC_MASK                    0xF87F /**< (word) ~0b 00000111 10000000 */
#define DD_INA_219_BADC_9BIT_1S_84US            0x0000 /**< 9-bit bus Res = 0..511 */
#define DD_INA_219_BADC_10BIT_1S_148US          0x0080 /**< 10-bit bus res = 0..1023 */
#define DD_INA_219_BADC_11BIT_1S_276US          0x0100 /**< 11-bit bus res = 0..2047 */
#define DD_INA_219_BADC_12BIT_1S_532US          0x0180 /**< 12-bit bus res = 0..4097, default*/
#define DD_INA_219_BADC_12BIT_2S_1060US         0x0480 /**< 2 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_4S_2130US         0x0500 /**< 4 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_8S_4260US         0x0580 /**< 8 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_16S_8512US        0x0600 /**< 16 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_32S_17MS          0x0680 /**< 32 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_64S_34MS          0x0700 /**< 64 x 12-bit bus samples averaged together */
#define DD_INA_219_BADC_12BIT_128S_69MS         0x0780 /**< 128 x 12-bit bus samples averaged together */

/* SADC: SADC Shunt ADC Resolution/Averaging
 * These bits adjust the Shunt ADC resolution (9-, 10-, 11-, or 12-bit) or set the number of samples
 * used when averaging results for the Shunt Voltage Register (01h). BADC (Bus) and SADC (Shunt) ADC
 * resolution/averaging and conversion time settings are shown in Table 5. (datasheet pg. 20) */
#define DD_INA_219_SADC_MASK                    0xFF87 /**< (word) ~0b 00000000 01111000 */
#define DD_INA_219_SADC_9BIT_1S_84US            0x0000 /**< 9-bit shunt Res = 0..511 */
#define DD_INA_219_SADC_10BIT_1S_148US          0x0008 /**< 10-bit shunt res = 0..1023 */
#define DD_INA_219_SADC_11BIT_1S_276US          0x0010 /**< 11-bit shunt res = 0..2047 */
#define DD_INA_219_SADC_12BIT_1S_532US          0x0018 /**< 12-bit shunt res = 0..4097, default */
#define DD_INA_219_SADC_12BIT_2S_1060US         0x0048 /**< 2 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_4S_2130US         0x0050 /**< 4 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_8S_4260US         0x0058 /**< 8 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_16S_8512US        0x0060 /**< 16 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_32S_17MS          0x0068 /**< 32 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_64S_34MS          0x0070 /**< 64 x 12-bit shunt samples averaged together */
#define DD_INA_219_SADC_12BIT_128S_69MS         0x0078 /**< 128 x 12-bit shunt samples averaged together */

/* MODE: Operating Mode
 * Selects continuous, triggered, or power-down mode of operation.
 * These bits default to continuous shunt and bus measurement mode.
 * The mode settings are shown in Table 6. (datasheet pg. 20) */
#define DD_INA_219_MODE_MASK                    0xFFF8 /**< (word) ~0b 00000000 00000111 */
#define DD_INA_219_MODE_POWER_DOWN              0x0000
#define DD_INA_219_MODE_SHUNT_VOLT_TRG          0x0001 /**< Shunt voltage, triggered */
#define DD_INA_219_MODE_BUS_VOLT_TRG            0x0002 /**< Bus voltage, triggered */
#define DD_INA_219_MODE_SHUNT_BUS_VOLT_TRG      0x0003 /**< Shunt and bus, triggered */
#define DD_INA_219_MODE_ADC_OFF                 0x0004 /**< ADC off (disabled) */
#define DD_INA_219_MODE_BUS_VOLT_CONT           0x0005 /**< Shunt voltage, continuous*/
#define DD_INA_219_MODE_SHUNT_VOLT_CONT         0x0006 /**< Bus voltage, continuous */
#define DD_INA_219_MODE_SHUNT_BUS_VOLT_CONT     0x0007 /**< Shunt and bus, continuous, default */

/* Bus Voltage Register
 * The Bus Voltage register stores the most recent bus voltage reading, VBUS.
 * At full-scale range = 32 V (decimal = 8000, hex = 1F40), and LSB = 4 mV. */
#define DD_INA_219_V_BUS_LSB_VOLT               ( 0.004F ) /**< V/LSB */
#define DD_INA_219_V_BUS_LSB_MILLI_VOLT         ( 4.0F )  /**< mV/LSB */

#define DD_INA_219_V_BUS_STAT_MASK              0xFFFC /**< (word) ~0b 00000000 00000011 */
#define DD_INA_219_V_BUS_STAT_OVF               0x0001 /**< Math Overflow Flag */
#define DD_INA_219_V_BUS_STAT_CNVR              0x0002 /**< Conversion Ready */

/* Shunt Voltage Register
 * The Shunt Voltage register stores the current shunt voltage reading, VSHUNT.
 * At PGA = /1, full-scale range = ±40 mV (decimal = 4000). For VSHUNT = +40 mV,
 * Value = 0FA0h; For VSHUNT = –40 mV, Value = F060h; and LSB = 10uV. */
#define DD_INA_219_V_SHUNT_LSB_VOLT            ( 0.00001F ) /**< Shunt voltage LSB @unit Volt/LSB */
#define DD_INA_219_V_SHUNT_LSB_MILLI_VOLT      ( 0.01F )    /**< Shunt voltage LSB @unit mVolt/LSB */

/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
BOOLEAN dd_ina_219_init( void );
void    dd_ina_219_main( void );

#endif /* DD_INA_219_H */
