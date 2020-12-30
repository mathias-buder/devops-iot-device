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
#include <stdio.h>
#include <string.h>

//#include "esp_log.h"

#include "SENSE.h"


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
#define SENSE_LOG_MSG_TAG       "SENSE"

SENSE_DATA_OUT_TYPE SENSE_C::sense_data_out_s;
SENSE_TS_C          sense_ts_A;

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void SENSE_C::init( void )
{
    SENSE_C::sense_data_out_s.p_ts_data_out_s = sense_ts_A.init();    /*!< Initialize Touch Sensor module */
}


SENSE_DATA_OUT_TYPE SENSE_C::main( SENSE_DATA_IN_TYPE& data_in_s )
{
    sense_ts_A.main( data_in_s.ts_data_in_s );  /*!< Schedule Touch Sensor module */

    return SENSE_C::sense_data_out_s;
}
