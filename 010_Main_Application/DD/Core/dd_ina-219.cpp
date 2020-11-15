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

#include "../../UTIL/UTIL.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE BOOLEAN dd_ina_219_configure( DD_INA_219_SHUNT_VOL_RANGE shunt_voltage_range_e,
                                      DD_INA_219_SADC_RES_AVE    shunt_voltage_res_ave_e,
                                      DD_INA_219_BUS_VOL_RANGE   bus_voltage_range_e,
                                      DD_INA_219_BADC_RES_AVE    bus_voltage_res_ave_e );
PRIVATE BOOLEAN dd_ina_219_calibrate( void );
PRIVATE BOOLEAN dd_ina_219_get_shunt_voltage_raw( S16* const p_value_s16 );
PRIVATE BOOLEAN dd_ina_219_get_bus_voltage_raw( DD_INA_219_BUS_VOL_DATA_TYPE* const p_bus_data_s );
PRIVATE BOOLEAN dd_ina_219_get_power_raw( U16* const p_value_u16 );
PRIVATE BOOLEAN dd_ina_219_get_current_raw( S16* const p_value_s16 );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
BOOLEAN dd_ina_219_init( void )
{
    ESP_LOGI( DD_INA_219_LOG_MSG_TAG, "Initializing ..." );

    /* Initialize global voltage ranges with invalid range */
    dd_ina_219_data_s.shunt_voltage_range_e            = DD_INA_219_SHUNT_VOL_RANGE_SIZE;
    dd_ina_219_data_s.shunt_adc_resolution_averaging_e = DD_INA_219_SADC_RES_AVE_SIZE;
    dd_ina_219_data_s.bus_voltage_range_e              = DD_INA_219_BUS_VOL_RANGE_SIZE;
    dd_ina_219_data_s.bus_adc_resolution_averaging_e   = DD_INA_219_BADC_RES_AVE_SIZE;

    /* Set global configuration register */
    if (   ( FALSE == dd_ina_219_configure( dd_ina_219_shunt_voltage_range_cfg_e,
                                            dd_ina_219_shunt_voltage_adc_res_ave_cfg_e,
                                            dd_ina_219_bus_voltage_range_cfg_e,
                                            dd_ina_219_bus_voltage_adc_res_ave_cfg_e )
           )
           /* Set calibration register */
        || ( FALSE == dd_ina_219_calibrate() ) )
    {
        ESP_LOGE( DD_INA_219_LOG_MSG_TAG, "Initializing failed" );
        return FALSE;
    }

    ESP_LOGI( DD_INA_219_LOG_MSG_TAG, "Done" );
    return TRUE;
}

void dd_ina_219_main( void )
{
    /* Read raw ADC measurements */
    dd_ina_219_get_shunt_voltage_raw( &dd_ina_219_data_s.shunt_voltage_raw_s16 );
    dd_ina_219_get_bus_voltage_raw( &dd_ina_219_data_s.bus_voltage_data_s );
    dd_ina_219_get_power_raw( &dd_ina_219_data_s.power_raw_u16 );
    dd_ina_219_get_current_raw( &dd_ina_219_data_s.current_raw_s16 );

    /* Convert Bus Voltage into mV / V */
    dd_ina_219_data_s.bus_voltage_mV_f32 = dd_ina_219_data_s.bus_voltage_data_s.voltage_raw_u16 * DD_INA_219_V_BUS_LSB_MILLI_VOLT;
    dd_ina_219_data_s.bus_voltage_V_f32  = dd_ina_219_data_s.bus_voltage_data_s.voltage_raw_u16 * DD_INA_219_V_BUS_LSB_VOLT;

    /* Convert Shunt Voltage into mV */
    dd_ina_219_data_s.shunt_voltage_mV_f32  = dd_ina_219_data_s.shunt_voltage_raw_s16 * DD_INA_219_V_SHUNT_LSB_MILLI_VOLT;

    /* Convert Current into mA */
    dd_ina_219_data_s.current_mA_f32 = dd_ina_219_data_s.current_raw_s16 * dd_ina_219_data_s.current_lsb_A_f32 * 1000.0F;

    /* Convert Power into mW */
    dd_ina_219_data_s.power_mW_f32 = dd_ina_219_data_s.power_raw_u16 * dd_ina_219_data_s.power_lsb_W_f32 * 1000.0F;


    ESP_LOGD( DD_INA_219_LOG_MSG_TAG, "ShuntV %0.2f mV, BusV %0.2f V, Current %0.2f mA, Power %0.3f mW", dd_ina_219_data_s.shunt_voltage_mV_f32,
                                                                                                         dd_ina_219_data_s.bus_voltage_V_f32,
                                                                                                         dd_ina_219_data_s.current_mA_f32,
                                                                                                         dd_ina_219_data_s.power_mW_f32 );
}

