/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_tmp_102.c
        @details TMP-102 device driver implementation

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <assert.h>

#include "esp_log.h"

#include "dd_tmp-102.h"
#include "dd_i2c.h"

#include "../../UTIL/UTIL.h"


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

DD_TMP_102_C::DD_TMP_102_C( U8 i2c_addr_u8 )
{
    this->i2c_addr_u8 = i2c_addr_u8;
}


DD_TMP_102_C::~DD_TMP_102_C()
{
}


DD_TMP_102_DATA_OUT_TYPE* DD_TMP_102_C::init( void )
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
    if ( FALSE == DD_I2C_C::write_burst( this->i2c_addr_u8, DD_TMP_102_CONFIG, ( (U8*) &config_register_u16 ), sizeof( config_register_u16 ) ) )
    {
        ESP_LOGE( DD_TMP_102_LOG_MSG_TAG, "Couldn't update configuration registers" );
        return NULL;
    }

    ESP_LOGI( DD_TMP_102_LOG_MSG_TAG, "Done" );
    return &this->data_out_s;
}


void DD_TMP_102_C::main( void )
{
    /* Get raw temperature reading */
    get_temperature_raw( &this->data_out_s.temperature_raw_s16 );

    /* Convert raw temperature reading to °C */
    this->data_out_s.temperature_deg_f32 = this->data_out_s.temperature_raw_s16 * DD_TMP_102_TEMP_LSB;

    ESP_LOGD( DD_TMP_102_LOG_MSG_TAG, "TempRaw: %i [LSB], TempDeg: %0.3f °C", this->data_out_s.temperature_raw_s16,
                                                                              this->data_out_s.temperature_deg_f32 );
}


BOOLEAN DD_TMP_102_C::get_temperature_raw( S16* p_value_s16 )
{
    U8  register_vu8[2U];

    if ( NULL != p_value_s16 )
    {
        if ( FALSE == DD_I2C_C::read_burst( this->i2c_addr_u8, DD_TMP_102_TEMP, register_vu8, sizeof( register_vu8 ) ) )
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
