/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "DD.h"
#include "Core/dd_database.h"
#include "Core/dd_i2c.h"
#include "Core/dd_icm-20600.h"
#include "Core/dd_sd.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
FILE*   p_log_file;
BOOLEAN file_written_b = FALSE;
/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/


void dd_init(void)
{

    /* Initialize I2C basic device driver */
    dd_i2c_init();

    dd_sd_init();

    dd_sd_create_file( &p_log_file, "my_log_file.txt" );

    printf("dd_init : p_log_file: %p\n", p_log_file);


    /* Initialize ICM-2600 motion subsystem */
//    if( TRUE != dd_icm_20600_init() )
//    {
//        printf( "dd_icm_20600_init() failed with error: 0x%x\n", dd_i2c_get_error()->current_t );
//    }

}


void dd_main(void)
{
   // dd_icm_20600_main();

   U32 idx_32;

   printf("dd_main : p_log_file: %p\n", p_log_file);

   if ( FALSE == file_written_b )
   {
       if ( NULL  != p_log_file )
       {
           ESP_LOGI( DD_LOG_MSG_TAG, "Writing to file" );

           for ( idx_32 = 0U; idx_32 < 500U; ++idx_32 )
           {
               fprintf(p_log_file, " This is a Test This is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a Test\n");
               // ESP_LOGI( DD_LOG_MSG_TAG, "Printing line %i!\n", idx_32 );
           }

           if (0U == fclose( p_log_file ) )
           {
               ESP_LOGI( DD_LOG_MSG_TAG, "File closed" );
               file_written_b = TRUE;
           }
           else
           {
               ESP_LOGE( DD_LOG_MSG_TAG, "File couln't be closed" );
           }
       }
       else
       {
           ESP_LOGE( DD_LOG_MSG_TAG, "Error while trying to write to file" );
       }
   }
}