PRIVATE BOOLEAN dd_ina_219_configure( DD_INA_219_SHUNT_VOL_RANGE shunt_voltage_range_e,
                                      DD_INA_219_SADC_RES_AVE    shunt_voltage_res_ave_e,
                                      DD_INA_219_BUS_VOL_RANGE   bus_voltage_range_e,
                                      DD_INA_219_BADC_RES_AVE    bus_voltage_res_ave_e )
{
    U16 shunt_voltage_range_u16   = 0U;
    U16 shunt_voltage_res_ave_u16 = 0U;
    U16 bus_voltage_range_u16     = 0U;
    U16 bus_voltage_res_ave_u16   = 0U;
    U16 config_register_u16       = 0U;

    /* Set shunt voltage range */
    switch ( shunt_voltage_range_e )
    {
    case DD_INA_219_SHUNT_VOL_RANGE_40MV:
        shunt_voltage_range_u16 = DD_INA_219_PG_GAIN_1_40MV;
        break;
    case DD_INA_219_SHUNT_VOL_RANGE_80MV:
        shunt_voltage_range_u16 = DD_INA_219_PG_GAIN_1_2_80MV;
        break;
    case DD_INA_219_SHUNT_VOL_RANGE_160MV:
        shunt_voltage_range_u16 = DD_INA_219_PG_GAIN_1_4_160MV;
        break;
    case DD_INA_219_SHUNT_VOL_RANGE_320MV:
        shunt_voltage_range_u16 = DD_INA_219_PG_GAIN_1_8_320MV;
        break;

    default:
        assert( DD_INA_219_SHUNT_VOL_RANGE_40MV  == shunt_voltage_range_e );
        assert( DD_INA_219_SHUNT_VOL_RANGE_80MV  == shunt_voltage_range_e );
        assert( DD_INA_219_SHUNT_VOL_RANGE_160MV == shunt_voltage_range_e );
        assert( DD_INA_219_SHUNT_VOL_RANGE_320MV == shunt_voltage_range_e );
        return FALSE;
    }

    /* Shunt voltage ADC resolution/Averaging setting */
    switch ( shunt_voltage_res_ave_e )
    {
    case DD_INA_219_SADC_RES_AVE_12BIT_1S_532US:
        shunt_voltage_res_ave_u16 = DD_INA_219_SADC_12BIT_1S_532US;
        break;
    case DD_INA_219_SADC_RES_AVE_12BIT_2S_1060US:
        shunt_voltage_res_ave_u16 = DD_INA_219_SADC_12BIT_2S_1060US;
        break;
    case DD_INA_219_SADC_RES_AVE_12BIT_4S_2130US:
        shunt_voltage_res_ave_u16 = DD_INA_219_SADC_12BIT_4S_2130US;
        break;
    case DD_INA_219_SADC_RES_AVE_12BIT_8S_4260US:
        shunt_voltage_res_ave_u16 = DD_INA_219_SADC_12BIT_8S_4260US;
        break;
    case DD_INA_219_SADC_RES_AVE_12BIT_16S_8512US:
        shunt_voltage_res_ave_u16 = DD_INA_219_SADC_12BIT_16S_8512US;
        break;
    case DD_INA_219_SADC_RES_AVE_12BIT_32S_17MS:
        shunt_voltage_res_ave_u16 = DD_INA_219_SADC_12BIT_32S_17MS;
        break;
    case DD_INA_219_SADC_RES_AVE_12BIT_64S_34MS:
        shunt_voltage_res_ave_u16 = DD_INA_219_SADC_12BIT_64S_34MS;
        break;
    case DD_INA_219_SADC_RES_AVE_12BIT_128S_69MS:
        shunt_voltage_res_ave_u16 = DD_INA_219_SADC_12BIT_128S_69MS;
        break;

    default:
        assert( DD_INA_219_SADC_RES_AVE_12BIT_1S_532US   == shunt_voltage_res_ave_e );
        assert( DD_INA_219_SADC_RES_AVE_12BIT_2S_1060US  == shunt_voltage_res_ave_e );
        assert( DD_INA_219_SADC_RES_AVE_12BIT_4S_2130US  == shunt_voltage_res_ave_e );
        assert( DD_INA_219_SADC_RES_AVE_12BIT_8S_4260US  == shunt_voltage_res_ave_e );
        assert( DD_INA_219_SADC_RES_AVE_12BIT_16S_8512US == shunt_voltage_res_ave_e );
        assert( DD_INA_219_SADC_RES_AVE_12BIT_64S_34MS   == shunt_voltage_res_ave_e );
        assert( DD_INA_219_SADC_RES_AVE_12BIT_128S_69MS  == shunt_voltage_res_ave_e );
        return FALSE;
    }

    /* Bus voltage ADC resolution/Averaging setting */
    switch ( bus_voltage_res_ave_e )
    {
    case DD_INA_219_BADC_RES_AVE_12BIT_1S_532US:
        bus_voltage_res_ave_u16 = DD_INA_219_BADC_12BIT_1S_532US;
        break;
    case DD_INA_219_BADC_RES_AVE_12BIT_2S_1060US:
        bus_voltage_res_ave_u16 = DD_INA_219_BADC_12BIT_2S_1060US;
        break;
    case DD_INA_219_BADC_RES_AVE_12BIT_4S_2130US:
        bus_voltage_res_ave_u16 = DD_INA_219_BADC_12BIT_4S_2130US;
        break;
    case DD_INA_219_BADC_RES_AVE_12BIT_8S_4260US:
        bus_voltage_res_ave_u16 = DD_INA_219_BADC_12BIT_8S_4260US;
        break;
    case DD_INA_219_BADC_RES_AVE_12BIT_16S_8512US:
        bus_voltage_res_ave_u16 = DD_INA_219_BADC_12BIT_16S_8512US;
        break;
    case DD_INA_219_BADC_RES_AVE_12BIT_32S_17MS:
        bus_voltage_res_ave_u16 = DD_INA_219_BADC_12BIT_32S_17MS;
        break;
    case DD_INA_219_BADC_RES_AVE_12BIT_64S_34MS:
        bus_voltage_res_ave_u16 = DD_INA_219_BADC_12BIT_64S_34MS;
        break;
    case DD_INA_219_BADC_RES_AVE_12BIT_128S_69MS:
        bus_voltage_res_ave_u16 = DD_INA_219_BADC_12BIT_128S_69MS;
        break;

    default:
        assert( DD_INA_219_BADC_RES_AVE_12BIT_1S_532US   == bus_voltage_res_ave_e );
        assert( DD_INA_219_BADC_RES_AVE_12BIT_2S_1060US  == bus_voltage_res_ave_e );
        assert( DD_INA_219_BADC_RES_AVE_12BIT_4S_2130US  == bus_voltage_res_ave_e );
        assert( DD_INA_219_BADC_RES_AVE_12BIT_8S_4260US  == bus_voltage_res_ave_e );
        assert( DD_INA_219_BADC_RES_AVE_12BIT_16S_8512US == bus_voltage_res_ave_e );
        assert( DD_INA_219_BADC_RES_AVE_12BIT_64S_34MS   == bus_voltage_res_ave_e );
        assert( DD_INA_219_BADC_RES_AVE_12BIT_128S_69MS  == bus_voltage_res_ave_e );
        return FALSE;
    }

    /* Set bus voltage range */
    switch ( bus_voltage_range_e )
    {
    case DD_INA_219_BUS_VOL_RANGE_16V:
        bus_voltage_range_u16 = DD_INA_219_BRNG_RANGE_16V;
        break;
    case DD_INA_219_BUS_VOL_RANGE_32V:
        bus_voltage_range_u16 = DD_INA_219_BRNG_RANGE_32V;
        break;

    default:
        bus_voltage_range_u16 = DD_INA_219_BRNG_RANGE_16V;
        assert( DD_INA_219_BUS_VOL_RANGE_16V  == bus_voltage_range_u16 );
        assert( DD_INA_219_BUS_VOL_RANGE_32V  == bus_voltage_range_u16 );
        return FALSE;
    }

    /* Construct 16-bit configuration register content (datasheet pg. 19) */
    config_register_u16 =   DD_INA_219_MODE_SHUNT_BUS_VOLT_CONT /* Operating Mode */
                          | shunt_voltage_res_ave_u16           /* SADC Shunt ADC Resolution/Averaging */
                          | bus_voltage_res_ave_u16             /* BADC Bus ADC Resolution/Averaging */
                          | shunt_voltage_range_u16             /* PGA (Shunt Voltage Only) */
                          | bus_voltage_range_u16;              /* Bus Voltage Range */

    /* Update configuration Registers */
    if ( FALSE == dd_i2c_write_burst( DD_INA_219_I2C_ADDR, DD_INA_219_CONFIG, ( (U8*) &config_register_u16 ), sizeof( config_register_u16 ) ) )
    {
        ESP_LOGE( DD_INA_219_LOG_MSG_TAG, "Couldn't update configuration registers" );
        return FALSE;
    }

    /* Update current voltage ranges and ADC settings into global device data structure */
    dd_ina_219_data_s.shunt_voltage_range_e            = shunt_voltage_range_e;
    dd_ina_219_data_s.shunt_adc_resolution_averaging_e = shunt_voltage_res_ave_e;
    dd_ina_219_data_s.bus_voltage_range_e              = bus_voltage_range_e;
    dd_ina_219_data_s.bus_adc_resolution_averaging_e   = bus_voltage_res_ave_e;

    return TRUE;
}

