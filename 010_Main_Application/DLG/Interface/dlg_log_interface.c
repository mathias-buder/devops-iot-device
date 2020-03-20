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

        @file dlg_log_interface.c
        @details Some detailed description

*********************************************************************/

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <stdio.h>
#include <string.h>
#include "esp_log.h"

#include "dlg_log_interface.h"
#include "../Core/dlg_database.h"
#include "../Core/dlg_log.h"

#include "../DD/DD.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
FILE* p_dlg_log_file;

/*************************************************************/
/*      LOCAL VARIABLES                                      */
/*************************************************************/
U8      cnt_u8         = 100U;
BOOLEAN my_file_written_b = FALSE;
/*************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                        */
/*************************************************************/

/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/
void dlg_log_create_file( void )
{
    p_dlg_log_file = dd_sd_create_binary_file( "my_log_file.sbf" ); /* Format: .sbf: sewela-binary-file */
}

void dlg_log_get_data( void )
{
    /* Acquire ICM-20600 database pointer */
    p_dlg_icm_20600_data_s = dd_icm_20600_get_database();
}

void dlg_log_write_data( void )
{
    if ( FALSE == my_file_written_b )
    {
        if ( cnt_u8 > 0 )
        {
            dlg_log_create_icm_20600_data_frame();

            fwrite( &dlg_log_database_s, sizeof( DLG_LOG_DATA ), sizeof( dlg_log_database_s ), p_dlg_log_file );

            --cnt_u8;

            ESP_LOGI( "DLG", "Logging %i", cnt_u8 );
        }
        else
        {
            dd_sd_close_file();
            my_file_written_b = TRUE;
        }
    }
}