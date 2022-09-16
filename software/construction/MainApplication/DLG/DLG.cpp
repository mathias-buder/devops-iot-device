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

        @file DLG.c
        @details Some detailed description

*********************************************************************/


/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "DLG.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void DLG_C::init( void )
{
    DLG_LOG_C::init();
}

void DLG_C::main( const LOG_DATA_IN_TYPE &dlg_data_in_s )
{
    DLG_LOG_C::main( dlg_data_in_s.log_data_in_s );
}
