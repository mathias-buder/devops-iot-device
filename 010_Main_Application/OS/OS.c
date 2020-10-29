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
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "os_time.h"

#include "../DD/DD.h"
#include "../SENSE/SENSE.h"
#include "../VE/VE.h"
#include "../DLG/DLG.h"

void app_main()
{
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

    os_time_init(); /* Initialize Global Time Module */
    dd_init();      /* Initialize Device Driver Domain ( DD ) */
    sense_init();   /* Initialize Sensor Processing Domain ( SENSE ) */
    ve_init();      /* Initialize Vibration Engine Domain ( VE ) */
    dlg_init();     /* Initialize Data Logging Domain( DLG ) */

    /***********************************************
     ********** Enter Infinite Main Loop ***********
     ***********************************************/
    while ( TRUE )
    {
        /* Schedule every 100 ms */
        vTaskDelayUntil( &xLastWakeTime, xFrequency );

        dd_main();        /* Schedule Device Driver Domain ( DD ) */
        sense_main();     /* Schedule Sensor Processing Domain ( SENSE ) */
        ve_main();        /* Schedule Vibration Engine Domain ( VE ) */
        dlg_main();       /* Schedule Data Logging Domain( DLG ) */
        os_time_update(); /* Update Global Time Module */
    }
}
