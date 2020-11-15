/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must--
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file DD.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "DD.h"

#include "dd_i2c.h"
#include "dd_ina-219.h"

#include "esp_log.h"

#include <stdio.h>
#include <string.h>

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
#define DD_LOG_MSG_TAG "DD"

DD_DATA_TYPE dd_data_s;

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
DD_INA_219_C dd_ina_219_A( 0x40 );
DD_INA_219_DATA_IN_TYPE dd_ina_219_cfg_s = {
    .shunt_voltage_range_e            = DD_INA_219_SHUNT_VOL_RANGE_40MV,
    .shunt_adc_resolution_averaging_e = DD_INA_219_SADC_RES_AVE_12BIT_128S_69MS,
    .bus_voltage_range_e              = DD_INA_219_BUS_VOL_RANGE_16V,
    .bus_adc_resolution_averaging_e   = DD_INA_219_BADC_RES_AVE_12BIT_1S_532US,
    .max_current_mA_f32               = 500.0F
};

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void dd_init( void )
{
  //  dd_sd_init();        /* Initialize SD card driver */
    DD_I2C_C::init();       /* Initialize I2C basic device driver */
   // dd_adc_init();       /* Initialize ADC basic device driver */
   // dd_mcpwm_init();     /* Initialize MCPWM basic device driver */
   // dd_mcpwm_if_init();  /* Initialize MCPWM input interface module */
   // dd_icm_20600_init(); /* Initialize ICM-2600 motion subsystem */
   // dd_max_30102_init(); /* Initialize MAX-30102 HR+SpO2 subsystem */
   // dd_ina_219_init();   /* Initialize INA-219 Current/Voltage/Power measuring subsystem */
    dd_data_s.p_ina_219_data_out_s = dd_ina_219_A.init( &dd_ina_219_cfg_s );
    // dd_tmp_102_init();   /* Initialize TMP-102 temperature sensor */
}

DD_DATA_TYPE dd_main( void )
{
  //  dd_adc_main();       /* Schedule ADC basic device driver */
  //  dd_mcpwm_if_main();  /* Schedule MCPWM input interface module ( must be called before dd_mcpwm_main() ) */
  //  dd_mcpwm_main();     /* Schedule MCPWM basic device driver */
  //  dd_icm_20600_main(); /* Schedule ICM-2600 motion subsystem */
  //  dd_max_30102_main(); /* Schedule MAX-30102 HR+SpO2 subsystem */
    dd_ina_219_A.main();   /* Schedule INA-219 (A) Current/Voltage/Power measuring subsystem */
  //  dd_tmp_102_main();   /* Schedule TMP-102 temperature sensor */

    return dd_data_s;
}
