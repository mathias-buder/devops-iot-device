/*********************************************************************
    MODULE SPECIFICATION:

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dlg_log.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "../Config/dlg_log_cfg.h"
#include "dlg_log.h"
#include "dlg_database.h"
#include "dlg_types.h"
#include "../Interface/dlg_log_if.h"

#include "../../DD/DD.h"
#include "../../OS/OS.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
PRIVATE U32 file_count_u32 = 1U;
PRIVATE U32 data_chunk_cnt_u32;

#ifdef DLG_LOG_TEST_ENABLE
PRIVATE U32  log_test_mode_cnt_u32;
#endif

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE void    dlg_log_create_dd_i2c_data_frame( void );
PRIVATE void    dlg_log_create_dd_adc_data_frame( void );
PRIVATE void    dlg_log_create_dd_mcpwm_data_frame( void );
PRIVATE void    dlg_log_create_dd_icm_20600_data_frame( void );
PRIVATE void    dlg_log_create_dd_max_30102_data_frame( void );
PRIVATE void    dlg_log_create_dd_ina_219_data_frame( void );
PRIVATE void    dlg_log_create_sense_ts_data_frame( void );
PRIVATE BOOLEAN dlg_log_handle_file( U32 id_u32 );
/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

void dlg_log_init( void )
{
    ESP_LOGI( DLG_LOG_LOG_MSG_TAG, "Initializing..." );

    /* Get reference to all log data structures */
    dlg_log_get_data();

    /* Create initial log file */
    dlg_log_handle_file( file_count_u32 );

    /* Calculate number of cycles to be logged to each individual  file.
     * The maximum log file size shall be a multiple of sizeof( dlg_log_database_s ) and
     * within the interval of [ 1 >= data_chunk_per_file >= DLG_LOG_FILE_SIZE_IN_KBYTE ] */
    dlg_database_s.num_data_chunk_per_file_u32 = ( U32 )( ( DLG_LOG_FILE_SIZE_IN_KBYTE * 1000U ) / sizeof( dlg_log_database_s ) );

    if(    ( NULL != dlg_database_s.p_file_handle )
        && ( 0U   < dlg_database_s.num_data_chunk_per_file_u32 ) )
    {
        dlg_database_s.logging_enabled_b = TRUE;
    }

    ESP_LOGD( DLG_LOG_LOG_MSG_TAG, "Logging %s", ( dlg_database_s.logging_enabled_b == TRUE ) ? "enabled" : "disabled" );
    ESP_LOGD( DLG_LOG_LOG_MSG_TAG, "Database size %i Byte", sizeof( dlg_log_database_s ) );
    ESP_LOGD( DLG_LOG_LOG_MSG_TAG, "Data chunks per file: %i", dlg_database_s.num_data_chunk_per_file_u32 );
}


