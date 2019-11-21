/*********************************************************************
    MODULE SPECIFICATION:

        $URL$
        $Revision$
        $Author$
        $Date$

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2019

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>

#include "dd_i2c.h"
#include "../../types.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
PRIVATE DD_I2C_ERROR_TYPE dd_i2c_error_s;

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/**
 * @details This function performs a read/write test on the specified I2C device to make sure the
 * low-level interface is working correctly.
 * @param[in] error_t I2C error code
 * @return TRUE if NO error is present
 */
PRIVATE BOOLEAN dd_i2c_handle_error( esp_err_t error_t );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

void dd_i2c_init(void)
{
    i2c_config_t  dd_i2c_if_config_s;

    /* Initialize I2C peripheral structure */
    dd_i2c_if_config_s.mode             = DD_I2C_OPERATION_MODE;
    dd_i2c_if_config_s.sda_io_num       = DD_I2C_SDA_IO_NUM;
    dd_i2c_if_config_s.scl_io_num       = DD_I2C_SCL_IO_NUM;
    dd_i2c_if_config_s.sda_pullup_en    = DD_I2C_SDA_PULLUP_CFG;
    dd_i2c_if_config_s.scl_pullup_en    = DD_I2C_SCL_PULLUP_CFG;
    dd_i2c_if_config_s.master.clk_speed = DD_I2C_SCL_FREQ;

    ESP_ERROR_CHECK( i2c_param_config( DD_I2C_PORT_NUM,
                                       &dd_i2c_if_config_s ) );

    ESP_ERROR_CHECK( i2c_driver_install( DD_I2C_PORT_NUM,
                                         dd_i2c_if_config_s.mode,
                                         0U,
                                         0U,
                                         0U ) );

    /* Initialize I2C error structure */
    dd_i2c_error_s.current_t  = DD_I2C_ERROR_OK;
    dd_i2c_error_s.previous_t = DD_I2C_ERROR_OK;
    dd_i2c_error_s.state_b    = TRUE;
}


BOOLEAN dd_i2c_read_single( U8  device_addr_u8,
                            U8  register_addr_u8,
                            U8* data_u8 )
{
    DD_I2C_ERROR     i2c_error_t   = DD_I2C_ERROR_FAIL;
    i2c_cmd_handle_t p_i2c_command = NULL;

    p_i2c_command = i2c_cmd_link_create();

    /* Check for NULL pointer */
    if( NULL != p_i2c_command )
    {
        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );


        ESP_ERROR_CHECK( i2c_master_write_byte(  p_i2c_command,
                                                (device_addr_u8 << 1) | I2C_MASTER_WRITE,
                                                 TRUE ) );


        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                register_addr_u8,
                                                TRUE ) );


        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );



        ESP_ERROR_CHECK( i2c_master_write_byte(  p_i2c_command,
                                                (device_addr_u8 << 1) | I2C_MASTER_READ,
                                                 TRUE ) );


        ESP_ERROR_CHECK( i2c_master_read_byte( p_i2c_command,
                                               data_u8,
                                               FALSE ) );


        ESP_ERROR_CHECK( i2c_master_stop( p_i2c_command ) );


        i2c_error_t = i2c_master_cmd_begin( DD_I2C_PORT_NUM,
                                            p_i2c_command,
                                            DD_I2C_BUS_BUSY_TIME_OUT );

        i2c_cmd_link_delete( p_i2c_command );
    }
    else
    {
        /* Assert in case of NULL pointer */
        assert( NULL != p_i2c_command );
    }

    /* Temporary hard-coded return value to "TRUE" because function i2c_master_cmd_begin()
     * currently returns DD_I2C_ERROR_TIMEOUT which brakes the I2C communication. */
    return TRUE;
}


BOOLEAN dd_i2c_read_burst( U8  device_addr_u8,
                           U8  register_addr_u8,
                           U8* p_data_u8,
                           U8  data_size_u8 )
{

    DD_I2C_ERROR     i2c_error_t   = DD_I2C_ERROR_FAIL;
    i2c_cmd_handle_t p_i2c_command = NULL;


    p_i2c_command = i2c_cmd_link_create();

    /* Check for NULL pointer */
    if( NULL != p_i2c_command )
    {
        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );


        ESP_ERROR_CHECK( i2c_master_write_byte(  p_i2c_command,
                                                (device_addr_u8 << 1) | I2C_MASTER_WRITE,
                                                 TRUE ) );


        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                register_addr_u8,
                                                TRUE ) );


        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );



        ESP_ERROR_CHECK( i2c_master_write_byte(  p_i2c_command,
                                                (device_addr_u8 << 1) | I2C_MASTER_READ,
                                                 TRUE ) );

        /* Check for single-byte-read request */
        if( data_size_u8 > 1U )
        {
            ESP_ERROR_CHECK( i2c_master_read(  p_i2c_command,
                                               p_data_u8,
                                              (data_size_u8 - 1),
                                               FALSE ) );


            ESP_ERROR_CHECK( i2c_master_read_byte( p_i2c_command,
                                                   &p_data_u8[data_size_u8 - 1],
                                                   TRUE ) );
        }
        else
        {
            ESP_ERROR_CHECK( i2c_master_read_byte( p_i2c_command,
                                                   p_data_u8,
                                                   FALSE ) );
        }

        ESP_ERROR_CHECK( i2c_master_stop( p_i2c_command ) );


        i2c_error_t = i2c_master_cmd_begin( DD_I2C_PORT_NUM,
                                            p_i2c_command,
                                            DD_I2C_BUS_BUSY_TIME_OUT );

        i2c_cmd_link_delete( p_i2c_command );
    }
    else
    {
        /* Assert in case of NULL pointer */
        assert( NULL != p_i2c_command );
    }

   return dd_i2c_handle_error( i2c_error_t );
}