PRIVATE BOOLEAN dd_ina_219_calibrate( void )
{
    /* Calculating calibration register
     * See datasheet pg. 12 for details */
    dd_ina_219_data_s.current_lsb_A_f32        = ( F32 )( ( dd_ina_219_max_current_mA_cfg_f32 / 1000.0F ) / 32768U );
    dd_ina_219_data_s.power_lsb_W_f32          = 20.0F * dd_ina_219_data_s.current_lsb_A_f32;
    dd_ina_219_data_s.calibration_register_u16 = ( U16 )( 0.04096F / ( dd_ina_219_data_s.current_lsb_A_f32 * DD_INA_219_SHUNT_RESISTOR_VALUE_OHM ) );

    /* Update Calibration Registers */
    if ( FALSE == dd_i2c_write_burst( DD_INA_219_I2C_ADDR, DD_INA_219_CALIB_DATA, ( (U8*) &dd_ina_219_data_s.calibration_register_u16 ), sizeof( dd_ina_219_data_s.calibration_register_u16 ) ) )
    {
        ESP_LOGE( DD_INA_219_LOG_MSG_TAG, "Couldn't update calibration registers" );
        return FALSE;
    }

    return TRUE;
}

PRIVATE BOOLEAN dd_ina_219_get_shunt_voltage_raw( S16* const p_value_s16 )
{
    U8 register_vu8[2U];

    if ( NULL != p_value_s16 )
    {
        if ( FALSE == dd_i2c_read_burst( DD_INA_219_I2C_ADDR, DD_INA_219_SHUNT_VOLTAGE_DATA, register_vu8, sizeof( register_vu8 ) ) )
        {
            return FALSE;
        }

        /* INA-219 is returning the 16-bit register content in MSByte first. Therefore, the byte order
         * of 16-bit variable register_u16 need to be swapped. */
        *p_value_s16 = ( register_vu8[0U] << 8U ) | register_vu8[1U];
    }
    else
    {
        assert( NULL != p_value_s16 );
        return FALSE;
    }

    return TRUE;
}


