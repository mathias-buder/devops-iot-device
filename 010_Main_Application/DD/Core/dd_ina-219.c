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
PRIVATE BOOLEAN dd_ina_219_get_shunt_voltage_raw( U16* const p_value_u16 );
PRIVATE BOOLEAN dd_ina_219_get_bus_voltage_raw( U16* const p_value_u16 );
PRIVATE BOOLEAN dd_ina_219_get_power_raw( U16* const p_value_u16 );
PRIVATE BOOLEAN dd_ina_219_get_current_raw( U16* const p_value_u16 );

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
    /* Read raw ADC measurements */
    dd_ina_219_get_shunt_voltage_raw( &dd_ina_219_data_s.shunt_voltage_raw_u16 );
    dd_ina_219_get_bus_voltage_raw( &dd_ina_219_data_s.bus_voltage_raw_u16 );
    dd_ina_219_get_power_raw( &dd_ina_219_data_s.power_raw_u16 );
    dd_ina_219_get_current_raw( &dd_ina_219_data_s.current_raw_u16 );

    ESP_LOGD( DD_INA_219_LOG_MSG_TAG, "Raw ShuntV %i, Raw BusV %i, Raw Power %i, Raw Current %i", dd_ina_219_data_s.shunt_voltage_raw_u16,
                                                                                                  dd_ina_219_data_s.bus_voltage_raw_u16,
                                                                                                  dd_ina_219_data_s.power_raw_u16,
                                                                                                  dd_ina_219_data_s.current_raw_u16 );
}

PRIVATE BOOLEAN dd_ina_219_get_shunt_voltage_raw( U16* const p_value_u16 )
{
    if ( NULL != p_value_u16 )
    {
        if ( FALSE == dd_i2c_read_burst( DD_INA_219_I2C_ADDR, DD_INA_219_SHUNT_VOLTAGE_DATA, (U8*) p_value_u16, sizeof( U16 ) ) )
        {
            return FALSE;
        }
    }
    else
    {
        assert( NULL != p_value_u16 );
        return FALSE;
    }

    return TRUE;
}


PRIVATE BOOLEAN dd_ina_219_get_bus_voltage_raw( U16* const p_value_u16 )
{
    if ( NULL != p_value_u16 )
    {
        if ( FALSE == dd_i2c_read_burst( DD_INA_219_I2C_ADDR, DD_INA_219_BUS_VOLTAGE_DATA, (U8*) p_value_u16, sizeof( U16 ) ) )
        {
            return FALSE;
        }
    }
    else
    {
        assert( NULL != p_value_u16 );
        return FALSE;
    }

    return TRUE;
}


PRIVATE BOOLEAN dd_ina_219_get_power_raw( U16* const p_value_u16 )
{
    if ( NULL != p_value_u16 )
    {
        if ( FALSE == dd_i2c_read_burst( DD_INA_219_I2C_ADDR, DD_INA_219_POWER_DATA, (U8*) p_value_u16, sizeof( U16 ) ) )
        {
            return FALSE;
        }
    }
    else
    {
        assert( NULL != p_value_u16 );
        return FALSE;
    }

    return TRUE;
}


PRIVATE BOOLEAN dd_ina_219_get_current_raw( U16* const p_value_u16 )
{
    if ( NULL != p_value_u16 )
    {
        if ( FALSE == dd_i2c_read_burst( DD_INA_219_I2C_ADDR, DD_INA_219_CURRENT_DATA, (U8*) p_value_u16, sizeof( U16 ) ) )
        {
            return FALSE;
        }
    }
    else
    {
        assert( NULL != p_value_u16 );
        return FALSE;
    }

    return TRUE;
}
