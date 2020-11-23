/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file os_main.c
        @details Some detailed description

*********************************************************************/
#include "OS.h"

#include <stdio.h>

#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/task.h"

#include "Core/os_tm.h"

#include "../DD/DD.h"
#include "../DLG/DLG.h"
#include "../SENSE/SENSE.h"
//#include "../VE/VE.h"

DD_DATA_OUT_TYPE    dd_data_out_s;
DD_DATA_IN_TYPE     dd_data_in_s;
LOG_DATA_IN_TYPE    log_data_in_s;
SENSE_DATA_IN_TYPE  sense_data_in_s;
SENSE_DATA_OUT_TYPE sense_data_out_s;


void os_collect_sense_data_in( const DD_DATA_OUT_TYPE& dd_data_out_s,
                               SENSE_DATA_IN_TYPE&     sense_data_in_s );

void os_collect_dlg_data_in( LOG_DATA_IN_TYPE&          log_data_in_s,
                             const DD_DATA_IN_TYPE&     dd_data_in_s,
                             const DD_DATA_OUT_TYPE&    dd_data_out_s,
                             const SENSE_DATA_OUT_TYPE& sense_data_out_s );


extern "C" void app_main()
{
    /* Get current OS tick count */
    TickType_t initial_tick_cnt_u32 = xTaskGetTickCount();

//    os_tm_init();    /* Initialize Global Time Module */
//    os_wifi_init();  /* Initialize and connect to wifi network */
    DD_C::init();      /* Initialize Device Driver Domain ( DD ) */
    SENSE_C::init();   /* Initialize Sensor Processing Domain ( SENSE ) */
//    ve_init();       /* Initialize Vibration Engine Domain ( VE ) */
    DLG_C::init();     /* Initialize Data Logging Domain( DLG ) */

    /***********************************************
     *          Enter Infinite Main Loop           *
     ***********************************************/
    while ( TRUE )
    {
        /* Schedule every OS_MAIN_CYCLE_TIME_INCREMENT ms */
        vTaskDelayUntil( &initial_tick_cnt_u32, (TickType_t) OS_MAIN_CYCLE_TIME_INCREMENT );

        //      os_tm_update(); /* Update Global Time Module */

        dd_data_out_s    = DD_C::process_inputs(); /* Get-and-Process all inputs ( DD ) */

        /* Collected all data to be transfered into SENSE domain */
        os_collect_sense_data_in( dd_data_out_s,
                                  sense_data_in_s );

        /* Schedule Sensor Processing Domain ( SENSE ) */
        sense_data_out_s = SENSE_C::main( sense_data_in_s );

        //       VE_DATA_IN_TYPE = map_dd_to_sense_ve(DD_DATA_OUT_TYPE, SENSE_DATA_OUT_TYPE)
        //       ve_main();      /* Schedule Vibration Engine Domain ( VE ) */

        //      DD_DATA_IN_TYPE = collect_dd_data_in(SENSE_DATA_OUT_TYPE)
        DD_C::process_outputs( dd_data_in_s );



        /* Collected all data to be logged */
        os_collect_dlg_data_in( log_data_in_s,
                                dd_data_in_s,
                                dd_data_out_s,
                                sense_data_out_s );

        /* Schedule Data Logging Domain( DLG ) */
        DLG_C::main( log_data_in_s );
    }
}



void os_collect_sense_data_in( const DD_DATA_OUT_TYPE& dd_data_out_s,
                               SENSE_DATA_IN_TYPE&     sense_data_in_s )
{
    sense_data_in_s.ts_data_in_s.adc_raw_level_f32 = dd_data_out_s.p_adc_data_out_s->raw_level_f32;
    sense_data_in_s.ts_data_in_s.adc_voltage_mV_u16 = dd_data_out_s.p_adc_data_out_s->voltage_u16;
}