BOOLEAN dd_i2c_read_bit( U8       device_addr_u8,
                         U8       register_addr_u8,
                         U8       bit_position_u8,
                         BOOLEAN* bit_value_b )
{
    BOOLEAN state_b      = FALSE;
    U8 register_value_u8 = 0U;

    state_b = dd_i2c_read_single(  device_addr_u8,
                                   register_addr_u8,
                                  &register_value_u8 );

    *bit_value_b = ( register_value_u8 & (1 << bit_position_u8) ) != 0U ? TRUE : FALSE;

    return state_b;
}


BOOLEAN dd_i2c_read_bits( U8  device_addr_u8,
                          U8  regisetr_addr_u8,
                          U8  start_bit_u8,
                          U8  length_u8,
                          U8* p_data_u8 )
{
  /* 01101001 read byte
     76543210 bit numbers
        xxx   args: bitStart=4, length=3
        010   masked
       -> 010 shifted */

    BOOLEAN state_b      = FALSE;
    U8 register_value_u8 = 0U;
    U8 bit_mask_u8       = 0U;

    state_b = dd_i2c_read_single( device_addr_u8,
                                  regisetr_addr_u8,
                                 &register_value_u8 );

    bit_mask_u8 = ( ( 1 << length_u8 ) - 1 ) << ( start_bit_u8 - length_u8 + 1 );

    register_value_u8 &= bit_mask_u8;

    register_value_u8 >>= (start_bit_u8 - length_u8 + 1);

    *p_data_u8 = register_value_u8;

    return state_b;
}


BOOLEAN dd_i2c_write_single( U8 device_addr_u8,
                             U8 register_addr_u8,
                             U8 data_u8 )
{
    DD_I2C_ERROR     i2c_error_t   = DD_I2C_ERROR_FAIL;
    i2c_cmd_handle_t p_i2c_command = NULL;

    /* Get I2C command handle */
    p_i2c_command = i2c_cmd_link_create();

    /* Check for NULL pointer */
    if( NULL != p_i2c_command )
    {
        /* Create command sequence */
        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command) );

        /* Add appropriate device address to command sequence */
        ESP_ERROR_CHECK( i2c_master_write_byte(  p_i2c_command,
                                                (device_addr_u8 << 1) | I2C_MASTER_WRITE,
                                                 TRUE) );

        /* Added appropriate I2C device register to command sequence */
        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                register_addr_u8,
                                                TRUE) );

        /* Add one bytes of data to command sequence */
        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                data_u8,
                                                TRUE) );

        /* Finish I2C transmission command sequence */
        ESP_ERROR_CHECK( i2c_master_stop( p_i2c_command) );

        /* Start command sequence transmission */
        i2c_error_t = i2c_master_cmd_begin( DD_I2C_PORT_NUM,
                                             p_i2c_command,
                                             DD_I2C_BUS_BUSY_TIME_OUT );

        /* Delete command sequence and release/return I2C resource */
        i2c_cmd_link_delete( p_i2c_command );
    }
    else
    {
        /* Assert in case of NULL pointer */
        assert( NULL != p_i2c_command );
    }

    return dd_i2c_handle_error( i2c_error_t );
}