void dlg_log_main( void )
{
    if ( TRUE == dlg_database_s.logging_enabled_b )
    {
        if ( data_chunk_cnt_u32 >= dlg_database_s.num_data_chunk_per_file_u32 )
        {
            /* Create next log file */
            ++file_count_u32;

            if ( FALSE == dlg_log_handle_file( file_count_u32 ) )
            {
                /* Disable logging in case of an error during file creation */
                ESP_LOGE( DLG_LOG_LOG_MSG_TAG, "Error during creation" );
                dlg_database_s.logging_enabled_b = FALSE;
                return;
            }

            /* Reset chunk counter */
            data_chunk_cnt_u32 = 0U;
        }

        /* Acquire current values for all logging structure */
        dlg_log_create_dd_i2c_data_frame();
        dlg_log_create_dd_adc_data_frame();
        dlg_log_create_dd_mcpwm_data_frame();
        dlg_log_create_dd_icm_20600_data_frame();
        dlg_log_create_dd_max_30102_data_frame();
        dlg_log_create_dd_ina_219_data_frame();
        dlg_log_create_sense_ts_data_frame();

        /* Acquire current time stamp */
        dlg_log_database_s.dlg_time_stamp_f32 = os_time_stamp_ms_f32;

        /* Increment global message counter */
        ++dlg_log_database_s.dlg_global_msg_cnt_u8;

        /* Write entire logging structure into .sbf file */
        fwrite( &dlg_log_database_s, sizeof( dlg_log_database_s ), 1U, dlg_database_s.p_file_handle );

        ESP_LOGI( DLG_LOG_LOG_MSG_TAG, "Logging to file %s [%i Byte / %i Byte ] [%i / %i]",
                                       dlg_database_s.file_name_vc,
                                       sizeof( dlg_log_database_s ) * ( data_chunk_cnt_u32 + 1U ),
                                       sizeof( dlg_log_database_s ) * dlg_database_s.num_data_chunk_per_file_u32,
                                       data_chunk_cnt_u32 + 1U,
                                       dlg_database_s.num_data_chunk_per_file_u32 );

        /* Advance counter to write next chunk of data */
        ++data_chunk_cnt_u32;

#ifdef DLG_LOG_TEST_ENABLE
        ++log_test_mode_cnt_u32;
#endif

    }
}


PRIVATE BOOLEAN dlg_log_handle_file( U32 id_u32 )
{
    /* Close file in case it is currently open */
    if (    ( TRUE  == dd_sd_is_file_open() )
         && ( FALSE == dd_sd_close_file()   ) )
    {
        return FALSE;
    }

    /* Generate file name */
    snprintf( dlg_database_s.file_name_vc,
              DLG_LOG_MAX_FILE_NAME_LENGTH,
              "%s_%i%s", DLG_LOG_DEV_SERIAL_NO,
                         id_u32,
                         DLG_LOG_FILE_DOT_EXTENSION );

    ESP_LOGI( DLG_LOG_LOG_MSG_TAG, "Current log file: %s", dlg_database_s.file_name_vc );

    /* Create log file and store its handle */
    dlg_database_s.p_file_handle = dd_sd_create_binary_file( dlg_database_s.file_name_vc );

    /* Check for error during file creation */
    if ( NULL == dlg_database_s.p_file_handle )
    {
        return FALSE;
    }

    return TRUE;
}


PRIVATE void dlg_log_create_dd_i2c_data_frame( void )
{
#ifdef DLG_LOG_TEST_ENABLE
    dlg_log_database_s.dd_i2c_is_error_present_u8 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_i2c_error_code_s16      = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_i2c_access_type_u8      = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_i2c_device_addr_u8      = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_i2c_register_addr_u8    = log_test_mode_cnt_u32;
#else
    dlg_log_database_s.dd_i2c_is_error_present_u8   = (U8) p_dlg_dd_i2c_error_data_s->is_error_present_b;
    dlg_log_database_s.dd_i2c_error_code_s16        = (S16) p_dlg_dd_i2c_error_data_s->error_info_vs[0U].error_e;
    dlg_log_database_s.dd_i2c_access_type_u8        = (U8) p_dlg_dd_i2c_error_data_s->error_info_vs[0U].access_type_e;
    dlg_log_database_s.dd_i2c_device_addr_u8        = p_dlg_dd_i2c_error_data_s->error_info_vs[0U].device_addr_u8;
    dlg_log_database_s.dd_i2c_register_addr_u8      = p_dlg_dd_i2c_error_data_s->error_info_vs[0U].register_addr_u8;
#endif
}