void os_collect_dlg_data_in( LOG_DATA_IN_TYPE&          log_data_in_s,
                             const DD_DATA_IN_TYPE&     dd_data_in_s,
                             const DD_DATA_OUT_TYPE&    dd_data_out_s,
                             const SENSE_DATA_OUT_TYPE& sense_data_out_s )
{
    /* Collect I2C related data */
    log_data_in_s.log_data_in_s.dd_i2c_is_error_present_u8           = dd_data_out_s.p_i2c_error_out_s->is_present_b;
    log_data_in_s.log_data_in_s.dd_i2c_error_code_s16                = dd_data_out_s.p_i2c_error_out_s->error_info_vs[0U].error_e;
    log_data_in_s.log_data_in_s.dd_i2c_access_type_u8                = dd_data_out_s.p_i2c_error_out_s->error_info_vs[0U].access_type_e;
    log_data_in_s.log_data_in_s.dd_i2c_device_addr_u8                = dd_data_out_s.p_i2c_error_out_s->error_info_vs[0U].device_addr_u8;
    log_data_in_s.log_data_in_s.dd_i2c_register_addr_u8              = dd_data_out_s.p_i2c_error_out_s->error_info_vs[0U].register_addr_u8;


    if ( NULL != dd_data_out_s.p_icm_20600_data_out_s )
    {
        log_data_in_s.log_data_in_s.dd_icm_20600_accel_raw_data_x_s16    = dd_data_out_s.p_icm_20600_data_out_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_X];
        log_data_in_s.log_data_in_s.dd_icm_20600_accel_raw_data_y_s16    = dd_data_out_s.p_icm_20600_data_out_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Y];
        log_data_in_s.log_data_in_s.dd_icm_20600_accel_raw_data_z_s16    = dd_data_out_s.p_icm_20600_data_out_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Z];
        log_data_in_s.log_data_in_s.dd_icm_20600_gyro_raw_data_x_s16     = dd_data_out_s.p_icm_20600_data_out_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_X];
        log_data_in_s.log_data_in_s.dd_icm_20600_gyro_raw_data_y_s16     = dd_data_out_s.p_icm_20600_data_out_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Y];
        log_data_in_s.log_data_in_s.dd_icm_20600_gyro_raw_data_z_s16     = dd_data_out_s.p_icm_20600_data_out_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Z];
        log_data_in_s.log_data_in_s.dd_icm_20600_temperature_raw_u16     = dd_data_out_s.p_icm_20600_data_out_s->temperature_raw_u16;
        log_data_in_s.log_data_in_s.dd_icm_20600_temperature_deg_f32     = dd_data_out_s.p_icm_20600_data_out_s->temperature_deg_f32;
        log_data_in_s.log_data_in_s.dd_icm_20600_self_test_xa_u8         = dd_data_out_s.p_icm_20600_data_out_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XA];
        log_data_in_s.log_data_in_s.dd_icm_20600_self_test_ya_u8         = dd_data_out_s.p_icm_20600_data_out_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YA];
        log_data_in_s.log_data_in_s.dd_icm_20600_self_test_za_u8         = dd_data_out_s.p_icm_20600_data_out_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZA];
        log_data_in_s.log_data_in_s.dd_icm_20600_self_test_xg_u8         = dd_data_out_s.p_icm_20600_data_out_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XG];
        log_data_in_s.log_data_in_s.dd_icm_20600_self_test_yg_u8         = dd_data_out_s.p_icm_20600_data_out_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YG];
        log_data_in_s.log_data_in_s.dd_icm_20600_self_test_zg_u8         = dd_data_out_s.p_icm_20600_data_out_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZG];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_xa_f32     = dd_data_out_s.p_icm_20600_data_out_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XA];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_ya_f32     = dd_data_out_s.p_icm_20600_data_out_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YA];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_za_f32     = dd_data_out_s.p_icm_20600_data_out_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZA];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_xg_f32     = dd_data_out_s.p_icm_20600_data_out_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XG];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_yg_f32     = dd_data_out_s.p_icm_20600_data_out_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YG];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_zg_f32     = dd_data_out_s.p_icm_20600_data_out_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZG];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_dev_xa_f32 = dd_data_out_s.p_icm_20600_data_out_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XA];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_dev_ya_f32 = dd_data_out_s.p_icm_20600_data_out_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YA];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_dev_za_f32 = dd_data_out_s.p_icm_20600_data_out_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZA];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_dev_xg_f32 = dd_data_out_s.p_icm_20600_data_out_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XG];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_dev_yg_f32 = dd_data_out_s.p_icm_20600_data_out_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YG];
        log_data_in_s.log_data_in_s.dd_icm_20600_factory_trim_dev_zg_f32 = dd_data_out_s.p_icm_20600_data_out_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZG];
        log_data_in_s.log_data_in_s.dd_icm_20600_chip_id_u8              = dd_data_out_s.p_icm_20600_data_out_s->device_id_u8;
        log_data_in_s.log_data_in_s.dd_icm_20600_dev_state_u8            = (U8) dd_data_out_s.p_icm_20600_data_out_s->state_s;
        log_data_in_s.log_data_in_s.dd_icm_20600_is_calibrated_u8        = dd_data_out_s.p_icm_20600_data_out_s->is_calibrated_b;
        log_data_in_s.log_data_in_s.dd_icm_20600_self_test_passed_u8     = dd_data_out_s.p_icm_20600_data_out_s->self_test_passed_b;
    }
    else
    {
        /* Set all values to 0 */
    }


    if ( NULL != dd_data_out_s.p_adc_data_out_s )
    {
        log_data_in_s.log_data_in_s.dd_adc_raw_data_u16          = dd_data_out_s.p_adc_data_out_s->raw_sample_u16;
        log_data_in_s.log_data_in_s.dd_adc_previous_raw_data_u16 = dd_data_out_s.p_adc_data_out_s->previous_raw_sample_u16;
        log_data_in_s.log_data_in_s.dd_adc_voltage_u16           = dd_data_out_s.p_adc_data_out_s->voltage_u16;
        log_data_in_s.log_data_in_s.dd_adc_raw_level_f32         = dd_data_out_s.p_adc_data_out_s->raw_level_f32;
        // log_data_in_s.log_data_in_s.dd_adc_filtered_level_f32    = dd_data_out_s.p_adc_data_out_s->filtered_level_f32; -> Removed from data structure, filtering will take place in SENSE domain
    }
    else
    {
        /* Set all values to 0 */
    }


    if ( NULL != dd_data_out_s.p_ina_219_data_out_s )
    {
        log_data_in_s.log_data_in_s.dd_ina_219_shunt_voltage_mV_f32  = dd_data_out_s.p_ina_219_data_out_s->shunt_voltage_mV_f32;
        log_data_in_s.log_data_in_s.dd_ina_219_bus_voltage_V_f32     = dd_data_out_s.p_ina_219_data_out_s->bus_voltage_V_f32;
        log_data_in_s.log_data_in_s.dd_ina_219_power_mW_f32          = dd_data_out_s.p_ina_219_data_out_s->power_mW_f32;
        log_data_in_s.log_data_in_s.dd_ina_219_current_mA_f32        = dd_data_out_s.p_ina_219_data_out_s->current_mA_f32;
        log_data_in_s.log_data_in_s.dd_ina_219_shunt_voltage_raw_s16 = dd_data_out_s.p_ina_219_data_out_s->shunt_voltage_raw_s16;
        log_data_in_s.log_data_in_s.dd_ina_219_power_raw_u16         = dd_data_out_s.p_ina_219_data_out_s->power_raw_u16;
        log_data_in_s.log_data_in_s.dd_ina_219_current_raw_s16       = dd_data_out_s.p_ina_219_data_out_s->current_raw_s16;
        log_data_in_s.log_data_in_s.dd_ina_219_bus_voltage_raw_u16   = dd_data_out_s.p_ina_219_data_out_s->bus_voltage_data_s.voltage_raw_u16;
        //    log_data_in_s.log_data_in_s.dd_ina_219_shunt_voltage_range_u8 = (U8) dd_data_out_s.p_ina_219_data_out_s->shunt_voltage_range_;
        //    log_data_in_s.log_data_in_s.dd_ina_219_bus_voltage_range_u8   = (U8) dd_data_out_s.p_ina_219_data_out_s->bus_voltage_range_e;
    }
    else
    {
        /* Set all values to 0 */
    }


    if ( NULL != dd_data_out_s.p_tmp_102_data_out_s )
    {
        log_data_in_s.log_data_in_s.dd_tmp_102_temperature_raw_s16 = dd_data_out_s.p_tmp_102_data_out_s->temperature_raw_s16;
        log_data_in_s.log_data_in_s.dd_tmp_102_temperature_deg_f32 = dd_data_out_s.p_tmp_102_data_out_s->temperature_deg_f32;
    }
    else
    {
        /* Set all values to 0 */
    }


    if ( NULL != dd_data_out_s.p_max_30102_data_out_s )
    {
        // log_data_in_s.log_data_in_s.dd_max_30102_int_status_u8           = dd_data_out_s.p_max_30102_data_out_s->int_status_vb[];
        log_data_in_s.log_data_in_s.dd_max_30102_part_id_u8              = dd_data_out_s.p_max_30102_data_out_s->part_id_u8;
        log_data_in_s.log_data_in_s.dd_max_30102_rev_id_u8               = dd_data_out_s.p_max_30102_data_out_s->rev_id_u8;
        log_data_in_s.log_data_in_s.dd_max_30102_read_ptr_u8             = dd_data_out_s.p_max_30102_data_out_s->read_ptr_u8;
        log_data_in_s.log_data_in_s.dd_max_30102_write_ptr_u8            = dd_data_out_s.p_max_30102_data_out_s->write_ptr_u8;
        log_data_in_s.log_data_in_s.dd_max_30102_red_data_raw_u32        = dd_data_out_s.p_max_30102_data_out_s->red_data_raw_u32;
        log_data_in_s.log_data_in_s.dd_max_30102_ir_data_raw_u32         = dd_data_out_s.p_max_30102_data_out_s->ir_data_raw_u32;
        log_data_in_s.log_data_in_s.dd_max_30102_temperature_raw_int_u8  = dd_data_out_s.p_max_30102_data_out_s->temperature_raw_vu8[DD_MAX_30102_TEMP_COMP_INT];
        log_data_in_s.log_data_in_s.dd_max_30102_temperature_raw_frac_u8 = dd_data_out_s.p_max_30102_data_out_s->temperature_raw_vu8[DD_MAX_30102_TEMP_COMP_FRAC];
        log_data_in_s.log_data_in_s.dd_max_30102_temperature_f32         = dd_data_out_s.p_max_30102_data_out_s->temperature_f32;
        log_data_in_s.log_data_in_s.dd_max_30102_mode_u8                 = (U8) dd_data_out_s.p_max_30102_data_out_s->mode_e;
    }
    else
    {
        /* Set all values to 0 */
    }


