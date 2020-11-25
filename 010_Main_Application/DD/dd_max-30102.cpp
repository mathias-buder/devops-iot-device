/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_max-30102.c
        @details MAX-30102 device driver implementation

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "esp_log.h"

#include "dd_max-30102.h"
#include "dd_i2c.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

DD_MAX_30102_C::DD_MAX_30102_C( U8 i2c_addr_u8 )
{
    this->i2c_addr_u8 = i2c_addr_u8;
}

DD_MAX_30102_C::~DD_MAX_30102_C()
{
}

DD_MAX_30102_DATA_OUT_TYPE* DD_MAX_30102_C::init( void )
{
    DD_MAX_30102_CONFIG_TYPE default_cfg_s = {
        .mode_cfg_e                    = DD_MAX_30102_MODE_HR,
        .adc_range_cfg_e               = DD_MAX_30102_ADC_RANGE_2048,
        .sample_rate_cfg_e             = DD_MAX_30102_SAMPLE_RATE_100,
        .sample_avg_cfg_e              = DD_MAX_30102_SAMPLE_AVG_4,
        .pulse_width_cfg_e             = DD_MAX_30102_PULSE_WIDTH_411,

        /** @details Default is 0x1F which corresponds to 6.4mA
         * amplitude = 0x02, 0.4mA  - Presence detection of ~4 inch
         * amplitude = 0x1F, 6.4mA  - Presence detection of ~8 inch
         * amplitude = 0x7F, 25.4mA - Presence detection of ~8 inch
         * amplitude = 0xFF, 50.0mA - Presence detection of ~12 inch */
        .led_amplitude_cfg_u8          = 0x1F,

        .temp_delay_ticks_cfg_u8       = 10U,

        /** @details This register sets the IR ADC count that will trigger the beginning of HR or SpO2 mode. The threshold
         * is defined as the 8 MSBs bits of the ADC count. For example, if PROX_INT_THRESH[7:0] = 0x01, then a 17-bit ADC
         * value of 1023 (decimal) or higher triggers the PROX_INT interrupt. If PROX_INT_THRESH[7:0] = 0xFF, then only a
         * saturated ADC triggers the interrupt. */
        .prox_threshold_cfg_u8         = 0x01U,

        .fifo_roll_over_cfg_b          = TRUE,
        .fifo_a_full_int_enable_cfg_b  = TRUE,
        .fifo_a_full_value_cfg_u8      = 2U,
        .die_temp_rdy_int_enable_cfg_b = TRUE,
        .prox_int_enable_cfg_b         = TRUE
    };

    return init( default_cfg_s );
}

DD_MAX_30102_DATA_OUT_TYPE* DD_MAX_30102_C::init( DD_MAX_30102_CONFIG_TYPE &r_config_s )
{
    ESP_LOGI( DD_MAX_30105_LOG_MSG_TAG, "Initializing ..." );

            /* Reset device */
    if (    ( FALSE == soft_reset() )

            /* Read device id and chip revision */
         || ( FALSE == get_part_id( this->data_out_s.part_id_u8 ) )
         || ( FALSE == get_rev_id( this->data_out_s.rev_id_u8 ) )

            /********* FIFO Configuration *********/
            /* The chip will average multiple samples of same type together */
         || ( FALSE == set_sample_average( r_config_s.sample_avg_cfg_e ) )

            /* Set to 30 samples to trigger an 'Almost Full' interrupt */
         || ( FALSE == set_fifo_a_full_value( r_config_s.fifo_a_full_value_cfg_u8 ) )
         || ( FALSE == set_fifo_int_a_full( r_config_s.fifo_a_full_int_enable_cfg_b ) )
         || ( FALSE == set_fifo_roll_over( r_config_s.fifo_roll_over_cfg_b ) )

            /* Reset the FIFO before checking the sensor */
         || ( FALSE == set_fifo_clear() )

            /********* SAMPLE Configuration *********/
         || ( FALSE == set_adc_range( r_config_s.adc_range_cfg_e ) )
         || ( FALSE == set_sample_rate( r_config_s.sample_rate_cfg_e ) )

            /****** LED Pulse Amplitude Configuration ******/
         || ( FALSE == set_pulse_amp( DD_MAX_30102_LED_TYPE_RED, r_config_s.led_amplitude_cfg_u8 ) )
         || ( FALSE == set_pulse_amp( DD_MAX_30102_LED_TYPE_IR, r_config_s.led_amplitude_cfg_u8 ) )
         || ( FALSE == set_pulse_amp( DD_MAX_30102_LED_TYPE_PROX, r_config_s.led_amplitude_cfg_u8 ) )

            /****** LED Pulse Width Configuration ******/
         || ( FALSE == set_pulse_width( r_config_s.pulse_width_cfg_e ) )

            /****** Interrupt Configuration ******/
         || ( FALSE == set_die_temp_rdy_int( r_config_s.die_temp_rdy_int_enable_cfg_b ) )
         || ( FALSE == set_prox_int( r_config_s.prox_int_enable_cfg_b ) )

         || ( FALSE == set_proximity_threshold( r_config_s.prox_threshold_cfg_u8 ) )

            /********* MODE Configuration *********/
         || ( FALSE == set_mode( this->data_out_s, r_config_s.mode_cfg_e ) )

         // || ( FALSE == dd_max_30102_setup_slot( DD_MAX_30102_SLOT_1, DD_MAX_30102_SLOT_MODE_LED_RED ) )
    )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "Initializing failed" );
        return NULL;
    }


    ESP_LOGI( DD_MAX_30105_LOG_MSG_TAG, "Done" );
    return &this->data_out_s;
}

