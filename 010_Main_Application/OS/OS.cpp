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
#include "../SENSE/SENSE.h"
#include "../VE/VE.h"
#include "../DLG/DLG.h"

void app_main()
{
    /* Get current OS tick count */
    TickType_t initial_tick_cnt_u32 = xTaskGetTickCount();

    os_tm_init();   /* Initialize Global Time Module */
    os_wifi_init(); /* Initialize and connect to wifi network */
    dd_init();      /* Initialize Device Driver Domain ( DD ) */
    sense_init();   /* Initialize Sensor Processing Domain ( SENSE ) */
    ve_init();      /* Initialize Vibration Engine Domain ( VE ) */
    dlg_init();     /* Initialize Data Logging Domain( DLG ) */

    /***********************************************
     ********** Enter Infinite Main Loop ***********
     ***********************************************/
    while ( TRUE )
    {
        /* Schedule every OS_MAIN_CYCLE_TIME_INCREMENT ms */
        vTaskDelayUntil( &initial_tick_cnt_u32, (TickType_t) OS_MAIN_CYCLE_TIME_INCREMENT );

        dd_main();      /* Schedule Device Driver Domain ( DD ) */
        sense_main();   /* Schedule Sensor Processing Domain ( SENSE ) */
        ve_main();      /* Schedule Vibration Engine Domain ( VE ) */
        dlg_main();     /* Schedule Data Logging Domain( DLG ) */
        os_tm_update(); /* Update Global Time Module */
    }
}
