/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_sd_interface.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>

#include "../Core/dd_database.h"
#include "../Core/dd_sd.h"
#include "dd_sd_interface.h"



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
BOOLEAN dd_sd_create_file( const char* p_file_name_c )
{
    U8 file_path_length_u8 =   strlen( DD_SD_MOUNT_POINT )
                             + strlen( p_file_name_c )
                             + 2U; /* 2 Byte: "/" + "\0" */

    if (    ( NULL != p_file_name_c )
         && ( DD_SD_MAX_FILE_PATH_LENGTH >= file_path_length_u8 ) )
    {
        /* Create full file path */
        strcpy( dd_sd_data_s.file_path_vc, DD_SD_MOUNT_POINT );
        strcat( dd_sd_data_s.file_path_vc, "/" );
        strcat( dd_sd_data_s.file_path_vc, p_file_name_c );

        ESP_LOGI( DD_SD_LOG_MSG_TAG, "Creating file %s [ %i / %i ]",
                  dd_sd_data_s.file_path_vc,
                  strlen( dd_sd_data_s.file_path_vc ) + 1U, /* + 1 Byte for "\0" */
                  file_path_length_u8 );

        /* Create file */
        dd_sd_data_s.p_file =  fopen( dd_sd_data_s.file_path_vc, "w" );

        if ( NULL == dd_sd_data_s.p_file )
        {
            ESP_LOGE( DD_SD_LOG_MSG_TAG, "Failed to open file %s for writing", dd_sd_data_s.file_path_vc );
            return FALSE;
        }
    }
    else
    {
        ESP_LOGE( DD_SD_LOG_MSG_TAG, "Pointer to p_file_name_c is NULL" );

        assert( NULL != p_file_name_c );
        assert( DD_SD_MAX_FILE_PATH_LENGTH >= file_path_length_u8 );

        return FALSE;
    }

    ESP_LOGI( DD_SD_LOG_MSG_TAG, "File %s created", dd_sd_data_s.file_path_vc );

    return TRUE;
}
