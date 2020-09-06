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

        @file sense_ts.h
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/



#include <stdio.h>
#include <math.h>

#include "esp_log.h"

#include "sense_ts.h"
#include "../Config/sense_ts_cfg.h"
#include "sense_database.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/



/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
BOOLEAN sense_ts_init( void )
{
    return TRUE;
}


void sense_ts_main( void )
{

    ESP_LOGD( SENSE_TS_LOG_MSG_TAG, "Current ADC sample: %i", sense_ts_input_s.raw_adc_sample_u16 );
}