PRIVATE BOOLEAN dd_ina_219_get_bus_voltage_raw( DD_INA_219_BUS_VOL_DATA_TYPE* const p_bus_data_s )
{
    U8  register_vu8[2U];

    if ( NULL != p_bus_data_s )
    {
        if ( FALSE == dd_i2c_read_burst( DD_INA_219_I2C_ADDR, DD_INA_219_BUS_VOLTAGE_DATA, register_vu8, sizeof( register_vu8 ) ) )
        {
            return FALSE;
        }

        /* INA-219 is returning the 16-bit register content in MSByte first. Therefore, the byte order
         * of 16-bit variable register_u16 need to be swapped. */
        p_bus_data_s->voltage_raw_u16 = ( ( register_vu8[0U] << 8U ) | register_vu8[1U] );

        /* Extract status information (datasheet pg. 23) */
        p_bus_data_s->math_overflow_b = TEST_BIT( p_bus_data_s->voltage_raw_u16, DD_INA_219_V_BUS_STAT_OVF );
        p_bus_data_s->conv_ready_b    = TEST_BIT( p_bus_data_s->voltage_raw_u16, DD_INA_219_V_BUS_STAT_CNVR );

        /* Extract raw ADC reading (datasheet pg. 23) In order to compute the value of the
         * Bus Voltage, Bus Voltage Register contents must be shifted right by three bits. */
        p_bus_data_s->voltage_raw_u16 >>= 3U;
    }
    else
    {
        assert( NULL != p_bus_data_s );
        return FALSE;
    }

    return TRUE;
}


