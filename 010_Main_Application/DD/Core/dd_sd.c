/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_sd.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

#include "../Config/dd_sd_cfg.h"

#include "dd_database.h"
#include "dd_sd.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
BOOLEAN dd_sd_init( void )
{
    ESP_LOGI( DD_SD_LOG_MSG_TAG, "Initializing SD card ..." );

    esp_err_t           status_t;
    sdmmc_card_t*       p_sd_card_info_s;
    sdmmc_host_t        sd_host_desc_s   = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t sd_slot_config_s = SDSPI_SLOT_CONFIG_DEFAULT();
    sd_slot_config_s.gpio_miso           = DD_SD_MISO_PIN;
    sd_slot_config_s.gpio_mosi           = DD_SD_MOSI_PIN;
    sd_slot_config_s.gpio_sck            = DD_SD_CLK_PIN;
    sd_slot_config_s.gpio_cs             = DD_SD_CS_PIN;

    /* Options for mounting the filesystem.
     * If format_if_mount_failed is set to true, SD card will be partitioned and
     * formatted in case when mounting fails. */
    esp_vfs_fat_sdmmc_mount_config_t mount_config_s = {
        .format_if_mount_failed = DD_SD_FORMAT_ON_FAILED_MOUNT,
        .max_files              = DD_SD_MAX_OPEN_FILES,
        .allocation_unit_size   = DD_SD_ALLOC_UNIT_SIZE
    };

    /* Use settings defined above to initialize SD card and mount FAT filesystem.
     * Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
     * Please check its source code and implement error recovery when developing
     * production applications. */
    status_t = esp_vfs_fat_sdmmc_mount( DD_SD_MOUNT_POINT, &sd_host_desc_s, &sd_slot_config_s, &mount_config_s, &p_sd_card_info_s );

    if ( ESP_OK != status_t )
    {
        if ( ESP_FAIL == status_t )
        {
            ESP_LOGE( DD_SD_LOG_MSG_TAG, "Failed to mount filesystem. If you want the card to be formatted, set format_if_mount_failed = true." );
        }
        else
        {
            ESP_LOGE( DD_SD_LOG_MSG_TAG, "Failed to initialize the card (%s). Make sure SD card lines have pull-up resistors in place.", esp_err_to_name( status_t ) );
        }

        /* Abort SD card initialization in case of mounting error */
        dd_sd_data_s.is_fs_mounted_b = FALSE;
        return FALSE;
    }

    ESP_LOGI( DD_SD_LOG_MSG_TAG, "SD card mounted successfully" );
    dd_sd_data_s.is_fs_mounted_b = TRUE;

    /* Card has been initialized, print its properties */
    sdmmc_card_print_info( stdout, p_sd_card_info_s );

    return TRUE;
}

BOOLEAN dd_sd_deinit( void )
{
    esp_err_t status_t;

    ESP_LOGI( DD_SD_LOG_MSG_TAG, "Unmounting partition and disable SDMMC or SPI peripheral" );

    status_t = esp_vfs_fat_sdmmc_unmount();

    if( ESP_OK == status_t )
    {
        dd_sd_data_s.is_fs_mounted_b = FALSE;
        ESP_LOGI( DD_SD_LOG_MSG_TAG, "SDMMC unmounted" );
        return TRUE;
    }

    ESP_LOGE( DD_SD_LOG_MSG_TAG, "Failed to unmount SD card (%s)",  esp_err_to_name( status_t ) );
    dd_sd_data_s.is_fs_mounted_b = TRUE;

    return FALSE;
}

BOOLEAN dd_sd_open_file( char*           p_file_name_c,
                         const DD_SD_FILE_MODE file_mode_e,
                         BOOLEAN               overwrite_b )
{
    U8 file_path_length_u8 =  strlen( DD_SD_MOUNT_POINT )
                            + strlen( p_file_name_c )
                            + 2U; /* 2 Byte: "/" + "\0" */

    if (    ( NULL != p_file_name_c )
         && ( FALSE == dd_sd_data_s.file_s.is_open_b )
         && ( DD_SD_MAX_FILE_PATH_LENGTH > file_path_length_u8 ) )
    {
        /* Create full file path */
        strcpy( dd_sd_data_s.file_s.full_path_vc, DD_SD_MOUNT_POINT );
        strcat( dd_sd_data_s.file_s.full_path_vc, "/" );
        strcat( dd_sd_data_s.file_s.full_path_vc, p_file_name_c );

        ESP_LOGI( DD_SD_LOG_MSG_TAG, "Creating file %s [ %i / %i ]",
                                      dd_sd_data_s.file_s.full_path_vc,
                                      strlen( dd_sd_data_s.file_s.full_path_vc ) + 1U, /* + 1 Byte for "\0" */
                                      file_path_length_u8 );

        /* Check if file exists */
        if ( 0U == stat( dd_sd_data_s.file_s.full_path_vc, &dd_sd_data_s.file_s.attrib_s ) )
        {
            /* Check if file shall be overwritten*/
            if ( FALSE == overwrite_b )
            {
                ESP_LOGW( DD_SD_LOG_MSG_TAG, "File %s exists already and won't be overwritten", dd_sd_data_s.file_s.full_path_vc );
                return FALSE;
            }
            else
            {
                ESP_LOGW( DD_SD_LOG_MSG_TAG, "File %s exists already and will be overwritten", dd_sd_data_s.file_s.full_path_vc );
            }
        }

        /* Create file */
        switch ( file_mode_e )
        {
        case DD_SD_FILE_MODE_WRITE:
            dd_sd_data_s.file_s.p_handle = fopen( dd_sd_data_s.file_s.full_path_vc, "w" );
            break;

        case DD_SD_FILE_MODE_WRITE_BINARY:
            dd_sd_data_s.file_s.p_handle = fopen( dd_sd_data_s.file_s.full_path_vc, "wb" );
            break;

        default:
            assert( DD_SD_FILE_MODE_WRITE        == file_mode_e );
            assert( DD_SD_FILE_MODE_WRITE_BINARY == file_mode_e );
            break;
        }

        if ( NULL == dd_sd_data_s.file_s.p_handle )
        {
            ESP_LOGE( DD_SD_LOG_MSG_TAG, "Failed to open file %s for writing", dd_sd_data_s.file_s.full_path_vc );
            return FALSE;
        }
    }
    else
    {
        assert( NULL != p_file_name_c );
        assert( DD_SD_MAX_FILE_PATH_LENGTH >= file_path_length_u8 );
        return FALSE;
    }

    /* Read file attributes */
    if( 0U != stat( dd_sd_data_s.file_s.full_path_vc, &dd_sd_data_s.file_s.attrib_s ) )
    {
        ESP_LOGE( DD_SD_LOG_MSG_TAG, "Failed to read attributes of file %s", dd_sd_data_s.file_s.full_path_vc );
        return FALSE;
    }

    ESP_LOGI( DD_SD_LOG_MSG_TAG, "File %s created", dd_sd_data_s.file_s.full_path_vc );


    /* Set file open flag to TRUE when this point is reached */
    dd_sd_data_s.file_s.is_open_b = TRUE;

    return TRUE;
}
