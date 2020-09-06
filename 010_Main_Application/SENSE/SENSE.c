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

        @file SENSE.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/

/* Standard libraries */
#include <stdio.h>
#include <string.h>

/* ESP32 specific */
#include "esp_log.h"

/* Internal */
#include "SENSE.h"
#include "Core/sense_database.h"
#include "Core/sense_hr.h"
#include "Core/sense_mtn.h"
#include "Core/sense_spo2.h"
#include "Core/sense_ts.h"


void sense_init(void)
{
    sense_ts_init();
}



void sense_main(void)
{
    sense_ts_main();
}