void DD_MAX_30102_C::main( void )
{
    /* Clear data from previous cycle */
    memset( &this->data_out_s.temperature_raw_vu8[0U], 0U, sizeof( this->data_out_s.temperature_raw_vu8 ) );
    this->data_out_s.temperature_f32  = 0.0F;
    this->data_out_s.red_data_raw_u32 = 0U;
    this->data_out_s.ir_data_raw_u32  = 0U;

    /* Read device temperature */
    get_temperature( this->data_out_s );

    /* Read all interrupt flags */
    get_int_status( this->data_out_s.int_status_vb );

    /* Read new data from FIFO */
    get_fifo_data( this->data_out_s );

    ESP_LOGD( DD_MAX_30105_LOG_MSG_TAG, "Part-Id: %i @ Rev: %i, Temp: %0.4f, Prox-Int: %i", this->data_out_s.part_id_u8,
                                                                                            this->data_out_s.rev_id_u8,
                                                                                            this->data_out_s.temperature_f32,
                                                                                            this->data_out_s.int_status_vb[DD_MAX_30102_INT_TYPE_PROX_INT] );
}

BOOLEAN DD_MAX_30102_C::get_int1( U8& r_register_u8 )
{
    return DD_I2C_C::read_single( this->i2c_addr_u8, DD_MAX_30102_INT_STAT_1, &r_register_u8 );
}

BOOLEAN DD_MAX_30102_C::get_int2( U8& r_register_u8 )
{
    return DD_I2C_C::read_single( this->i2c_addr_u8, DD_MAX_30102_INT_STAT_2, &r_register_u8 );
}

BOOLEAN DD_MAX_30102_C::get_int_status( BOOLEAN (&r_int_status_vb)[DD_MAX_30102_INT_TYPE_SIZE] )
{
    U8 registers_vu8[2U];

    /* Read content of register "Interrupt Status 1" and "Interrupt Status 2"*/
    if ( FALSE == DD_I2C_C::read_burst( this->i2c_addr_u8, DD_MAX_30102_INT_STAT_1, registers_vu8, sizeof( registers_vu8 ) ) )
    {
        return FALSE;
    }

    /* Extract all interrupt flags */
    r_int_status_vb[DD_MAX_30102_INT_TYPE_A_FULL]       = ( registers_vu8[0] & DD_MAX_30102_INT_A_FULL_MASK )       >> 7U;
    r_int_status_vb[DD_MAX_30102_INT_TYPE_PRG_RDY]      = ( registers_vu8[0] & DD_MAX_30102_INT_PPG_RDY_MASK )      >> 6U;
    r_int_status_vb[DD_MAX_30102_INT_TYPE_ALC_OVF]      = ( registers_vu8[0] & DD_MAX_30102_INT_ALC_OVF_MASK )      >> 5U;
    r_int_status_vb[DD_MAX_30102_INT_TYPE_PROX_INT]     = ( registers_vu8[0] & DD_MAX_30102_INT_PROX_INT_MASK )     >> 4U;
    r_int_status_vb[DD_MAX_30102_INT_TYPE_PWR_RDY]      = ( registers_vu8[0] & DD_MAX_30102_INT_PWR_RDY_MASK );
    r_int_status_vb[DD_MAX_30102_INT_TYPE_DIE_TEMP_RDY] = ( registers_vu8[1] & DD_MAX_30102_INT_DIE_TEMP_RDY_MASK ) >> 1U;

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::set_fifo_int_a_full( const BOOLEAN enable_b )
{

    return ( DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8,
                                               DD_MAX_30102_INT_ENABLE_1,
                                               DD_MAX_30102_INT_A_FULL_MASK,
                                               ( ( TRUE == enable_b ) ? DD_MAX_30102_INT_A_FULL_ENABLE : DD_MAX_30102_INT_A_FULL_DISABLE ) ) );
}

