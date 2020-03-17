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
BOOLEAN file_written_b = FALSE;
U32     idx_u32         = 50000U;
U32     time_in_ms_u32;
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

    dd_sd_create_file( "my_new_log_file.csv" );

    printf("dd_init : p_log_file: %p\n", dd_sd_data_s.p_file);

    /* Initialize ICM-2600 motion subsystem */
    if( FALSE == dd_icm_20600_init() )
    {
        ESP_LOGE( DD_LOG_MSG_TAG, "dd_icm_20600_init() failed with error: 0x%x\n", dd_i2c_get_error()->current_t );
    }
}

void dd_main(void)
{
  // U32 idx_32;

   // printf("dd_main : p_log_file: %p\n", dd_sd_data_s.p_file);

    if (    ( NULL != dd_sd_data_s.p_file )
         && ( FALSE == file_written_b     ) )
    {
        dd_icm_20600_main();

       if ( idx_u32 > 0 )
       {
           fprintf( dd_sd_data_s.p_file, "%i, %i, %i, %i, %0.3f\n", time_in_ms_u32,
                                                                    dd_icm_20600_data_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_X],
                                                                    dd_icm_20600_data_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_Y],
                                                                    dd_icm_20600_data_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_Z],
                                                                    dd_icm_20600_data_s.temperature_deg_f32 );
           --idx_u32;

           ESP_LOGI( DD_LOG_MSG_TAG, "Logging data to file %s @ time %i ms [step: %i]", dd_sd_data_s.file_path_vc, time_in_ms_u32, idx_u32 );
       }
       else
       {
            if ( 0U == fclose( dd_sd_data_s.p_file ) )
            {
              ESP_LOGI( DD_LOG_MSG_TAG, "File closed" );
              file_written_b = TRUE;
            }
            else
            {
              ESP_LOGE( DD_LOG_MSG_TAG, "File couln't be closed" );
            }
       }
    }

    time_in_ms_u32 += 100U;




//   if ( FALSE == file_written_b )
//   {
//       if ( NULL != dd_sd_data_s.p_file )
//       {
//           ESP_LOGI( DD_LOG_MSG_TAG, "Writing to file" );
//
//           for ( idx_32 = 0U; idx_32 < 500U; ++idx_32 )
//           {
//               fprintf( dd_sd_data_s.p_file, " This is a Test This is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a TestThis is a Test\n" );
//               // ESP_LOGI( DD_LOG_MSG_TAG, "Printing line %i!\n", idx_32 );
//           }
//
//           if ( 0U == fclose( dd_sd_data_s.p_file ) )
//           {
//               ESP_LOGI( DD_LOG_MSG_TAG, "File closed" );
//               file_written_b = TRUE;
//           }
//           else
//           {
//               ESP_LOGE( DD_LOG_MSG_TAG, "File couln't be closed" );
//           }
//       }
//       else
//       {
//           ESP_LOGE( DD_LOG_MSG_TAG, "Error while trying to write to file" );
//       }
//   }
}
