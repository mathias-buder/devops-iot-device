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

        @file sense_ts_if.h
        @details Some detailed description

*********************************************************************/

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "sense_ts_if.h"

#include "../Core/sense_database.h"
#include "../DD/DD.h"

#include <string.h>

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/


/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/
void sense_ts_if_init( void )
{
    memset( &sense_ts_input_s, 0U, sizeof( sense_ts_input_s ) );
}


void sense_ts_if_main( void )
{

    sense_ts_input_s.raw_adc_sample_u16 = dd_adc_get_current_sample();

}


