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
//#include "../SENSE/SENSE.h"
//#include "../VE/VE.h"

DD_DATA_OUT_TYPE dd_data_out_s;
DD_DATA_IN_TYPE  dd_data_in_s;

LOG_DATA_IN_TYPE log_data_in_s;

// SENSE_DATA_IN_TYPE

extern "C" void app_main()
{
    /* Get current OS tick count */
    TickType_t initial_tick_cnt_u32 = xTaskGetTickCount();

//    os_tm_init();   /* Initialize Global Time Module */
//    os_wifi_init(); /* Initialize and connect to wifi network */
    DD_C::init();      /* Initialize Device Driver Domain ( DD ) */
//    sense_init();   /* Initialize Sensor Processing Domain ( SENSE ) */
//    ve_init();      /* Initialize Vibration Engine Domain ( VE ) */
    DLG_C::init();     /* Initialize Data Logging Domain( DLG ) */

    /***********************************************
     *          Enter Infinite Main Loop           *
     ***********************************************/
    while ( TRUE )
    {
        /* Schedule every OS_MAIN_CYCLE_TIME_INCREMENT ms */
        vTaskDelayUntil( &initial_tick_cnt_u32, (TickType_t) OS_MAIN_CYCLE_TIME_INCREMENT );

        dd_data_out_s = DD_C::process_inputs();      /* Get-and-Process all inputs ( DD ) */

//       SENSE_DATA_IN_TYPE = map_dd_to_sense(DD_DATA_OUT_TYPE )
//       sense_main(SENSE_DATA_IN_TYPE);   /* Schedule Sensor Processing Domain ( SENSE ) */

//       VE_DATA_IN_TYPE = map_dd_to_sense_ve(DD_DATA_OUT_TYPE, SENSE_DATA_OUT_TYPE)
//       ve_main();      /* Schedule Vibration Engine Domain ( VE ) */


//      DD_DATA_IN_TYPE = collect_dd_data_in(SENSE_DATA_OUT_TYPE)
        DD_C::process_outputs( dd_data_in_s );

//      DLG_LOG_DATA_IN_TYPE = collect_dlg_data_in(DD_DATA_OUT_TYPE)
        DLG_C::main( log_data_in_s ); /* Schedule Data Logging Domain( DLG ) */

//      os_tm_update(); /* Update Global Time Module */
    }
}
