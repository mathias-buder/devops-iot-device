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


F32 global_time_f32;

struct tm timeinfo;

void app_main()
{

    time_t    now;
    char      strftime_buf[64];

    timeinfo.tm_sec   = 0U;    /**< tm_sec — the number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds. */
    timeinfo.tm_min   = 10U;   /**< tm_min — the number of minutes after the hour, in the range 0 to 59. */
    timeinfo.tm_hour  = 10U;   /**< tm_hour — the number of hours past midnight, in the range 0 to 23. */
    timeinfo.tm_mday  = 24U;   /**< tm_mday — the day of the month, in the range 1 to 31. */
    timeinfo.tm_mon   = 3U;    /**< tm_mon — the number of months since January, in the range 0 to 11. */
    timeinfo.tm_year  = 2020;  /**< tm_year — the number of years since 1900. */
    timeinfo.tm_wday  = 2U;    /**< tm_wday — the number of days since Sunday, in the range 0 to 6. */
    timeinfo.tm_yday  = 83U;   /**< tm_yday — the number of days since January 1, in the range 0 to 365. */
    timeinfo.tm_isdst = FALSE; /**< tm_isdst — a flag that indicates whether daylight saving time is in effect at the time described. The value is positive if daylight saving time is in effect, zero if it is not, and negative if the information is not available. */

    // Set timezone to Berlin Standard Time
    setenv( "TZ", "CET-1CEST,M3.5.0,M10.5.0/3", TRUE );
    tzset();


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

        global_time_f32 += 0.1F;

        time( &now );
        localtime_r( &now, &timeinfo );
        strftime( strftime_buf, sizeof( strftime_buf ), "%c", &timeinfo );
        ESP_LOGI( "OS", "The current date/time in Berlin is: %s", strftime_buf );
    }
}