PRIVATE void dlg_log_create_dd_adc_data_frame( void )
{
#ifdef DLG_LOG_TEST_ENABLE
    dlg_log_database_s.dd_adc_raw_data_u16          = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_adc_previous_raw_data_u16 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_adc_voltage_u16           = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_adc_raw_level_f32         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_adc_filtered_level_f32    = log_test_mode_cnt_u32;
#else
    dlg_log_database_s.dd_adc_raw_data_u16          = p_dlg_dd_adc_data_s->raw_sample_u16;
    dlg_log_database_s.dd_adc_previous_raw_data_u16 = p_dlg_dd_adc_data_s->previous_raw_sample_u16;
    dlg_log_database_s.dd_adc_voltage_u16           = p_dlg_dd_adc_data_s->voltage_u16;
    dlg_log_database_s.dd_adc_raw_level_f32         = p_dlg_dd_adc_data_s->raw_level_f32;
    dlg_log_database_s.dd_adc_filtered_level_f32    = p_dlg_dd_adc_data_s->filtered_level_f32;
#endif
}

PRIVATE void dlg_log_create_dd_icm_20600_data_frame( void )
{
#ifdef DLG_LOG_TEST_ENABLE
    /* DD_ICM_20600_raw_accel ( 0x14 )*/
    dlg_log_database_s.dd_icm_20600_accel_raw_data_x_s16    = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_accel_raw_data_y_s16    = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_accel_raw_data_z_s16    = log_test_mode_cnt_u32;

    /* DD_ICM_20600_raw_gyro ( 0x15 ) */
    dlg_log_database_s.dd_icm_20600_gyro_raw_data_x_s16     = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_gyro_raw_data_y_s16     = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_gyro_raw_data_z_s16     = log_test_mode_cnt_u32;

    /* DD_ICM_20600_temperature ( 0x16 )*/
    dlg_log_database_s.dd_icm_20600_temperature_raw_u16     = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_temperature_deg_f32     = log_test_mode_cnt_u32;

    /* DD_ICM_20600_selt_test ( 0x17 )*/
    dlg_log_database_s.dd_icm_20600_self_test_xa_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_self_test_ya_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_self_test_za_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_self_test_xg_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_self_test_yg_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_self_test_zg_u8         = log_test_mode_cnt_u32;

    /* DD_ICM_20600_factory_trim ( 0x18 )*/
    dlg_log_database_s.dd_icm_20600_factory_trim_xa_f32     = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_ya_f32     = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_za_f32     = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_xg_f32     = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_yg_f32     = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_zg_f32     = log_test_mode_cnt_u32;

    /* DD_ICM_20600_factory_trim_dev ( 0x19 )*/
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_xa_f32 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_ya_f32 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_za_f32 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_xg_f32 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_yg_f32 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_zg_f32 = log_test_mode_cnt_u32;

    /* DD_ICM_20600_general ( 0x1A) */
    dlg_log_database_s.dd_icm_20600_chip_id_u8              = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_dev_state_u8            = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_is_calibrated_u8        = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_icm_20600_self_test_passed_u8     = log_test_mode_cnt_u32;
#else
    /* DD_ICM_20600_raw_accel ( 0x14 )*/
    dlg_log_database_s.dd_icm_20600_accel_raw_data_x_s16    = p_dlg_dd_icm_20600_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_X];
    dlg_log_database_s.dd_icm_20600_accel_raw_data_y_s16    = p_dlg_dd_icm_20600_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Y];
    dlg_log_database_s.dd_icm_20600_accel_raw_data_z_s16    = p_dlg_dd_icm_20600_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Z];

    /* DD_ICM_20600_raw_gyro ( 0x15 ) */
    dlg_log_database_s.dd_icm_20600_gyro_raw_data_x_s16     = p_dlg_dd_icm_20600_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_X];
    dlg_log_database_s.dd_icm_20600_gyro_raw_data_y_s16     = p_dlg_dd_icm_20600_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Y];
    dlg_log_database_s.dd_icm_20600_gyro_raw_data_z_s16     = p_dlg_dd_icm_20600_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Z];

    /* DD_ICM_20600_temperature ( 0x16 )*/
    dlg_log_database_s.dd_icm_20600_temperature_raw_u16     = p_dlg_dd_icm_20600_data_s->temperature_raw_u16;
    dlg_log_database_s.dd_icm_20600_temperature_deg_f32     = p_dlg_dd_icm_20600_data_s->temperature_deg_f32;

    /* DD_ICM_20600_selt_test ( 0x17 )*/
    dlg_log_database_s.dd_icm_20600_self_test_xa_u8         = p_dlg_dd_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.dd_icm_20600_self_test_ya_u8         = p_dlg_dd_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.dd_icm_20600_self_test_za_u8         = p_dlg_dd_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.dd_icm_20600_self_test_xg_u8         = p_dlg_dd_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.dd_icm_20600_self_test_yg_u8         = p_dlg_dd_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.dd_icm_20600_self_test_zg_u8         = p_dlg_dd_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_factory_trim ( 0x18 )*/
    dlg_log_database_s.dd_icm_20600_factory_trim_xa_f32     = p_dlg_dd_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.dd_icm_20600_factory_trim_ya_f32     = p_dlg_dd_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.dd_icm_20600_factory_trim_za_f32     = p_dlg_dd_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.dd_icm_20600_factory_trim_xg_f32     = p_dlg_dd_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.dd_icm_20600_factory_trim_yg_f32     = p_dlg_dd_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.dd_icm_20600_factory_trim_zg_f32     = p_dlg_dd_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_factory_trim_dev ( 0x19 )*/
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_xa_f32 = p_dlg_dd_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_ya_f32 = p_dlg_dd_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_za_f32 = p_dlg_dd_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_xg_f32 = p_dlg_dd_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_yg_f32 = p_dlg_dd_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.dd_icm_20600_factory_trim_dev_zg_f32 = p_dlg_dd_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_general ( 0x1A) */
    dlg_log_database_s.dd_icm_20600_chip_id_u8              = p_dlg_dd_icm_20600_data_s->chip_id_u8;
    dlg_log_database_s.dd_icm_20600_dev_state_u8            = (U8) p_dlg_dd_icm_20600_data_s->dev_state_s.state_e;
    dlg_log_database_s.dd_icm_20600_is_calibrated_u8        = p_dlg_dd_icm_20600_data_s->is_calibrated_b;
    dlg_log_database_s.dd_icm_20600_self_test_passed_u8     = p_dlg_dd_icm_20600_data_s->self_test_passed_b;
