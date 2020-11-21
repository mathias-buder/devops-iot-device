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
#include "dlg_log.h"
#include "../DD/dd_sd.h"

#include "esp_log.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
DLG_LOG_DATA DLG_LOG_C::data_s;
U32          DLG_LOG_C::file_count_u32 = 1U;
U32          DLG_LOG_C::data_chunk_cnt_u32;
U8           DLG_LOG_C::global_msg_cnt_u8;
F32          DLG_LOG_C::time_stamp_in_sec_f32;
U16          DLG_LOG_C::total_data_size_in_byte_u16;
#ifdef DLG_LOG_TEST_ENABLE
U32 DLG_LOG_C::test_mode_cnt_u32;
#endif

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void DLG_LOG_C::init( void )
{
    U32 size_per_min_in_byte_u32;

    ESP_LOGI( DLG_LOG_LOG_MSG_TAG, "Initializing..." );

    total_data_size_in_byte_u16 =  sizeof( time_stamp_in_sec_f32 ) + sizeof( DLG_LOG_DATA_IN_TYPE ) + sizeof( global_msg_cnt_u8 );

    /* Create initial log file */
    DLG_LOG_C::handle_file( file_count_u32 );

    /* Calculate number of cycles to be logged to each individual  file.
     * The maximum log file size shall be a multiple of sizeof( dlg_log_database_s ) and
     * within the interval of [ 1 (byte) >= data_chunk_per_file <= sizeof( dlg_log_database_s ) (byte) ] */
    if ( FALSE == DLG_LOG_USE_FILES_PER_MINUTE )
    {
        DLG_LOG_C::data_s.num_data_chunk_per_file_u32 = ( U32 )( ( DLG_LOG_FILE_SIZE_IN_KBYTE * 1000U ) / total_data_size_in_byte_u16 );
    }
    else
    {
        size_per_min_in_byte_u32                    = ( U32 )( ( 1000.0F / DLG_LOG_MAIN_CYCLE_TIME_MS ) * 60.0F ) * total_data_size_in_byte_u16;
        DLG_LOG_C::data_s.num_data_chunk_per_file_u32 = ( U32 )( ( size_per_min_in_byte_u32 / DLG_LOG_NUM_FILES_PER_MINUTE ) / total_data_size_in_byte_u16 );
    }

    if (    ( NULL != DLG_LOG_C::data_s.p_file_handle )
         && ( 0U < DLG_LOG_C::data_s.num_data_chunk_per_file_u32 ) )
    {
        DLG_LOG_C::data_s.logging_enabled_b = TRUE;

        /* Print logging properties */
        ESP_LOGD( DLG_LOG_LOG_MSG_TAG, "Logging enabled" );
        ESP_LOGD( DLG_LOG_LOG_MSG_TAG, "Database size %i Byte", total_data_size_in_byte_u16 );
        ESP_LOGD( DLG_LOG_LOG_MSG_TAG, "Data chunks per file: %i", DLG_LOG_C::data_s.num_data_chunk_per_file_u32 );
    }
    else
    {
        if ( 0U >= DLG_LOG_C::data_s.num_data_chunk_per_file_u32 )
        {
            ESP_LOGE( DLG_LOG_LOG_MSG_TAG, "Invalid value for num_data_chunk_per_file_u32: %i", DLG_LOG_C::data_s.num_data_chunk_per_file_u32 );
        }

        ESP_LOGD( DLG_LOG_LOG_MSG_TAG, "Logging disabled" );
    }
}

void DLG_LOG_C::main( DLG_LOG_DATA_IN_TYPE data_in_s )
{
    if ( TRUE == DLG_LOG_C::data_s.logging_enabled_b )
    {
        if ( data_chunk_cnt_u32 >= DLG_LOG_C::data_s.num_data_chunk_per_file_u32 )
        {
            /* Create next log file */
            ++file_count_u32;

            if ( FALSE == DLG_LOG_C::handle_file( file_count_u32 ) )
            {
                /* Disable logging in case of an error during file creation */
                ESP_LOGE( DLG_LOG_LOG_MSG_TAG, "Error during creation" );
                DLG_LOG_C::data_s.logging_enabled_b = FALSE;
                return;
            }

            /* Reset chunk counter */
            data_chunk_cnt_u32 = 0U;
        }

        /* Increment global message counter */
        ++global_msg_cnt_u8;

        /*  Write current value of time_stamp_in_sec_f32 into .sbf file */
        fwrite( &time_stamp_in_sec_f32, sizeof( time_stamp_in_sec_f32 ), 1U, DLG_LOG_C::data_s.p_file_handle );

        /* Write entire DLG_LOG_DATA_IN_TYPE structure into .sbf file */
        fwrite( &data_in_s, sizeof( DLG_LOG_DATA_IN_TYPE ), 1U, DLG_LOG_C::data_s.p_file_handle );

        /* Write current value of global_msg_cnt_u8 into .sbf file */
        fwrite( &global_msg_cnt_u8, sizeof( global_msg_cnt_u8 ), 1U, DLG_LOG_C::data_s.p_file_handle );

        ESP_LOGI( DLG_LOG_LOG_MSG_TAG, "Logging to file %s [%i Byte / %i Byte ] [%i / %i]",
                                       DLG_LOG_C::data_s.file_name_vc,
                                       total_data_size_in_byte_u16 * ( data_chunk_cnt_u32 + 1U ),
                                       total_data_size_in_byte_u16 * DLG_LOG_C::data_s.num_data_chunk_per_file_u32,
                                       data_chunk_cnt_u32 + 1U,
                                       DLG_LOG_C::data_s.num_data_chunk_per_file_u32 );

        /* Acquire current time stamp */
        //dlg_log_database_s.dlg_time_stamp_in_sec_f32 = os_time_stamp_in_sec_f32;
        time_stamp_in_sec_f32 += 0.1F;

        /* Advance counter to write next chunk of data */
        ++data_chunk_cnt_u32;

#ifdef DLG_LOG_TEST_ENABLE
        ++DLG_LOG_C::test_mode_cnt_u32;
#endif

    }
}


BOOLEAN DLG_LOG_C::handle_file( U32 id_u32 )
{
    /* Close file in case it is currently open */
    if (    ( TRUE  == DD_SD_C::is_file_open() )
         && ( FALSE == DD_SD_C::close_file()   ) )
    {
        return FALSE;
    }

    /* Generate file name */
    snprintf( DLG_LOG_C::data_s.file_name_vc,
              DLG_LOG_MAX_FILE_NAME_LENGTH,
              "%s_%i%s", DLG_LOG_DEV_SERIAL_NO,
                         id_u32,
                         DLG_LOG_FILE_DOT_EXTENSION );

    ESP_LOGI( DLG_LOG_LOG_MSG_TAG, "Current log file: %s", DLG_LOG_C::data_s.file_name_vc );

    /* Create log file and store its handle */
    DLG_LOG_C::data_s.p_file_handle = DD_SD_C::create_binary_file( DLG_LOG_C::data_s.file_name_vc );

    /* Check for error during file creation */
    if ( NULL == DLG_LOG_C::data_s.p_file_handle )
    {
        return FALSE;
    }

    return TRUE;
}
