/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_sd_if.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_sd_if.h"

#include <stdio.h>
#include <string.h>

#include "../Core/dd_database.h"
#include "../Core/dd_sd.h"


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
FILE* dd_sd_create_file( char* p_file_name_c )
{
    /* NULL pointer check of p_file_name_c is handled within
     * function dd_sd_open_file() */
    if ( TRUE == dd_sd_open_file( p_file_name_c, DD_SD_FILE_MODE_WRITE, TRUE ) )
    {
        return dd_sd_data_s.file_s.p_handle;
    }

    return NULL;
}

FILE* dd_sd_create_binary_file( char* p_file_name_c )
{
    /* NULL pointer check of p_file_name_c is handled within
     * function dd_sd_open_file() */
    if ( TRUE == dd_sd_open_file( p_file_name_c, DD_SD_FILE_MODE_WRITE_BINARY, TRUE ) )
    {
        return dd_sd_data_s.file_s.p_handle;
    }

    return NULL;
}

BOOLEAN dd_sd_close_file( void )
{
    if ( 0U == fclose( dd_sd_data_s.file_s.p_handle ) )
    {
        ESP_LOGI( DD_SD_LOG_MSG_TAG, "File closed" );
        dd_sd_data_s.file_s.p_handle  = NULL;
        dd_sd_data_s.file_s.is_open_b = FALSE;
        return TRUE;
    }

    ESP_LOGE( DD_SD_LOG_MSG_TAG, "File couln't be closed" );
    return FALSE;
}


BOOLEAN dd_sd_is_file_open( void )
{
    return dd_sd_data_s.file_s.is_open_b;
}