#endif
}


PRIVATE void dlg_log_create_dd_max_30102_data_frame( void )
{
#ifdef DLG_LOG_TEST_ENABLE
    dlg_log_database_s.dd_max_30102_int_status_u8           = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_part_id_u8              = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_rev_id_u8               = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_read_ptr_u8             = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_write_ptr_u8            = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_red_data_raw_u32        = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_ir_data_raw_u32         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_temperature_raw_int_u8  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_temperature_raw_frac_u8 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_temperature_f32         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_max_30102_mode_u8                 = log_test_mode_cnt_u32;
#else
    // dlg_log_database_s.dd_max_30102_int_status_u8           = p_dlg_max_30102_data_s->int_status_vb[];
    dlg_log_database_s.dd_max_30102_part_id_u8              = p_dlg_dd_max_30102_data_s->part_id_u8;
    dlg_log_database_s.dd_max_30102_rev_id_u8               = p_dlg_dd_max_30102_data_s->rev_id_u8;
    dlg_log_database_s.dd_max_30102_read_ptr_u8             = p_dlg_dd_max_30102_data_s->read_ptr_u8;
    dlg_log_database_s.dd_max_30102_write_ptr_u8            = p_dlg_dd_max_30102_data_s->write_ptr_u8;
    dlg_log_database_s.dd_max_30102_red_data_raw_u32        = p_dlg_dd_max_30102_data_s->red_data_raw_u32;
    dlg_log_database_s.dd_max_30102_ir_data_raw_u32         = p_dlg_dd_max_30102_data_s->ir_data_raw_u32;
    dlg_log_database_s.dd_max_30102_temperature_raw_int_u8  = p_dlg_dd_max_30102_data_s->temperature_raw_vu8[DD_MAX_30102_TEMP_COMP_INT];
    dlg_log_database_s.dd_max_30102_temperature_raw_frac_u8 = p_dlg_dd_max_30102_data_s->temperature_raw_vu8[DD_MAX_30102_TEMP_COMP_FRAC];
    dlg_log_database_s.dd_max_30102_temperature_f32         = p_dlg_dd_max_30102_data_s->temperature_f32;
    dlg_log_database_s.dd_max_30102_mode_u8                 = (U8) p_dlg_dd_max_30102_data_s->mode_e;
#endif
}


