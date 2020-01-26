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
#include <math.h>

#include "dd_max-30102.h"
#include "dd_i2c.h"
#include "dd_database.h"

#include "../Config/dd_max-30102_Cfg.h"



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
BOOLEAN dd_max_30102_init(void)
{
    BOOLEAN state_b = FALSE;

    return state_b;
}


void dd_max_30102_main(void)
{

}

BOOLEAN dd_max_30102_get_int1( U8* p_register_u8 )
{
    BOOLEAN state_b = FALSE;

    if ( NULL != p_register_u8 )
    {
        state_b = dd_i2c_read_single( DD_MAX_30105_I2C_ADDR,
                                      DD_MAX_30102_INT_STAT_1,
                                      p_register_u8 );
    }

    return state_b;
}

BOOLEAN dd_max_30102_get_int2( U8* p_register_u8 )
{
    BOOLEAN state_b = FALSE;

    if ( NULL != p_register_u8 )
    {
        state_b = dd_i2c_read_single( DD_MAX_30105_I2C_ADDR,
                                      DD_MAX_30102_INT_STAT_2,
                                      p_register_u8 );
    }

    return state_b;
}

BOOLEAN dd_max_30102_set_int_a_full( BOOLEAN enable_b )
{
    BOOLEAN state_b = FALSE;

    if ( TRUE == enable_b )
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_A_FULL_MASK,
                                                 DD_MAX_30102_INT_A_FULL_ENABLE );
    }
    else
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_A_FULL_MASK,
                                                 DD_MAX_30102_INT_A_FULL_DISABLE );
    }

    return state_b;
}


BOOLEAN dd_max_30102_set_data_ready( BOOLEAN enable_b )
{
    BOOLEAN state_b = FALSE;

    if ( TRUE == enable_b )
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_PPG_RDY_MASK,
                                                 DD_MAX_30102_INT_PPG_RDY_ENABLE );
    }
    else
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_PPG_RDY_MASK,
                                                 DD_MAX_30102_INT_PPG_RDY_DISABLE );
    }

    return state_b;
}

BOOLEAN dd_max_30102_set_alc_ovf( BOOLEAN enable_b )
{
    BOOLEAN state_b = FALSE;

    if ( TRUE == enable_b )
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_ALC_OVF_MASK,
                                                 DD_MAX_30102_INT_ALC_OVF_ENABLE );
    }
    else
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_ALC_OVF_MASK,
                                                 DD_MAX_30102_INT_ALC_OVF_DISABLE );
    }

    return state_b;
}


BOOLEAN dd_max_30102_set_prox_int( BOOLEAN enable_b )
{
    BOOLEAN state_b = FALSE;

    if ( TRUE == enable_b )
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_PROX_INT_MASK,
                                                 DD_MAX_30102_INT_PROX_INT_ENABLE );
    }
    else
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_PROX_INT_MASK,
                                                 DD_MAX_30102_INT_PROX_INT_DISABLE );
    }

    return state_b;
}


BOOLEAN dd_max_30102_set_die_temp_ready( BOOLEAN enable_b )
{
    BOOLEAN state_b = FALSE;

    if ( TRUE == enable_b )
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_DIE_TEMP_RDY_MASK,
                                                 DD_MAX_30102_INT_DIE_TEMP_RDY_ENABLE );
    }
    else
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_INT_ENABLE_1,
                                                 DD_MAX_30102_INT_DIE_TEMP_RDY_MASK,
                                                 DD_MAX_30102_INT_DIE_TEMP_RDY_DISABLE );
    }

    return state_b;
}





BOOLEAN dd_max_30102_soft_reset( void )
{

    return ( dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                            DD_MAX_30102_MODE_CONFIG,
                                            DD_MAX_30102_RESET_MASK,
                                            DD_MAX_30102_RESET ) );

    //    // Poll for bit to clear, reset is then complete
    //    // Timeout after 100ms
    //    unsigned long startTime = millis();
    //    while (millis() - startTime < 100)
    //    {
    //      uint8_t response = readRegister8(_i2caddr, MAX30105_MODECONFIG);
    //      if ((response & MAX30105_RESET) == 0) break; //We're done!
    //      delay(1); //Let's not over burden the I2C bus
    //    }
}