BOOLEAN DD_MAX_30102_C::set_data_ready( const BOOLEAN enable_b )
{
    return ( DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8,
                                               DD_MAX_30102_INT_ENABLE_1,
                                               DD_MAX_30102_INT_PPG_RDY_MASK,
                                               ( ( TRUE == enable_b ) ? DD_MAX_30102_INT_PPG_RDY_ENABLE : DD_MAX_30102_INT_PPG_RDY_DISABLE ) ) );
}

BOOLEAN DD_MAX_30102_C::set_alc_ovf( const BOOLEAN enable_b )
{
    return ( DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8,
                                               DD_MAX_30102_INT_ENABLE_1,
                                               DD_MAX_30102_INT_ALC_OVF_MASK,
                                               ( ( TRUE == enable_b ) ? DD_MAX_30102_INT_ALC_OVF_ENABLE : DD_MAX_30102_INT_ALC_OVF_DISABLE ) ) );
}

BOOLEAN DD_MAX_30102_C::set_prox_int( BOOLEAN enable_b )
{
    return ( DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8,
                                               DD_MAX_30102_INT_ENABLE_1,
                                               DD_MAX_30102_INT_PROX_INT_MASK,
                                               ( ( TRUE == enable_b ) ? DD_MAX_30102_INT_PROX_INT_ENABLE : DD_MAX_30102_INT_PROX_INT_DISABLE ) ) );
}

BOOLEAN DD_MAX_30102_C::set_die_temp_rdy_int( const BOOLEAN enable_b )
{
    return ( DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8,
                                               DD_MAX_30102_INT_ENABLE_2,
                                               DD_MAX_30102_INT_DIE_TEMP_RDY_MASK,
                                               ( ( TRUE == enable_b ) ? DD_MAX_30102_INT_DIE_TEMP_RDY_ENABLE : DD_MAX_30102_INT_DIE_TEMP_RDY_DISABLE ) ) );
}

BOOLEAN DD_MAX_30102_C::soft_reset( void )
{
    U8 time_out_cnt_u8 = 100U;
    U8 register_value_u8;

    if ( FALSE == DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8, DD_MAX_30102_MODE_CONFIG, DD_MAX_30102_RESET_MASK, DD_MAX_30102_RESET ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_soft_reset I2C error" );
        return FALSE;
    }

    /* Poll DD_MAX_30102_RESET interrupt register */
    while ( --time_out_cnt_u8 )
    {
        if ( FALSE == DD_I2C_C::read_single( this->i2c_addr_u8, DD_MAX_30102_MODE_CONFIG, &register_value_u8 ) )
        {
            ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_soft_reset I2C error" );
            return FALSE;
        }

        /* Check to see if DD_MAX_30102_RESET interrupt is set */
        else if ( ( register_value_u8 & DD_MAX_30102_RESET ) == 0U )
        {
            /* Exit function in case reset is completed */
            return TRUE;
        }

        /* Delay for some time to not over burden the I2C bus */
        vTaskDelay( 100U );
    }

    /* Will be reached in case time_out_cnt_u8 is elapsed */
    ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_soft_reset time out" );
    return FALSE;
}

BOOLEAN DD_MAX_30102_C::set_wake_up( const BOOLEAN enable_b )
{

    return ( DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8,
                                               DD_MAX_30102_MODE_CONFIG,
                                               DD_MAX_30102_SHUTDOWN_MASK,
                                               ( ( TRUE == enable_b ) ? DD_MAX_30102_WAKEUP : DD_MAX_30102_SHUTDOWN ) ) );
}

