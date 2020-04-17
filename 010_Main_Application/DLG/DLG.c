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
/* Standard libraries */
#include <Interface/dlg_log_if.h>
#include <stdio.h>
#include <string.h>

/* ESP32 specific */
#include "esp_log.h"

/* Internal */
#include "DLG.h"
#include "Core/dlg_log.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void dlg_init( void )
{
    dlg_log_init();
}

void dlg_main( void )
{
     dlg_log_main();
}
