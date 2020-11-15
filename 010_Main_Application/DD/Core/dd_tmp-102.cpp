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
PRIVATE BOOLEAN dd_tmp_102_get_temperature_raw( S16* p_value_s16 );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
BOOLEAN dd_tmp_102_init( void )
{
    ESP_LOGI( DD_TMP_102_LOG_MSG_TAG, "Initializing ..." );

    U16 config_register_u16 =   DD_TMP_102_CONFIG_OS_DISABLE
                              | DD_TMP_102_CONFIG_F_1FAULT
                              | DD_TMP_102_CONFIG_POL_LOW
                              | DD_TMP_102_CONFIG_TM_COMP
                              | DD_TMP_102_CONFIG_SD_CC
                              | DD_TMP_102_CONFIG_CR_8HZ
                              | DD_TMP_102_CONFIG_EM_12BIT;

    /* Update configuration Registers */
    if ( FALSE == dd_i2c_write_burst( DD_TMP_102_I2C_ADDR, DD_TMP_102_CONFIG, ( (U8*) &config_register_u16 ), sizeof( config_register_u16 ) ) )
    {
        ESP_LOGE( DD_TMP_102_LOG_MSG_TAG, "Couldn't update configuration registers" );
        return FALSE;
    }

    ESP_LOGI( DD_TMP_102_LOG_MSG_TAG, "Done" );
    return TRUE;
}


void dd_tmp_102_main( void )
{
    /* Get raw temperature reading */
    dd_tmp_102_get_temperature_raw( &dd_tmp_102_data_s.temperature_raw_s16 );

    /* Convert raw temperature reading to °C */
    dd_tmp_102_data_s.temperature_deg_f32 = dd_tmp_102_data_s.temperature_raw_s16 * DD_TMP_102_TEMP_LSB;

    ESP_LOGD( DD_TMP_102_LOG_MSG_TAG, "TempRaw: %i [LSB], TempDeg: %0.3f °C", dd_tmp_102_data_s.temperature_raw_s16,
                                                                              dd_tmp_102_data_s.temperature_deg_f32 );
}


PRIVATE BOOLEAN dd_tmp_102_get_temperature_raw( S16* p_value_s16 )
{
    U8  register_vu8[2U];

    if ( NULL != p_value_s16 )
    {
        if ( FALSE == dd_i2c_read_burst( DD_TMP_102_I2C_ADDR, DD_TMP_102_TEMP, register_vu8, sizeof( register_vu8 ) ) )
        {
            return FALSE;
        }

        /* TMP-102 is returning the 16-bit register content in MSByte first. Further more, the temperature
         * data is left-justified and need to be shifted to the right by 4-bit (12-Bit mode) */
        *p_value_s16 = ( ( register_vu8[0U] << 8U ) | register_vu8[1U] ) >> 4U;
    }
    else
    {
        assert( NULL != p_value_s16 );
        return FALSE;
    }

    return TRUE;
}