BOOLEAN dd_max_30102_set_wake_up( const BOOLEAN enable_b )
{
    BOOLEAN state_b = FALSE;

    if ( TRUE == enable_b )
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_MODE_CONFIG,
                                                 DD_MAX_30102_SHUTDOWN_MASK,
                                                 DD_MAX_30102_WAKEUP );
    }
    else
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_MODE_CONFIG,
                                                 DD_MAX_30102_SHUTDOWN_MASK,
                                                 DD_MAX_30102_SHUTDOWN );
    }

    return state_b;
}

BOOLEAN dd_max_30102_set_led_mode( const DD_MAX_30102_LED_MODE mode_e )
{
    BOOLEAN state_b = FALSE;
    U8      mode_u8 = 0xFF;

    if ( DD_MAX_30102_MODE_SIZE > mode_e )
    {
        switch ( mode_e )
        {
        case DD_MAX_30102_MODE_RED:
            mode_u8 = DD_MAX_30102_MODE_LED_RED;
            break;

        case DD_MAX_30102_MODE_RED_IR:
            mode_u8 = DD_MAX_30102_MODE_LED_RED_IR;
            break;

        case DD_MAX_30102_MODE_MULTI_LED:
            mode_u8 = DD_MAX_30102_MODE_LED_MULTI;
            break;

        default:
            assert(    ( mode_e == DD_MAX_30102_MODE_RED )
                    || ( mode_e == DD_MAX_30102_MODE_RED_IR )
                    || ( mode_e == DD_MAX_30102_MODE_MULTI_LED ) );
            break;
        }

        if ( 0xFF != mode_u8 )
        {
            state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                     DD_MAX_30102_MODE_CONFIG,
                                                     DD_MAX_30102_MODE_MASK,
                                                     mode_u8 );
        }
        else
        {
            /* state_b set to FALSE already */
        }
    }
    else
    {
        assert( DD_MAX_30102_MODE_SIZE > mode_e );
    }

    return state_b;
}



BOOLEAN dd_max_30102_set_adc_range( const DD_MAX_30102_ADC_RANGE range_e )
{
    BOOLEAN state_b  = FALSE;
    U8      range_u8 = 0xFF;

    if ( DD_MAX_30102_ADC_RANGE_SIZE > range_e )
    {
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
            assert(    ( range_e == DD_MAX_30102_ADC_RANGE_2048 )
                    || ( range_e == DD_MAX_30102_ADC_RANGE_4096 )
                    || ( range_e == DD_MAX_30102_ADC_RANGE_8192 )
                    || ( range_e == DD_MAX_30102_ADC_RANGE_16384 ) );
            break;
        }

        if ( 0xFF != range_u8 )
        {
            state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                     DD_MAX_30102_MODE_CONFIG,
                                                     DD_MAX_30102_SPO2_ADC_RANGE_MASK,
                                                     range_u8 );
        }
        else
        {
            /* state_b set to FALSE already */
        }
    }
    else
    {
        assert( DD_MAX_30102_ADC_RANGE_SIZE > range_e );
    }

    return state_b;
}



BOOLEAN dd_max_30102_set_sample_rate( const DD_MAX_30102_SAMPLE_RATE rate_e )
{
    BOOLEAN state_b  = FALSE;
    U8      rate_u8 = 0xFF;

    if ( DD_MAX_30102_SAMPLE_RATE_SIZE > rate_e )
    {
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
            assert(    ( rate_e == DD_MAX_30102_SAMPLE_RATE_50 )
                    || ( rate_e == DD_MAX_30102_SAMPLE_RATE_100 )
                    || ( rate_e == DD_MAX_30102_SAMPLE_RATE_200 )
                    || ( rate_e == DD_MAX_30102_SAMPLE_RATE_400 )
                    || ( rate_e == DD_MAX_30102_SAMPLE_RATE_800 )
                    || ( rate_e == DD_MAX_30102_SAMPLE_RATE_1000 )
                    || ( rate_e == DD_MAX_30102_SAMPLE_RATE_1600 )
                    || ( rate_e == DD_MAX_30102_SAMPLE_RATE_3200 ) );
            break;
        }

        if ( 0xFF != rate_u8 )
        {
            state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                     DD_MAX_30102_MODE_CONFIG,
                                                     DD_MAX_30102_SPO2_SMP_RATE_MASK,
                                                     rate_u8 );
        }
        else
        {
            /* state_b set to FALSE already */
        }
    }
    else
    {
        assert( DD_MAX_30102_SAMPLE_RATE_SIZE > rate_u8 );
    }

    return state_b;
}