PRIVATE void dlg_log_create_dd_ina_219_data_frame( void )
{
#ifdef DLG_LOG_TEST_ENABLE
    dlg_log_database_s.dd_ina_219_shunt_voltage_mV_f32   = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_ina_219_bus_voltage_V_f32      = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_ina_219_power_mW_f32           = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_ina_219_current_mA_f32         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_ina_219_shunt_voltage_raw_u16  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_ina_219_power_raw_u16          = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_ina_219_current_raw_u16        = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_ina_219_bus_voltage_raw_u16    = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_ina_219_shunt_voltage_range_u8 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_ina_219_bus_voltage_range_u8   = log_test_mode_cnt_u32;
#else
    dlg_log_database_s.dd_ina_219_shunt_voltage_mV_f32   = p_dlg_dd_ina_219_data_s->shunt_voltage_mV_f32;
    dlg_log_database_s.dd_ina_219_bus_voltage_V_f32      = p_dlg_dd_ina_219_data_s->bus_voltage_V_f32;
    dlg_log_database_s.dd_ina_219_power_mW_f32           = p_dlg_dd_ina_219_data_s->power_mW_f32;
    dlg_log_database_s.dd_ina_219_current_mA_f32         = p_dlg_dd_ina_219_data_s->current_mA_f32;
    dlg_log_database_s.dd_ina_219_shunt_voltage_raw_u16  = p_dlg_dd_ina_219_data_s->shunt_voltage_raw_u16;
    dlg_log_database_s.dd_ina_219_power_raw_u16          = p_dlg_dd_ina_219_data_s->power_raw_u16;
    dlg_log_database_s.dd_ina_219_current_raw_u16        = p_dlg_dd_ina_219_data_s->current_raw_u16;
    dlg_log_database_s.dd_ina_219_bus_voltage_raw_u16    = p_dlg_dd_ina_219_data_s->bus_voltage_data_s.voltage_raw_u16;
    dlg_log_database_s.dd_ina_219_shunt_voltage_range_u8 = (U8) p_dlg_dd_ina_219_data_s->shunt_voltage_range_e;
    dlg_log_database_s.dd_ina_219_bus_voltage_range_u8   = (U8) p_dlg_dd_ina_219_data_s->bus_voltage_range_e;
#endif
}


PRIVATE void dlg_log_create_sense_ts_data_frame( void )
{
#ifdef DLG_LOG_TEST_ENABLE
    dlg_log_database_s.sense_ts_alpha_filtered_adc_level_f32      = log_test_mode_cnt_u32;
    dlg_log_database_s.sense_ts_alpha_beta_filtered_adc_level_f32 = log_test_mode_cnt_u32;
    dlg_log_database_s.sense_ts_touch_confidence_f32              = log_test_mode_cnt_u32;
    dlg_log_database_s.sense_ts_touch_confidence_max_f32          = log_test_mode_cnt_u32;
#else
    dlg_log_database_s.sense_ts_alpha_filtered_adc_level_f32      = p_dlg_sense_ts_data_s->alpha_filtered_adc_level_f32;
    dlg_log_database_s.sense_ts_alpha_beta_filtered_adc_level_f32 = p_dlg_sense_ts_data_s->alpha_beta_filtered_adc_level_f32;
    dlg_log_database_s.sense_ts_touch_confidence_f32              = p_dlg_sense_ts_data_s->touch_conf_s.confidence_f32;
    dlg_log_database_s.sense_ts_touch_confidence_max_f32          = p_dlg_sense_ts_data_s->touch_conf_s.confidence_max_f32;
#endif
}


