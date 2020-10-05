/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_ina_219.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "esp_log.h"

#include "dd_ina-219.h"
#include "dd_i2c.h"
#include "dd_database.h"



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
BOOLEAN dd_ina_219_init( void )
{

    ESP_LOGI( DD_INA_219_LOG_MSG_TAG, "Initializing ..." );

//    if (    ( FALSE == dd_max_30102_soft_reset() )
//
//         /* Read device id and chip revision */
//         || ( FALSE == dd_max_30102_get_part_id( &dd_max_30102_data_s.part_id_u8 ) )
//         || ( FALSE == dd_max_30102_get_rev_id( &dd_max_30102_data_s.rev_id_u8 ) )
//
//
//         )
//    {
//        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "Initializing failed" );
//        return FALSE;
//    }

    ESP_LOGI( DD_INA_219_LOG_MSG_TAG, "Done" );
    return TRUE;
}

void dd_ina_219_main( void )
{
//    /* Clear data from previous cycle */
//    memset( &dd_max_30102_data_s.temperature_raw_vu8[0U], 0U, sizeof( dd_max_30102_data_s.temperature_raw_vu8 ) );
//    dd_max_30102_data_s.temperature_f32  = 0.0F;
//    dd_max_30102_data_s.red_data_raw_u32 = 0U;
//    dd_max_30102_data_s.ir_data_raw_u32  = 0U;
//
//    /* Read device temperature */
//    dd_max_30102_get_temperature( &dd_max_30102_data_s );
//
//    /* Read all interrupt flags */
//    dd_max_30102_get_int_status( dd_max_30102_data_s.int_status_vb );
//
//    /* Read new data from FIFO */
//    dd_max_30102_get_fifo_data( &dd_max_30102_data_s );
//
//    ESP_LOGD( DD_MAX_30105_LOG_MSG_TAG, "Part-Id: %i @ Rev: %i, Temp: %0.4f, Prox-Int: %i", dd_max_30102_data_s.part_id_u8,
//                                                                                            dd_max_30102_data_s.rev_id_u8,
//                                                                                            dd_max_30102_data_s.temperature_f32,
//                                                                                            dd_max_30102_data_s.int_status_vb[DD_MAX_30102_INT_TYPE_PROX_INT] );
}