BOOLEAN dd_max_30102_set_pulse_width( const DD_MAX_30102_PULSE_WIDTH width_e )
{
    BOOLEAN state_b  = FALSE;
    U8      width_u8 = 0xFF;

    if ( DD_MAX_30102_PULSE_WIDTH_SIZE > width_e )
    {
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
            assert(    ( width_e == DD_MAX_30102_ADC_RANGE_2048 )
                    || ( width_e == DD_MAX_30102_ADC_RANGE_4096 )
                    || ( width_e == DD_MAX_30102_ADC_RANGE_8192 )
                    || ( width_e == DD_MAX_30102_ADC_RANGE_16384 ) );
            break;
        }

        if ( 0xFF != width_e )
        {
            state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                     DD_MAX_30102_MODE_CONFIG,
                                                     DD_MAX_30102_SPO2_ADC_RANGE_MASK,
                                                     width_u8 );
        }
        else
        {
            /* state_b set to FALSE already */
        }
    }
    else
    {
        assert( DD_MAX_30102_PULSE_WIDTH_SIZE > width_e );
    }

    return state_b;
}

BOOLEAN dd_max_30102_set_amplitude_set( const DD_MAX_30102_LED_TYPE type_e,
                                        const U8                    amplitude_u8 )
{
    BOOLEAN state_b          = FALSE;
    U8      type_reg_addr_u8 = 0xFF;

    if ( DD_MAX_30102_LED_TYPE_SIZE > type_e )
    {
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
            assert(    ( type_e == DD_MAX_30102_LED_TYPE_RED )
                    || ( type_e == DD_MAX_30102_LED_TYPE_IR )
                    || ( type_e == DD_MAX_30102_LED_TYPE_PROX ) );
            break;
        }

        if ( 0xFF != type_reg_addr_u8 )
        {
            state_b = dd_i2c_write_single( DD_MAX_30105_I2C_ADDR,
                                           type_reg_addr_u8,
                                           amplitude_u8 );
        }
        else
        {
            /* state_b set to FALSE already */
        }
    }
    else
    {
        assert( DD_MAX_30102_LED_TYPE_SIZE > type_e );
    }

    return state_b;
}

BOOLEAN dd_max_30102_set_proximity_threshold( U8 threshold_u8 )
{
    return ( dd_i2c_write_single( DD_MAX_30105_I2C_ADDR,
                                  DD_MAX_30102_PROX_INT_THRESH,
                                  threshold_u8 ) );
}

