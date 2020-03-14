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

#include "dd_sd.h"
#include "../Config/dd_sd_cfg.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
static const char *TAG = "example";


BOOLEAN dd_sd_init( void )
{
    ESP_LOGI( DD_SD_LOG_MSG_TAG, "Initializing SD card ..." );

    /* */
    esp_err_t           status_t;
    sdmmc_card_t*       p_sd_card_info_s;
    sdmmc_host_t        sd_host_desc_s   = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t sd_slot_config_s = SDSPI_SLOT_CONFIG_DEFAULT();
    sd_slot_config_s.gpio_miso           = DD_SD_MISO_PIN;
    sd_slot_config_s.gpio_mosi           = DD_SD_MOSI_PIN;
    sd_slot_config_s.gpio_sck            = DD_SD_CLK_PIN;
    sd_slot_config_s.gpio_cs             = DD_SD_CS_PIN;

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = FALSE,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
    // Please check its source code and implement error recovery when developing
    // production applications.
    status_t = esp_vfs_fat_sdmmc_mount("/sdcard", &sd_host_desc_s, &sd_slot_config_s, &mount_config, &p_sd_card_info_s);

    if ( ESP_OK != status_t )
    {
        if ( ESP_FAIL == status_t )
        {
            ESP_LOGE( TAG, "Failed to mount filesystem. If you want the card to be formatted, set format_if_mount_failed = true." );
        }
        else
        {
            ESP_LOGE( TAG, "Failed to initialize the card (%s). Make sure SD card lines have pull-up resistors in place.", esp_err_to_name( status_t ) );
        }

        /* Abort SD card initialization in case of mounting error */
        return FALSE;
    }

    ESP_LOGI( DD_SD_LOG_MSG_TAG, "SD card mounted successfully" );

    // Card has been initialized, print its properties
    sdmmc_card_print_info( stdout, p_sd_card_info_s );

    // Use POSIX and C standard library functions to work with files.
    // First create a file.
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen("/sdcard/hello.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return FALSE;
    }
    fprintf(f, "Hello %s!\n", p_sd_card_info_s->cid.name);
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // Check if destination file exists before renaming
    struct stat st;
    if (stat("/sdcard/foo.txt", &st) == 0) {
        // Delete it if it exists
        unlink("/sdcard/foo.txt");
    }

    // Rename original file
    ESP_LOGI(TAG, "Renaming file");
    if (rename("/sdcard/hello.txt", "/sdcard/foo.txt") != 0) {
        ESP_LOGE(TAG, "Rename failed");
        return FALSE;
    }

    // Open renamed file for reading
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/sdcard/foo.txt", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return FALSE;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);

    // All done, unmount partition and disable SDMMC or SPI peripheral
    esp_vfs_fat_sdmmc_unmount();
    ESP_LOGI(TAG, "Card unmounted");

   return TRUE;
}














