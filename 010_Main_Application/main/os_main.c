/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

*********************************************************************/


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "../DD/DD.h"
#include "../SENSE/SENSE.h"
#include "../DLG/DLG.h"

#include <time.h>
#include <sys/time.h>


U32 global_time_u32;

struct tm tm = {
    .tm_yday = 2018 - 1900,
    .tm_mon  = 10,
    .tm_mday = 15,
    .tm_hour = 14,
    .tm_min  = 10,
    .tm_sec  = 10};

void app_main()
{

    time_t t = mktime( &tm );
    printf( "Setting time: %s\n", asctime( &tm ) );
    struct timeval now = {.tv_sec = t};
    settimeofday( &now, NULL );

    /* Print chip information */
    esp_chip_info_t  chip_info;
    TickType_t       xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency    = 10U;

    esp_chip_info( &chip_info );
    printf( "This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            ( chip_info.features & CHIP_FEATURE_BT ) ? "/BT" : "",
            ( chip_info.features & CHIP_FEATURE_BLE ) ? "/BLE" : "" );

    printf( "silicon revision %d, ", chip_info.revision );

    printf( "%dMB %s flash\n\n", spi_flash_get_chip_size() / ( 1024 * 1024 ),
            ( chip_info.features & CHIP_FEATURE_EMB_FLASH ) ? "embedded" : "external" );

    /* Initialize all device drivers */
    dd_init();

    dlg_init();


    while ( TRUE )
    {
        /* Schedule Device Driver (DD) */
        dd_main();

        dlg_main();

        /* Schedule every 100 ms */
        vTaskDelayUntil( &xLastWakeTime, xFrequency );

        global_time_u32 += 100U;
    }
}
