/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must--
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file DD.c
        @details Global device driver implementation

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "DD.h"
#include "dd_sd.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
#define DD_LOG_MSG_TAG "DD"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
DD_DATA_OUT_TYPE DD_C::dd_data_out_s;

/*********************************************************************/
/*      TMP-102                                                      */
/*********************************************************************/
DD_TMP_102_C dd_tmp_102_A( 0x49 );


/*********************************************************************/
/*      INA-219                                                      */
/*********************************************************************/
DD_INA_219_C dd_ina_219_A( 0x40, 0.1F );

DD_INA_219_CONFIG_TYPE dd_ina_219_cfg_s = {
    .shunt_voltage_range_e            = DD_INA_219_SHUNT_VOL_RANGE_40MV,
    .shunt_adc_resolution_averaging_e = DD_INA_219_SADC_RES_AVE_12BIT_128S_69MS,
    .bus_voltage_range_e              = DD_INA_219_BUS_VOL_RANGE_16V,
    .bus_adc_resolution_averaging_e   = DD_INA_219_BADC_RES_AVE_12BIT_1S_532US,
    .max_current_mA_f32               = 500.0F
};


/*********************************************************************/
/*      MAX-30102                                                    */
/*********************************************************************/
DD_MAX_30102_C dd_max_30102( 0x57 );

DD_MAX_30102_CONFIG_TYPE dd_max_30102_cfg_s = {
    .mode_cfg_e                    = DD_MAX_30102_MODE_HR,
    .adc_range_cfg_e               = DD_MAX_30102_ADC_RANGE_2048,
    .sample_rate_cfg_e             = DD_MAX_30102_SAMPLE_RATE_100,
    .sample_avg_cfg_e              = DD_MAX_30102_SAMPLE_AVG_4,
    .pulse_width_cfg_e             = DD_MAX_30102_PULSE_WIDTH_411,
    .led_amplitude_cfg_u8          = 0x1F,
    .temp_delay_ticks_cfg_u8       = 10U,
    .prox_threshold_cfg_u8         = 0x01,
    .fifo_roll_over_cfg_b          = TRUE,
    .fifo_a_full_int_enable_cfg_b  = TRUE,
    .fifo_a_full_value_cfg_u8      = 2U,
    .die_temp_rdy_int_enable_cfg_b = TRUE,
    .prox_int_enable_cfg_b         = TRUE
};


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void DD_C::init( void )
{
    DD_SD_C::init();                                                                       /*!< Initialize SD card driver */
    DD_MCPWM_C::init();                                                                    /*!< Initialize MCPWM device driver with default configuration */
    DD_C::dd_data_out_s.p_i2c_error_out_s      = DD_I2C_C::init();                         /*!< Initialize I2C device driver */
    DD_C::dd_data_out_s.p_adc_data_out_s       = DD_ADC_C::init();                         /*!< Initialize ADC device driver */
    DD_C::dd_data_out_s.p_icm_20600_data_out_s = DD_ICM_20600_C::init();                   /*!< Initialize ICM-2600 motion subsystem */
    DD_C::dd_data_out_s.p_max_30102_data_out_s = dd_max_30102.init( &dd_max_30102_cfg_s ); /*!< Initialize MAX-30102 HR+SpO2 subsystem */
    DD_C::dd_data_out_s.p_ina_219_data_out_s   = dd_ina_219_A.init( &dd_ina_219_cfg_s );   /*!< Initialize INA-219 Current/Voltage/Power measuring subsystem */
    DD_C::dd_data_out_s.p_tmp_102_data_out_s   = dd_tmp_102_A.init();                      /*!< Initialize TMP-102 temperature sensor */
}

DD_DATA_OUT_TYPE DD_C::process_inputs( void )
{
    DD_ADC_C::main();       /*!< Schedule ADC basic device driver */
    DD_ICM_20600_C::main(); /*!< Schedule ICM-2600 motion subsystem */
    dd_max_30102.main();    /*!< Schedule MAX-30102 HR+SpO2 subsystem */
    dd_ina_219_A.main();    /*!< Schedule INA-219 (A) Current/Voltage/Power measuring subsystem */
    dd_tmp_102_A.main();    /*!< Schedule TMP-102 (A) temperature sensor */

    /* Return copy of global domain data structure */
    return DD_C::dd_data_out_s;
}


void DD_C::process_outputs( DD_DATA_IN_TYPE &data_in_s )
{
    DD_MCPWM_C::update_channels( data_in_s.mcpwm_data_in_s ); /*!< Update MCPWM module */
}