#ifndef JTAG_DEBUG_SUPPORT_ENABLE
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_1_mode_u8               = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_1].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_1_duty_cycle_f32        = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_1].duty_cycle_f32;
#endif
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_2_mode_u8               = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_2].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_3_mode_u8               = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_3].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_4_mode_u8               = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_4].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_5_mode_u8               = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_5].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_6_mode_u8               = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_6].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_2_duty_cycle_f32        = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_2].duty_cycle_f32;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_3_duty_cycle_f32        = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_3].duty_cycle_f32;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_4_duty_cycle_f32        = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_4].duty_cycle_f32;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_5_duty_cycle_f32        = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_5].duty_cycle_f32;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_6_duty_cycle_f32        = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_6].duty_cycle_f32;
#ifndef JTAG_DEBUG_SUPPORT_ENABLE
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_7_mode_u8               = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_7].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_8_mode_u8               = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_8].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_9_mode_u8               = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_9].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_7_duty_cycle_f32        = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_7].duty_cycle_f32;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_8_duty_cycle_f32        = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_8].duty_cycle_f32;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_9_duty_cycle_f32        = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_9].duty_cycle_f32;
#endif
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_10_mode_u8              = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_10].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_11_mode_u8              = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_11].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_12_mode_u8              = (U8) dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_12].mode_e;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_10_duty_cycle_f32       = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_10].duty_cycle_f32;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_11_duty_cycle_f32       = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_11].duty_cycle_f32;
    log_data_in_s.log_data_in_s.dd_mcpwm_ch_12_duty_cycle_f32       = dd_data_in_s.mcpwm_data_in_s.pwm_cfg_s[DD_MCPWM_CHANNEL_12].duty_cycle_f32;

    if ( NULL != sense_data_out_s.p_ts_data_out_s )
    {
        /* SENSE */
        log_data_in_s.log_data_in_s.sense_ts_alpha_filtered_adc_level_f32      = sense_data_out_s.p_ts_data_out_s->alpha_filtered_adc_level_f32;
        log_data_in_s.log_data_in_s.sense_ts_alpha_beta_filtered_adc_level_f32 = sense_data_out_s.p_ts_data_out_s->alpha_beta_filtered_adc_level_f32;
        ;
        log_data_in_s.log_data_in_s.sense_ts_touch_confidence_f32     = sense_data_out_s.p_ts_data_out_s->touch_conf_s.confidence_f32;
        log_data_in_s.log_data_in_s.sense_ts_touch_confidence_max_f32 = sense_data_out_s.p_ts_data_out_s->touch_conf_s.confidence_max_f32;
    }
}