PRIVATE void dlg_log_create_dd_mcpwm_data_frame( void )
{
#ifdef DLG_LOG_TEST_ENABLE
    dlg_log_database_s.dd_mcpwm_ch_1_mode_u8         = log_test_mode_cnt_u32
    dlg_log_database_s.dd_mcpwm_ch_2_mode_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_3_mode_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_4_mode_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_5_mode_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_6_mode_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_7_mode_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_8_mode_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_9_mode_u8         = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_10_mode_u8        = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_11_mode_u8        = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_12_mode_u8        = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_1_duty_cycle_f32  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_2_duty_cycle_f32  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_3_duty_cycle_f32  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_4_duty_cycle_f32  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_5_duty_cycle_f32  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_6_duty_cycle_f32  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_7_duty_cycle_f32  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_8_duty_cycle_f32  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_9_duty_cycle_f32  = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_10_duty_cycle_f32 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_11_duty_cycle_f32 = log_test_mode_cnt_u32;
    dlg_log_database_s.dd_mcpwm_ch_12_duty_cycle_f32 = log_test_mode_cnt_u32;
#else
#ifndef JTAG_DEBUG_SUPPORT_ENABLE
    dlg_log_database_s.dd_mcpwm_ch_1_mode_u8         = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_1].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_1_duty_cycle_f32  = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_1].duty_cycle_f32;
#endif
    dlg_log_database_s.dd_mcpwm_ch_2_mode_u8         = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_2].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_3_mode_u8         = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_3].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_4_mode_u8         = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_4].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_5_mode_u8         = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_5].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_6_mode_u8         = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_6].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_2_duty_cycle_f32  = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_2].duty_cycle_f32;
    dlg_log_database_s.dd_mcpwm_ch_3_duty_cycle_f32  = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_3].duty_cycle_f32;
    dlg_log_database_s.dd_mcpwm_ch_4_duty_cycle_f32  = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_4].duty_cycle_f32;
    dlg_log_database_s.dd_mcpwm_ch_5_duty_cycle_f32  = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_5].duty_cycle_f32;
    dlg_log_database_s.dd_mcpwm_ch_6_duty_cycle_f32  = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_6].duty_cycle_f32;
#ifndef JTAG_DEBUG_SUPPORT_ENABLE
    dlg_log_database_s.dd_mcpwm_ch_7_mode_u8         = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_7].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_8_mode_u8         = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_8].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_9_mode_u8         = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_9].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_7_duty_cycle_f32  = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_7].duty_cycle_f32;
    dlg_log_database_s.dd_mcpwm_ch_8_duty_cycle_f32  = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_8].duty_cycle_f32;
    dlg_log_database_s.dd_mcpwm_ch_9_duty_cycle_f32  = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_9].duty_cycle_f32;
#endif
    dlg_log_database_s.dd_mcpwm_ch_10_mode_u8        = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_10].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_11_mode_u8        = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_11].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_12_mode_u8        = (U8) p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_12].mode_e;
    dlg_log_database_s.dd_mcpwm_ch_10_duty_cycle_f32 = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_10].duty_cycle_f32;
    dlg_log_database_s.dd_mcpwm_ch_11_duty_cycle_f32 = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_11].duty_cycle_f32;
    dlg_log_database_s.dd_mcpwm_ch_12_duty_cycle_f32 = p_dlg_dd_mcpwm_data_s->p_channel_s[DD_MCPWM_CHANNEL_12].duty_cycle_f32;
#endif
}
