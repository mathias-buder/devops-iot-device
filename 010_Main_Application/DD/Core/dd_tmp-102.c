/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_tmp_102.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "esp_log.h"

#include "dd_tmp-102.h"
#include "dd_i2c.h"
#include "dd_database.h"

#include "../../UTIL/UTIL.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE BOOLEAN dd_tmp_102_get_temperature_raw( U16* p_value_u16 );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
BOOLEAN dd_tmp_102_init( void )
{
    ESP_LOGI( DD_TMP_102_LOG_MSG_TAG, "Initializing ..." );

    ESP_LOGI( DD_TMP_102_LOG_MSG_TAG, "Done" );
    return TRUE;
}

void dd_tmp_102_main( void )
{
    /* Get raw temperature reading */
    dd_tmp_102_get_temperature_raw( &dd_tmp_102_data_s.temperature_raw_u16 );

    /* Convert raw temperature reading in °C */
    dd_tmp_102_data_s.temperature_deg_f32 = dd_tmp_102_data_s.temperature_raw_u16 * DD_TMP_102_TEMP_LSB;

    ESP_LOGD( DD_TMP_102_LOG_MSG_TAG, "TempRaw: %i [LSB], TempDeg: %0.4f °C", dd_tmp_102_data_s.temperature_raw_u16,
                                                                              dd_tmp_102_data_s.temperature_deg_f32 );
}

PRIVATE BOOLEAN dd_tmp_102_get_temperature_raw( U16* p_value_u16 )
{
    U16 register_u16 = 0U;

    if ( NULL != p_value_u16 )
    {
        if ( FALSE == dd_i2c_read_burst( DD_TMP_102_I2C_ADDR, DD_TMP_102_PTR_TEMP, (U8*) &register_u16, sizeof( U16 ) ) )
        {
            return FALSE;
        }

        /* TMP-102 is returning the 16-bit register content in MSByte first. Therefore, the byte order
         * of 16-bit variable register_u16 need to be swapped. Further more, the temperature data is
         * left-justified and need to be shifted to the right by 4-bit (in 12-bit mode) */
         *p_value_u16 = SWAP_BYTES_IN_WORD( register_u16 ) >> 4U;
    }
    else
    {
        assert( NULL != p_value_u16 );
        return FALSE;
    }

    return TRUE;
}
