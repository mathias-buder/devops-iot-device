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
#include "../../OS/os_time.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
U32 file_count_u32 = 1U;
U32 data_chunk_cnt_u32;

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE void    dlg_log_create_i2c_data_frame( void );
PRIVATE void    dlg_log_create_icm_20600_data_frame( void );
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
    ESP_LOGD( DLG_LOG_LOG_MSG_TAG, "Database size %i Bytes", sizeof( dlg_log_database_s ) );
    ESP_LOGD( DLG_LOG_LOG_MSG_TAG, "Data chunks per file: %i", dlg_database_s.num_data_chunk_per_file_u32 );
}

void dlg_log_main( void )
{
    if ( TRUE == dlg_database_s.logging_enabled_b )
    {
        if ( data_chunk_cnt_u32 > dlg_database_s.num_data_chunk_per_file_u32 )
        {
            /* Create next log file */
            ++file_count_u32;
            dlg_log_handle_file( file_count_u32 );

            /* Reset chunk counter */
            data_chunk_cnt_u32 = 0U;
        }

        /* Acquire current values for all logging structure */
        dlg_log_create_i2c_data_frame();
        dlg_log_create_icm_20600_data_frame();

        /* Acquire current time stamp */
        dlg_log_database_s.time_stamp_f32 = os_time_stamp_ms_f32;

        /* Increment global message counter */
        ++dlg_log_database_s.global_msg_cnt_u8;

        /* Write entire logging structure into .sbf file */
        fwrite( &dlg_log_database_s, sizeof( dlg_log_database_s ), 1U, dlg_database_s.p_file_handle );

        ESP_LOGI( DLG_LOG_LOG_MSG_TAG, "Logging to file %s [%i Bytes / %i Bytes ] [%i / %i]",
                                       dlg_database_s.file_name_vc,
                                       sizeof( dlg_log_database_s ) * ( data_chunk_cnt_u32 + 1U ),
                                       sizeof( dlg_log_database_s ) * dlg_database_s.num_data_chunk_per_file_u32,
                                       data_chunk_cnt_u32,
                                       dlg_database_s.num_data_chunk_per_file_u32 );

        ++data_chunk_cnt_u32;
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



PRIVATE void dlg_log_create_i2c_data_frame( void )
{
    dlg_log_database_s.i2c_data_s.is_error_present_b = (U8) p_dlg_i2c_error_data_s->is_error_present_b;
    dlg_log_database_s.i2c_data_s.error_code_s16     = (S16) p_dlg_i2c_error_data_s->error_info_vs[0U].error_e;
    dlg_log_database_s.i2c_data_s.access_type_u8     = (U8) p_dlg_i2c_error_data_s->error_info_vs[0U].access_type_e;
    dlg_log_database_s.i2c_data_s.device_addr_u8     = p_dlg_i2c_error_data_s->error_info_vs[0U].device_addr_u8;
    dlg_log_database_s.i2c_data_s.register_addr_u8   = p_dlg_i2c_error_data_s->error_info_vs[0U].register_addr_u8;
}

PRIVATE void dlg_log_create_icm_20600_data_frame( void )
{
    /* DD_ICM_20600_raw_accel ( 0x14 )*/
    dlg_log_database_s.icm_20600_data_s.accel_raw_data_x_s16       = p_dlg_icm_20600_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_X];
    dlg_log_database_s.icm_20600_data_s.accel_raw_data_y_s16       = p_dlg_icm_20600_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Y];
    dlg_log_database_s.icm_20600_data_s.accel_raw_data_z_s16       = p_dlg_icm_20600_data_s->accel_data_raw_s16[DD_ICM_20600_ACCEL_Z];

    /* DD_ICM_20600_raw_gyro ( 0x15 ) */
    dlg_log_database_s.icm_20600_data_s.gyro_raw_data_x_s16        = p_dlg_icm_20600_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_X];
    dlg_log_database_s.icm_20600_data_s.gyro_raw_data_y_s16        = p_dlg_icm_20600_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Y];
    dlg_log_database_s.icm_20600_data_s.gyro_raw_data_z_s16        = p_dlg_icm_20600_data_s->gyro_data_raw_s16[DD_ICM_20600_GYRO_Z];

    /* DD_ICM_20600_temperature ( 0x16 )*/
    dlg_log_database_s.icm_20600_data_s.temperature_raw_u16        = p_dlg_icm_20600_data_s->temperature_raw_u16;
    dlg_log_database_s.icm_20600_data_s.temperature_deg_f32        = p_dlg_icm_20600_data_s->temperature_deg_f32;

    /* DD_ICM_20600_selt_test ( 0x17 )*/
    dlg_log_database_s.icm_20600_data_s.self_test_xa_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.icm_20600_data_s.self_test_ya_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.icm_20600_data_s.self_test_za_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.icm_20600_data_s.self_test_xg_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.icm_20600_data_s.self_test_yg_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.icm_20600_data_s.self_test_zg_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_factory_trim ( 0x18 )*/
    dlg_log_database_s.icm_20600_data_s.factory_trim_xa_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_ya_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_za_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_xg_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.icm_20600_data_s.factory_trim_yg_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.icm_20600_data_s.factory_trim_zg_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_factory_trim_dev ( 0x19 )*/
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_xa_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_ya_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_za_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_xg_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_yg_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_zg_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_general ( 0x1A) */
    dlg_log_database_s.icm_20600_data_s.chip_id_u8                 = p_dlg_icm_20600_data_s->chip_id_u8;
    dlg_log_database_s.icm_20600_data_s.dev_state_u8               = (U8) p_dlg_icm_20600_data_s->dev_state_s.state_e;
    dlg_log_database_s.icm_20600_data_s.is_calibrated_u8           = p_dlg_icm_20600_data_s->is_calibrated_b;
    dlg_log_database_s.icm_20600_data_s.self_test_passed_u8        = p_dlg_icm_20600_data_s->self_test_passed_b;
}