BOOLEAN DD_MAX_30102_C::set_mode( DD_MAX_30102_DATA_OUT_TYPE& r_data_s,
                                  const DD_MAX_30102_MODE     mode_e )
{
    U8 mode_u8 = 0xFF;

    switch ( mode_e )
    {
    case DD_MAX_30102_MODE_HR:
        mode_u8 = DD_MAX_30102_LED_MODE_HR;
        break;

    case DD_MAX_30102_MODE_SPO2:
        mode_u8 = DD_MAX_30102_LED_MODE_SPO2;
        break;

    case DD_MAX_30102_MODE_MULTI_LED:
        mode_u8 = DD_MAX_30102_LED_MODE_MULTI;
        break;

    default:
        assert( mode_e == DD_MAX_30102_MODE_HR        );
        assert( mode_e == DD_MAX_30102_MODE_SPO2      );
        assert( mode_e == DD_MAX_30102_MODE_MULTI_LED );
        return FALSE;
    }

    if (    ( 0xFF  == mode_u8 )
         || ( FALSE == DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8, DD_MAX_30102_MODE_CONFIG, DD_MAX_30102_LED_MODE_MASK, mode_u8 ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_set_mode: %s error", ( 0xFF == mode_u8 ) ? "wrong argument" : "I2C" );
        return FALSE;
    }

    /* Updated mode in database */
    r_data_s.mode_e = mode_e;

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::set_adc_range( const DD_MAX_30102_ADC_RANGE range_e )
{
    U8 range_u8 = 0xFF;

    switch ( range_e )
    {
    case DD_MAX_30102_ADC_RANGE_2048:
        range_u8 = DD_MAX_30102_SPO2_ADC_RANGE_2048;
        break;

    case DD_MAX_30102_ADC_RANGE_4096:
        range_u8 = DD_MAX_30102_SPO2_ADC_RANGE_4096;
        break;

    case DD_MAX_30102_ADC_RANGE_8192:
        range_u8 = DD_MAX_30102_SPO2_ADC_RANGE_8192;
        break;

    case DD_MAX_30102_ADC_RANGE_16384:
        range_u8 = DD_MAX_30102_SPO2_ADC_RANGE_16384;
        break;

    default:
        assert( range_e == DD_MAX_30102_ADC_RANGE_2048  );
        assert( range_e == DD_MAX_30102_ADC_RANGE_4096  );
        assert( range_e == DD_MAX_30102_ADC_RANGE_8192  );
        assert( range_e == DD_MAX_30102_ADC_RANGE_16384 );
        return FALSE;
    }

    if (    ( 0xFF  == range_u8 )
         || ( FALSE == DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8, DD_MAX_30102_SPO2_CONFIG, DD_MAX_30102_SPO2_ADC_RANGE_MASK, range_u8 ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_set_adc_range %s error", ( 0xFF  == range_u8 ) ? "wrong argument" : "I2C" );
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::set_sample_rate( const DD_MAX_30102_SAMPLE_RATE rate_e )
{
    U8 rate_u8 = 0xFF;

    switch ( rate_e )
    {
    case DD_MAX_30102_SAMPLE_RATE_50:
        rate_u8 = DD_MAX_30102_SPO2_SMP_RATE_50;
        break;

    case DD_MAX_30102_SAMPLE_RATE_100:
        rate_u8 = DD_MAX_30102_SPO2_SMP_RATE_100;
        break;

    case DD_MAX_30102_SAMPLE_RATE_200:
        rate_u8 = DD_MAX_30102_SPO2_SMP_RATE_200;
        break;

    case DD_MAX_30102_SAMPLE_RATE_400:
        rate_u8 = DD_MAX_30102_SPO2_SMP_RATE_400;
        break;

    case DD_MAX_30102_SAMPLE_RATE_800:
        rate_u8 = DD_MAX_30102_SPO2_SMP_RATE_800;
        break;

    case DD_MAX_30102_SAMPLE_RATE_1000:
        rate_u8 = DD_MAX_30102_SPO2_SMP_RATE_1000;
        break;

    case DD_MAX_30102_SAMPLE_RATE_1600:
        rate_u8 = DD_MAX_30102_SPO2_SMP_RATE_1600;
        break;

    case DD_MAX_30102_SAMPLE_RATE_3200:
        rate_u8 = DD_MAX_30102_SPO2_SMP_RATE_3200;
        break;

    default:
        assert( rate_e == DD_MAX_30102_SAMPLE_RATE_50   );
        assert( rate_e == DD_MAX_30102_SAMPLE_RATE_100  );
        assert( rate_e == DD_MAX_30102_SAMPLE_RATE_200  );
        assert( rate_e == DD_MAX_30102_SAMPLE_RATE_400  );
        assert( rate_e == DD_MAX_30102_SAMPLE_RATE_800  );
        assert( rate_e == DD_MAX_30102_SAMPLE_RATE_1000 );
        assert( rate_e == DD_MAX_30102_SAMPLE_RATE_1600 );
        assert( rate_e == DD_MAX_30102_SAMPLE_RATE_3200 );
        return FALSE;
    }

    if (    ( 0xFF == rate_u8 )
         || ( FALSE == DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8, DD_MAX_30102_SPO2_CONFIG, DD_MAX_30102_SPO2_SMP_RATE_MASK, rate_u8 ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_set_sample_rate %s error", ( 0xFF == rate_u8 ) ? "wrong argument" : "I2C" );
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::set_pulse_width( const DD_MAX_30102_PULSE_WIDTH width_e )
{
    U8 width_u8 = 0xFF;

    switch ( width_e )
    {
    case DD_MAX_30102_PULSE_WIDTH_69:
        width_u8 = DD_MAX_30102_LED_PULSE_WIDTH_69;
        break;

    case DD_MAX_30102_PULSE_WIDTH_118:
        width_u8 = DD_MAX_30102_LED_PULSE_WIDTH_118;
        break;

    case DD_MAX_30102_PULSE_WIDTH_215:
        width_u8 = DD_MAX_30102_LED_PULSE_WIDTH_215;
        break;

    case DD_MAX_30102_PULSE_WIDTH_411:
        width_u8 = DD_MAX_30102_LED_PULSE_WIDTH_411;
        break;

    default:
        assert( width_e == DD_MAX_30102_PULSE_WIDTH_69  );
        assert( width_e == DD_MAX_30102_PULSE_WIDTH_118 );
        assert( width_e == DD_MAX_30102_PULSE_WIDTH_215 );
        assert( width_e == DD_MAX_30102_PULSE_WIDTH_411 );
        return FALSE;
    }

    if (    ( 0xFF == width_e )
         || ( FALSE == DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8, DD_MAX_30102_SPO2_CONFIG, DD_MAX_30102_LED_PULSE_WIDTH_MASK, width_u8 ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_set_pulse_width %s error", ( 0xFF == width_e ) ? "wrong argument" : "I2C" );
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::set_pulse_amp( const DD_MAX_30102_LED_TYPE type_e,
                                       const U8                    amplitude_u8 )
{
    U8 type_reg_addr_u8;

    switch ( type_e )
    {
    case DD_MAX_30102_LED_TYPE_RED:
        type_reg_addr_u8 = DD_MAX_30102_LED1_PULSE_AMP;
        break;

    case DD_MAX_30102_LED_TYPE_IR:
        type_reg_addr_u8 = DD_MAX_30102_LED2_PULSE_AMP;
        break;

    case DD_MAX_30102_LED_TYPE_PROX:
        type_reg_addr_u8 = DD_MAX_30102_LED_PROX_AMP;
        break;

    default:
        assert( type_e == DD_MAX_30102_LED_TYPE_RED  );
        assert( type_e == DD_MAX_30102_LED_TYPE_IR   );
        assert( type_e == DD_MAX_30102_LED_TYPE_PROX );
        return FALSE;
    }

    if (    ( 0xFF  == type_reg_addr_u8 )
         || ( FALSE == DD_I2C_C::write_single( this->i2c_addr_u8, type_reg_addr_u8, amplitude_u8 ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_set_amplitude %s error", ( 0xFF == type_reg_addr_u8 ) ? "wrong argument" : "I2C" );
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::set_proximity_threshold( const U8 threshold_u8 )
{
    return ( DD_I2C_C::write_single( this->i2c_addr_u8,
                                     DD_MAX_30102_PROX_INT_THRESH,
                                     threshold_u8 ) );
}

BOOLEAN DD_MAX_30102_C::setup_slot( const DD_MAX_30102_SLOT      slot_e,
                                    const DD_MAX_30102_SLOT_MODE mode_e )
{
    U8 slot_mask_u8     = 0xFF;
    U8 slot_mode_u8     = 0xFF;
    U8 register_addr_u8 = 0xFF;
    U8 bit_offset_u8    = 0U;

    /* Switch slot type */
    switch ( slot_e )
    {
    case DD_MAX_30102_SLOT_1:
        slot_mask_u8     = DD_MAX_30102_SLOT_1_MASK;
        register_addr_u8 = DD_MAX_30102_MULTI_LED_CONFIG_1;
        break;

    case DD_MAX_30102_SLOT_2:
        slot_mask_u8     = DD_MAX_30102_SLOT_2_MASK;
        register_addr_u8 = DD_MAX_30102_MULTI_LED_CONFIG_1;
        bit_offset_u8    = 4U;
        break;

    case DD_MAX_30102_SLOT_3:
        slot_mask_u8     = DD_MAX_30102_SLOT_3_MASK;
        register_addr_u8 = DD_MAX_30102_MULTI_LED_CONFIG_2;
        break;

    case DD_MAX_30102_SLOT_4:
        slot_mask_u8     = DD_MAX_30102_SLOT_4_MASK;
        register_addr_u8 = DD_MAX_30102_MULTI_LED_CONFIG_2;
        bit_offset_u8    = 4U;
        break;

    default:
        assert( slot_e == DD_MAX_30102_SLOT_1 );
        assert( slot_e == DD_MAX_30102_SLOT_2 );
        assert( slot_e == DD_MAX_30102_SLOT_3 );
        assert( slot_e == DD_MAX_30102_SLOT_4 );
        break;
    }

    /* Switch slot mode */
    switch ( mode_e )
    {
    case DD_MAX_30102_SLOT_MODE_NONE:
        slot_mode_u8 = DD_MAX_30102_SLOT_NONE;
        break;

    case DD_MAX_30102_SLOT_MODE_LED_RED:
        slot_mode_u8 = DD_MAX_30102_SLOT_RED_LED;
        break;

    case DD_MAX_30102_SLOT_MODE_LED_IR:
        slot_mode_u8 = DD_MAX_30102_SLOT_IR_LED;
        break;

    case DD_MAX_30102_SLOT_MODE_LED_RED_PILOT:
        slot_mode_u8 = DD_MAX_30102_SLOT_RED_PILOT;
        break;

    case DD_MAX_30102_SLOT_MODE_LED_IR_PILOT:
        slot_mode_u8 = DD_MAX_30102_SLOT_IR_PILOT;
        break;

    default:
        assert( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_NONE          );
        assert( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_LED_RED       );
        assert( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_LED_IR        );
        assert( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_LED_RED_PILOT );
        assert( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_LED_IR_PILOT  );
        break;
    }

    if (    ( 0xFF == slot_mask_u8 )
         || ( 0xFF == slot_mode_u8 )
         || ( 0xFF == register_addr_u8 )
         || ( FALSE == DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8, register_addr_u8, slot_mask_u8, ( slot_mode_u8 << bit_offset_u8 ) ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_setup_slot %s error", (    ( 0xFF  == slot_mask_u8 )
                                                                                  || ( 0xFF  == slot_mask_u8 )
                                                                                  || ( 0xFF  == slot_mask_u8 ) )
                                                                                   ? "wrong argument" : "I2C" );
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::set_sample_average( const DD_MAX_30102_SAMPLE_AVG average_e )
{
    U8 average_u8 = 0xFF;

    switch ( average_e )
    {
    case DD_MAX_30102_SAMPLE_AVG_NONE:
        average_u8 = DD_MAX_30102_SMP_AVG_NONE;
        break;

    case DD_MAX_30102_SAMPLE_AVG_2:
        average_u8 = DD_MAX_30102_SMP_AVG_2;
        break;

    case DD_MAX_30102_SAMPLE_AVG_4:
        average_u8 = DD_MAX_30102_SMP_AVG_4;
        break;

    case DD_MAX_30102_SAMPLE_AVG_8:
        average_u8 = DD_MAX_30102_SMP_AVG_8;
        break;

    case DD_MAX_30102_SAMPLE_AVG_16:
        average_u8 = DD_MAX_30102_SMP_AVG_16;
        break;

    case DD_MAX_30102_SAMPLE_AVG_32:
        average_u8 = DD_MAX_30102_SMP_AVG_32;
        break;

    default:
        assert( average_e == DD_MAX_30102_SAMPLE_AVG_NONE );
        assert( average_e == DD_MAX_30102_SAMPLE_AVG_2 );
        assert( average_e == DD_MAX_30102_SAMPLE_AVG_4 );
        assert( average_e == DD_MAX_30102_SAMPLE_AVG_8 );
        assert( average_e == DD_MAX_30102_SAMPLE_AVG_16 );
        assert( average_e == DD_MAX_30102_SAMPLE_AVG_32 );
        break;
    }

    if (    ( 0xFF == average_u8 )
         || ( FALSE == DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8, DD_MAX_30102_FIFO_CONFIG, DD_MAX_30102_SMP_AVG_MASK, average_u8 ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_set_sample_average %s error", ( 0xFF == average_u8 ) ? "wrong argument" : "I2C" );
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::set_fifo_roll_over( const BOOLEAN enable_b )
{
    return ( DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8,
                                               DD_MAX_30102_FIFO_CONFIG,
                                               DD_MAX_30102_ROLL_OVER_MASK,
                                               ( TRUE == enable_b ) ? DD_MAX_30102_ROLL_OVER_ENABLE : DD_MAX_30102_ROLL_OVER_DISABLE ) );
}

BOOLEAN DD_MAX_30102_C::set_fifo_a_full_value( const U8 value_u8 )
{
    if (    ( DD_MAX_30102_A_FULL_MAX_VAL < value_u8 )
         || ( FALSE == DD_I2C_C::read_modify_write_mask( this->i2c_addr_u8, DD_MAX_30102_FIFO_CONFIG, DD_MAX_30102_A_FULL_MASK, value_u8 ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_set_fifo_a_full_value %s error", ( DD_MAX_30102_A_FULL_MAX_VAL >= value_u8 ) ? "wrong argument" : "I2C" );
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::set_fifo_clear( void )
{
    if (    ( FALSE == DD_I2C_C::write_single( this->i2c_addr_u8, DD_MAX_30102_FIFO_WRITE_PTR, 0U ) )
         || ( FALSE == DD_I2C_C::write_single( this->i2c_addr_u8, DD_MAX_30102_FIFO_OVF_COUNTER, 0U ) )
         || ( FALSE == DD_I2C_C::write_single( this->i2c_addr_u8, DD_MAX_30102_FIFO_READ_PTR,  0U ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "dd_max_30102_set_fifo_clear I2C error" );
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::get_fifo_ptr_by_type( const DD_MAX_30102_PTR_TYPE ptr_type_e,
                                              U8&                         r_value_u8 )
{
    U8 ptr_reg_addr_u8 = 0xFF;

    switch ( ptr_type_e )
    {
    case DD_MAX_30102_PTR_TYPE_READ:
        ptr_reg_addr_u8 = DD_MAX_30102_FIFO_READ_PTR;
        break;

    case DD_MAX_30102_PTR_TYPE_WRITE:
        ptr_reg_addr_u8 = DD_MAX_30102_FIFO_WRITE_PTR;
        break;

    default:
        assert( ptr_type_e == DD_MAX_30102_PTR_TYPE_READ  );
        assert( ptr_type_e == DD_MAX_30102_PTR_TYPE_WRITE );
        return FALSE;
    }

    if (    ( 0xFF == ptr_reg_addr_u8 )
         || ( FALSE == DD_I2C_C::read_single( this->i2c_addr_u8, ptr_reg_addr_u8, &r_value_u8 ) ) )
    {
        return FALSE;
    }

    /* Mask from [4:0] according to datasheet pg. 13 */
    r_value_u8 &= 0x1F;


    return TRUE;
}

BOOLEAN DD_MAX_30102_C::get_fifo_ovf_cnt( U8& r_ovf_cnt_u8 )
{
    if ( FALSE == DD_I2C_C::read_single( this->i2c_addr_u8, DD_MAX_30102_FIFO_OVF_COUNTER, &r_ovf_cnt_u8 ) )
    {
        return FALSE;
    }

    /* Mask from [4:0] according to datasheet pg. 13 */
    r_ovf_cnt_u8 &= 0x1F;

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::get_temperature( DD_MAX_30102_DATA_OUT_TYPE &r_data_s )
{
    U8 time_out_cnt_u8 = DD_MAX_30105_TEMP_TIME_OUT_CNT;
    U8 register_value_u8;

    /* Initialize temperature value to BIG_NUMBER */
    r_data_s.temperature_f32 = BIG_NUMBER;

    /* DIE_TEMP_RDY interrupt must be enabled */
    /* See issue 19: https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/issues/19 */

    /* Step 1: Configure die temperature register to take 1 temperature sample*/
    if ( FALSE == DD_I2C_C::write_single( this->i2c_addr_u8, DD_MAX_30102_DIE_TEMP_CONFIG, 0x01 ) )
    {
        return FALSE;
    }

    while ( --time_out_cnt_u8 )
    {
        /* Read die DIE_TEMP_RDY interrupt register content */
        if ( FALSE == DD_I2C_C::read_single( this->i2c_addr_u8, DD_MAX_30102_INT_STAT_2, &register_value_u8 ) )
        {
            return FALSE;
        }

        /* Check to see if DIE_TEMP_RDY interrupt is set */
        if ( ( register_value_u8 & DD_MAX_30102_INT_DIE_TEMP_RDY_ENABLE ) > 0U )
        {
            /* Step 2: Read die temperature register ( integer + fraction ) */
            if ( FALSE == DD_I2C_C::read_burst( this->i2c_addr_u8, DD_MAX_30102_DIE_TEMP_INT, r_data_s.temperature_raw_vu8, DD_MAX_30102_TEMP_COMP_SIZE ) )
            {
                return FALSE;
            }

            /* Step 3: Calculate temperature (datasheet pg. 22)
             * Register DD_MAX_30102_DIE_TEMP_INT stores the integer temperature data in 2's complement format, where each bit
             * corresponds to 1°C. The value read by the I2C driver is in U8 format by default and need to be converted by casting
             * it to type S8 ( signed char ) */
            r_data_s.temperature_f32 = ( F32 )   ( (S8) r_data_s.temperature_raw_vu8[DD_MAX_30102_TEMP_COMP_INT] )
                                               + ( ( (F32) r_data_s.temperature_raw_vu8[DD_MAX_30102_TEMP_COMP_FRAC] ) * DD_MAX_30102_DIE_TEMP_FRAC_RES );

            break;
        }
        /* Delay for some time to not over burden the I2C bus */
        vTaskDelay( (TickType_t) DD_MAX_30105_TEMP_DELAY_TICKS );
    }

    return TRUE;
}

BOOLEAN DD_MAX_30102_C::get_part_id( U8 &r_register_u8 )
{
    return ( DD_I2C_C::read_single( this->i2c_addr_u8, DD_MAX_30102_PART_ID, &r_register_u8 ) );
}

BOOLEAN DD_MAX_30102_C::get_rev_id( U8& r_register_u8 )
{

    return DD_I2C_C::read_single( this->i2c_addr_u8, DD_MAX_30102_REVISION_ID, &r_register_u8 );
}

BOOLEAN DD_MAX_30102_C::get_fifo_data( DD_MAX_30102_DATA_OUT_TYPE& r_data_s )
{
    U8  idx_u8;
    S8  num_samples_s8;
    U8  register_vu8[2U * DD_MAX_30102_FIFO_CHANNEL_SIZE];
    U32 red_fifo_data_u32;
    U32 ir_fifo_data_u32;
    U8  fifo_ovf_cnt_u8;


    /* Get FIFO pointers */
    if (    ( FALSE == get_fifo_ptr_by_type( DD_MAX_30102_PTR_TYPE_READ, r_data_s.read_ptr_u8 ) )
         || ( FALSE == get_fifo_ptr_by_type( DD_MAX_30102_PTR_TYPE_WRITE, r_data_s.write_ptr_u8 ) )
         || ( FALSE == get_fifo_ovf_cnt( fifo_ovf_cnt_u8 ) ) )
    {
        ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "Error while reading FIFO read/write properties" );
        return FALSE;
    }

    /* Check if new data is available */
    if ( r_data_s.read_ptr_u8 != r_data_s.write_ptr_u8 )
    {
        /* Calculate the number of reading to be taken from the FIFO */
        num_samples_s8 = r_data_s.write_ptr_u8 - r_data_s.read_ptr_u8;

        /* Handle FIFO pointer wrap around */
        if ( 0 > num_samples_s8 )
        {
            num_samples_s8 += DD_MAX_30102_FIFO_SIZE;
        }

        ESP_LOGD( DD_MAX_30105_LOG_MSG_TAG, "FIFO: %i samples, WRITE_PTR: %i, READ_PTR: %i, OVF_CNT: %i", num_samples_s8,
                                                                                                          r_data_s.write_ptr_u8,
                                                                                                          r_data_s.read_ptr_u8,
                                                                                                          fifo_ovf_cnt_u8 );

        switch ( r_data_s.mode_e )
        {
            case DD_MAX_30102_MODE_HR:
            {
            for ( idx_u8 = 0U; idx_u8 < num_samples_s8; ++idx_u8 )
            {
                /* Read one complete data block (2 x 3 byte) off the FIFO */
                if ( FALSE == DD_I2C_C::read_burst( this->i2c_addr_u8, DD_MAX_30102_FIFO_DATA, register_vu8, 1U * DD_MAX_30102_FIFO_CHANNEL_SIZE ) )
                {
                    /* Invalidate raw values */
                    r_data_s.ir_data_raw_u32 = MAX_U32;

                    ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "Error while reading FIFO data" );
                    return FALSE;
                }

                /* Extract 18-bit sample for each channel */
                red_fifo_data_u32 = ( register_vu8[0U] << 16U ) | ( register_vu8[1U] << 8U ) | ( register_vu8[2U] );

                r_data_s.ir_data_raw_u32 += red_fifo_data_u32;
            }

            r_data_s.ir_data_raw_u32 /= num_samples_s8;

            ESP_LOGD( DD_MAX_30105_LOG_MSG_TAG, "IR: %i", r_data_s.ir_data_raw_u32 );

            break;
        }
            case DD_MAX_30102_MODE_SPO2:
            {
            for ( idx_u8 = 0U; idx_u8 < num_samples_s8; ++idx_u8 )
            {
                /* Read one complete data block (2 x 3 byte) off the FIFO */
                if ( FALSE == DD_I2C_C::read_burst( this->i2c_addr_u8, DD_MAX_30102_FIFO_DATA, register_vu8, 2U * DD_MAX_30102_FIFO_CHANNEL_SIZE ) )
                {
                    /* Invalidate raw values */
                    r_data_s.red_data_raw_u32 = MAX_U32;
                    r_data_s.ir_data_raw_u32  = MAX_U32;

                    ESP_LOGE( DD_MAX_30105_LOG_MSG_TAG, "Error while reading FIFO data" );
                    return FALSE;
                }

                /* Extract 18-bit sample for each channel */
                red_fifo_data_u32 = ( register_vu8[0U] << 16U ) | ( register_vu8[1U] << 8U ) | ( register_vu8[2U] );
                ir_fifo_data_u32  = ( register_vu8[3U] << 16U ) | ( register_vu8[4U] << 8U ) | ( register_vu8[5U] );

                r_data_s.red_data_raw_u32 += red_fifo_data_u32;
                r_data_s.ir_data_raw_u32  += ir_fifo_data_u32;
            }

            r_data_s.red_data_raw_u32 /= num_samples_s8;
            r_data_s.ir_data_raw_u32  /= num_samples_s8;

            ESP_LOGD( DD_MAX_30105_LOG_MSG_TAG, "RED: %i, IR: %i", r_data_s.red_data_raw_u32,
                                                                   r_data_s.ir_data_raw_u32 );

            break;
        }
            default:
            {
                assert( DD_MAX_30102_MODE_HR        == r_data_s.mode_e );
                assert( DD_MAX_30102_MODE_SPO2      == r_data_s.mode_e );
                assert( DD_MAX_30102_MODE_MULTI_LED == r_data_s.mode_e );
                return FALSE;
            }
        }
    }

    return TRUE;
}

