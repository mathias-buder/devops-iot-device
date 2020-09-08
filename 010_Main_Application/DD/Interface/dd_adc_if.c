/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_adc_if.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_adc_if.h"

#include <stdio.h>
#include <string.h>

#include "../Core/dd_database.h"
#include "../Core/dd_adc.h"

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
DD_ADC_DATA* dd_adc_get_database( void )
{
    return &dd_adc_data_s;
}

U16 dd_adc_get_raw_sample( void )
{
    return dd_adc_data_s.raw_sample_u16;
}

F32 dd_adc_get_raw_level( void )
{
    return dd_adc_data_s.raw_level_f32;
}
