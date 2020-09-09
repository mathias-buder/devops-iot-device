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

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/


/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/
SENSE_TS_DATA * sense_ts_get_database( void )
{
    return &sense_ts_data_s;
}