BOOLEAN dd_i2c_write_burst( U8  device_addr_u8,
                            U8  register_addr_u8,
                            U8 *p_data_u8,
                            U8  data_size_u8 )
{
    DD_I2C_ERROR     i2c_error_t   = DD_I2C_ERROR_FAIL;
    i2c_cmd_handle_t p_i2c_command = NULL;

    /* Get I2C command handle */
    p_i2c_command = i2c_cmd_link_create();

    /* Check for NULL pointer */
    if( NULL != p_i2c_command )
    {
        /* Create command sequence */
        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );

        /* Add appropriate device address to command sequence */
        ESP_ERROR_CHECK( i2c_master_write_byte(  p_i2c_command,
                                                (device_addr_u8 << 1) | I2C_MASTER_WRITE,
                                                 TRUE ) );

        /* Added appropriate I2C device register to command sequence */
        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                register_addr_u8,
                                                TRUE ) );

        /* Add data_size_u8 bytes of data to command sequence */
        ESP_ERROR_CHECK( i2c_master_write( p_i2c_command,
                                           p_data_u8,
                                           data_size_u8,
                                           TRUE ) );

        /* Finish I2C transmission command sequence */
        ESP_ERROR_CHECK( i2c_master_stop( p_i2c_command) );

        /* Start command sequence transmission */
        i2c_error_t =  i2c_master_cmd_begin( DD_I2C_PORT_NUM,
                                             p_i2c_command,
                                             DD_I2C_BUS_BUSY_TIME_OUT );

        /* Delete command sequence and release/return I2C resource */
        i2c_cmd_link_delete( p_i2c_command );
    }
    else
    {
        /* Assert in case of NULL pointer */
        assert( NULL != p_i2c_command );
    }

    return dd_i2c_handle_error( i2c_error_t );
}


BOOLEAN dd_i2c_write_bits( U8 device_addr_u8,
                           U8 register_addr_u8,
                           U8 bit_start_u8,
                           U8 length_u8,
                           U8 data_u8 )
{
    /*    010 value to write (data_u8)
     76543210 bit numbers
        xxx   args: bit_start_u8 = 4, data_size_u8 = 3
     00011100 mask byte
     10101111 original value (sample)
     10100011 original & ~mask
     10101011 masked | value     */

    BOOLEAN state_b      = FALSE;
    U8 register_value_u8 = 0U;
    U8 bit_mask_u8       = 0U;

        /* Read corresponding register */
        state_b = dd_i2c_read_single( device_addr_u8,
                                      register_addr_u8,
                                     &register_value_u8 );

        /* Check for error in read cycle */
        if( FALSE != state_b )
        {
            /* Create bit-mask with provided bit_start_u8 and data_size_u8 */
            bit_mask_u8 = ( ( 1 << length_u8 ) - 1 ) << ( bit_start_u8 - length_u8 + 1 );

            /* Shift data_u8 into correct position */
            data_u8 <<= ( bit_start_u8 - length_u8 + 1 );

            /* Zero all non-important bits in data_u8 */
            data_u8 &= bit_mask_u8;

            /* Zero all important bits in existing byte */
            register_value_u8 &= ~( bit_mask_u8 );

            /* Combine data_u8 with existing byte */
            register_value_u8 |= data_u8;

            /* Write updated register_value_u8 to corresponding register */
            state_b = dd_i2c_write_single( device_addr_u8,
                                           register_addr_u8,
                                           register_value_u8 );
        }

    return state_b;
}


BOOLEAN dd_i2c_read_modify_write( U8 device_addr_u8,
                                  U8 register_addr_u8,
                                  U8 bit_position_u8,
                                  BOOLEAN bit_value_b )
{
    BOOLEAN status_b           = FALSE;
    U8      register_value_u8  = 0U;

    status_b = dd_i2c_read_single( device_addr_u8,
                                   register_addr_u8,
                                  &register_value_u8 );

    /* Check for error in read cycle */
    if( FALSE != status_b )
    {
        register_value_u8 = ( bit_value_b != 0 ) ? ( register_value_u8 | (1 << bit_position_u8) ) : ( register_value_u8 & ~(1 << bit_position_u8) );

        status_b = dd_i2c_write_single( device_addr_u8,
                                        register_addr_u8,
                                        register_value_u8 );
    }

    return status_b;
}


PRIVATE BOOLEAN dd_i2c_handle_error( esp_err_t error_t )
{
    /* Check for very first error */
    if( error_t != DD_I2C_ERROR_OK )
    {
        dd_i2c_error_s.state_b = FALSE;

        if(    ( dd_i2c_error_s.current_t  == DD_I2C_ERROR_OK )
            && ( dd_i2c_error_s.previous_t == DD_I2C_ERROR_OK ) )
        {
            dd_i2c_error_s.current_t = error_t;
        }
        else
        {
            dd_i2c_error_s.previous_t = dd_i2c_error_s.current_t;
            dd_i2c_error_s.current_t  = error_t;
        }
    }
    else
    {
        dd_i2c_error_s.current_t = DD_I2C_ERROR_OK;
        dd_i2c_error_s.state_b   = TRUE;
    }

    return dd_i2c_error_s.state_b;
}


/* This function shall execute an I2C test procedure to make sure the connected devices are working properly */
DD_I2C_ERROR_TYPE* dd_i2c_get_error(void)
{
    return &dd_i2c_error_s;
}

BOOLEAN dd_i2c_interface_test( U8 device_addr_u8,
                               U8 register_low_addr_u8,
                               U8 register_low_exp_val_u8,
                               U8 register_high_addr_u8,
                               U8 register_high_exp_val_u8 )
{

    BOOLEAN status_b = FALSE;

    return status_b;

}