BOOLEAN dd_max_30102_setup_slot( const DD_MAX_30102_SLOT      slot_e,
                                 const DD_MAX_30102_SLOT_MODE mode_e )
{
    BOOLEAN state_b          = FALSE;
    U8      slot_mask_u8     = 0xFF;
    U8      slot_mode_u8     = 0xFF;
    U8      register_addr_u8 = 0xFF;
    U8      bit_offset_u8    = 0U;

    if (    ( DD_MAX_30102_SLOT_SIZE      > slot_e )
         && ( DD_MAX_30102_SLOT_MODE_SIZE > mode_e ) )
    {
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
            assert(    ( slot_e == DD_MAX_30102_SLOT_1 )
                    || ( slot_e == DD_MAX_30102_SLOT_2 )
                    || ( slot_e == DD_MAX_30102_SLOT_3 )
                    || ( slot_e == DD_MAX_30102_SLOT_4 ) );
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

        case DD_MAX_30102_SLOT_MONE_PILOT:
            slot_mode_u8 = DD_MAX_30102_SLOT_NONE_PILOT;
            break;

        case DD_MAX_30102_SLOT_MODE_LED_RED_PILOT:
            slot_mode_u8 = DD_MAX_30102_SLOT_RED_PILOT;
            break;

        case DD_MAX_30102_SLOT_MODE_LED_IR_PILOT:
            slot_mode_u8 = DD_MAX_30102_SLOT_IR_PILOT;
            break;

        default:
            assert(    ( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_NONE )
                    || ( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_LED_RED )
                    || ( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_LED_IR )
                    || ( slot_mode_u8 == DD_MAX_30102_SLOT_MONE_PILOT )
                    || ( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_LED_RED_PILOT )
                    || ( slot_mode_u8 == DD_MAX_30102_SLOT_MODE_LED_IR_PILOT ) );
            break;
        }

        if (    ( 0xFF != slot_mask_u8 )
             && ( 0xFF != slot_mode_u8 )
             && ( 0xFF != register_addr_u8 ) )
        {
            state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                     register_addr_u8,
                                                     slot_mask_u8,
                                                     ( slot_mode_u8 << bit_offset_u8 ) );
        }
        else
        {
            /* state_b set to FALSE already */
        }
    }
    else
    {
        assert(    ( DD_MAX_30102_SLOT_SIZE      > slot_e )
                || ( DD_MAX_30102_SLOT_MODE_SIZE > mode_e ) );
    }

    return state_b;
}

BOOLEAN dd_max_30102_set_sample_average( const DD_MAX_30102_SAMPLE_AVG average_e )
{
    BOOLEAN state_b    = FALSE;
    U8      average_u8 = 0xFF;

    if ( DD_MAX_30102_SAMPLE_AVG_SIZE > average_e )
    {
        switch ( average_e )
        {
        case DD_MAX_30102_SAMPLE_AVG_1:
            average_u8 = DD_MAX_30102_SMP_AVG_1;
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
            assert(    ( average_e == DD_MAX_30102_SAMPLE_AVG_1 )
                    || ( average_e == DD_MAX_30102_SAMPLE_AVG_2 )
                    || ( average_e == DD_MAX_30102_SAMPLE_AVG_4 )
                    || ( average_e == DD_MAX_30102_SAMPLE_AVG_8 )
                    || ( average_e == DD_MAX_30102_SAMPLE_AVG_16 )
                    || ( average_e == DD_MAX_30102_SAMPLE_AVG_32 ) );
            break;
        }

        if ( 0xFF != average_u8 )
        {
            state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                     DD_MAX_30102_FIFO_CONFIG,
                                                     DD_MAX_30102_SMP_AVG_MASK,
                                                     average_u8 );
        }
        else
        {
            /* state_b set to FALSE already */
        }
    }
    else
    {
        assert( DD_MAX_30102_SAMPLE_AVG_SIZE > average_e );
    }

    return state_b;
}


BOOLEAN dd_max_30102_set_fifo_roll_over( BOOLEAN enable_b )
{
    BOOLEAN state_b = FALSE;

    if ( TRUE == enable_b )
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_FIFO_CONFIG,
                                                 DD_MAX_30102_ROLL_OVER_MASK,
                                                 DD_MAX_30102_ROLL_OVER_ENABLE );
    }
    else
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_FIFO_CONFIG,
                                                 DD_MAX_30102_ROLL_OVER_MASK,
                                                 DD_MAX_30102_ROLL_OVER_DISABLE );
    }

    return state_b;
}

BOOLEAN dd_max_30102_set_fifo_a_full_value( U8 value_u8 )
{
    BOOLEAN state_b = FALSE;

    if ( DD_MAX_30102_A_FULL_MAX_VAL >= value_u8 )
    {
        state_b = dd_i2c_read_modify_write_mask( DD_MAX_30105_I2C_ADDR,
                                                 DD_MAX_30102_FIFO_CONFIG,
                                                 DD_MAX_30102_A_FULL_MASK,
                                                 value_u8 );
    }

    return state_b;
}

