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
#include <stdio.h>
#include <string.h>

/* ESP32 specific */
#include "esp_log.h"

/* Internal */
#include "DLG.h"
#include "Interface/dlg_log_interface.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
U8 count_u8 = 100;

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void dlg_init( void )
{
    /* Get pointers of all data structures to be logged */
    dlg_log_get_data();

    /* Create log file */
    dlg_log_create_file();
}

void dlg_main( void )
{

     dlg_log_write_data();
}