PRIVATE BOOLEAN dd_ina_219_get_power_raw( U16* const p_value_u16 )
{
    U8 register_vu8[2U];

    if ( NULL != p_value_u16 )
    {
        if ( FALSE == dd_i2c_read_burst( DD_INA_219_I2C_ADDR, DD_INA_219_POWER_DATA, register_vu8, sizeof( register_vu8 ) ) )
        {
            return FALSE;
        }

        /* INA-219 is returning the 16-bit register content in MSByte first. Therefore, the byte order
         * of 16-bit variable register_u16 need to be swapped. */
        *p_value_u16 = ( register_vu8[0U] << 8U ) | register_vu8[1U];
    }
    else
    {
        assert( NULL != p_value_u16 );
        return FALSE;
    }

    return TRUE;
}


PRIVATE BOOLEAN dd_ina_219_get_current_raw( S16* const p_value_s16 )
{
    U8 register_vu8[2U];

    if ( NULL != p_value_s16 )
    {
        if ( FALSE == dd_i2c_read_burst( DD_INA_219_I2C_ADDR, DD_INA_219_CURRENT_DATA, register_vu8, sizeof( register_vu8 ) ) )
        {
            return FALSE;
        }

        /* INA-219 is returning the 16-bit register content in MSByte first. Therefore, the byte order
         * of 16-bit variable register_u16 need to be swapped. */
        *p_value_s16 = ( register_vu8[0U] << 8U ) | register_vu8[1U];
    }
    else
    {
        assert( NULL != p_value_s16 );
        return FALSE;
    }

    return TRUE;
}