BOOLEAN dd_max_30102_get_ptr_value_by_type( const DD_MAX_30102_PTR_TYPE ptr_type_e,
                                                  U8*                   p_value_u8 )
{
    BOOLEAN state_b         = FALSE;
    U8      ptr_reg_addr_u8 = 0xFF;

    if (    ( DD_MAX_30102_PTR_TYPE_SIZE > ptr_type_e )
         && ( NULL != p_value_u8 ) )
    {
        switch ( ptr_type_e )
        {
        case DD_MAX_30102_PTR_TYPE_READ:
            ptr_reg_addr_u8 = DD_MAX_30102_FIFO_READ_PTR;
            break;

        case DD_MAX_30102_PTR_TYPE_WRITE:
            ptr_reg_addr_u8 = DD_MAX_30102_FIFO_WRITE_PTR;
            break;

        default:
            assert(    ( ptr_type_e == DD_MAX_30102_PTR_TYPE_READ )
                    || ( ptr_type_e == DD_MAX_30102_PTR_TYPE_WRITE ) );
            break;
        }

        if ( 0xFF != ptr_reg_addr_u8 )
        {
            state_b = dd_i2c_read_single( DD_MAX_30105_I2C_ADDR,
                                          ptr_reg_addr_u8,
                                          p_value_u8 );
        }
        else
        {
            /* state_b set to FALSE already */
        }
    }
    else
    {
        assert( (DD_MAX_30102_PTR_TYPE_SIZE > ptr_type_e )
                (NULL                      != p_value_u8 ) );
    }

    return state_b;
}


BOOLEAN dd_max_30102_read_temperature( F32* p_value_f32 )
{
    BOOLEAN state_b             = FALSE;
    U8      time_out_cnt_u8     = 10U;
    BOOLEAN time_out_b          = TRUE;
    U8      register_value_1_u8;
    U8      register_value_2_u8;

    if ( NULL != p_value_f32 )
    {
        /* DIE_TEMP_RDY interrupt must be enabled */
        /* See issue 19: https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/issues/19 */

        /* Step 1: Configure die temperature register to take 1 temperature sample */
        state_b = dd_i2c_write_single( DD_MAX_30105_I2C_ADDR,
                                       DD_MAX_30102_DIE_TEMP_CONFIG,
                                       0x01 );

        /* Check for I2C error */
        if ( TRUE != state_b )
        {
            return state_b;
        }

        while ( --time_out_cnt_u8 )
        {
            state_b = dd_i2c_read_single( DD_MAX_30105_I2C_ADDR,
                                          DD_MAX_30102_INT_STAT_2,
                                          &register_value_1_u8 );

            /* Check for I2C error */
            if ( TRUE != state_b )
            {
                break;
            }

            /* Check to see if DIE_TEMP_RDY interrupt is set */
            else if ( ( register_value_1_u8 & DD_MAX_30102_INT_DIE_TEMP_RDY_ENABLE ) > 0U )
            {
                time_out_b = FALSE;
                break;
            }

            /* Delay for some time to not over burden the I2C bus */
            vTaskDelay( 100U );
        }

        /* In case no time out occurred */
        if ( TRUE != time_out_b )
        {
            /* Step 2: Read die temperature register (integer) */
            state_b = dd_i2c_read_single( DD_MAX_30105_I2C_ADDR,
                                          DD_MAX_30102_DIE_TEMP_INT,
                                          &register_value_1_u8 );

            /* Check for I2C error */
            if ( TRUE != state_b )
            {
                return state_b;
            }

            state_b = dd_i2c_read_single( DD_MAX_30105_I2C_ADDR,
                                          DD_MAX_30102_DIE_TEMP_FRAC,
                                          &register_value_2_u8 );

            /* Check for I2C error */
            if ( TRUE != state_b )
            {
                return state_b;
            }

            /* Step 3: Calculate temperature (datasheet pg. 22) */
            *p_value_f32 = ( (F32) register_value_1_u8 ) + ( ( (F32) register_value_2_u8 ) * 0.0625F );
        }
        else
        {
            /* Set temperature value to BIG_NUMBER */
            *p_value_f32 = BIG_NUMBER;
        }
    }
    else
    {
        assert( NULL != p_value_f32 );
    }

    return state_b;
}


