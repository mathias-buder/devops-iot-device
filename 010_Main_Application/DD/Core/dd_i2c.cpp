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

#include "dd_i2c.h"
#include "dd_database.h"

#include "../../types.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

DD_I2C_C::DD_I2C_C()
{
}


DD_I2C_C::~DD_I2C_C()
{
}


void DD_I2C_C::init( void )
{
    i2c_config_t dd_i2c_if_config_s;

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
}

BOOLEAN DD_I2C_C::read_single( U8  device_addr_u8,
                            U8  register_addr_u8,
                            U8* data_u8 )
{
    DD_I2C_ERROR     i2c_error_e   = DD_I2C_ERROR_FAIL;
    i2c_cmd_handle_t p_i2c_command = NULL;

    p_i2c_command = i2c_cmd_link_create();

    /* Check for NULL pointer */
    if ( NULL != p_i2c_command )
    {
        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );

        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                ( device_addr_u8 << 1 ) | I2C_MASTER_WRITE,
                                                TRUE ) );

        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                register_addr_u8,
                                                TRUE ) );

        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );

        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                ( device_addr_u8 << 1 ) | I2C_MASTER_READ,
                                                TRUE ) );

        ESP_ERROR_CHECK( i2c_master_read_byte( p_i2c_command,
                                               data_u8,
                                               I2C_MASTER_LAST_NACK ) );

        ESP_ERROR_CHECK( i2c_master_stop( p_i2c_command ) );

        i2c_error_e = ( DD_I2C_ERROR ) i2c_master_cmd_begin( DD_I2C_PORT_NUM,
                                                             p_i2c_command,
                                                             DD_I2C_BUS_BUSY_TIME_OUT );

        i2c_cmd_link_delete( p_i2c_command );
    }
    else
    {
        /* Assert in case of NULL pointer */
        assert( NULL != p_i2c_command );
    }

    return handle_error( i2c_error_e,
                         DD_I2C_ACCESS_TYPE_RD_SINGLE,
                         device_addr_u8,
                         register_addr_u8 );
}

BOOLEAN DD_I2C_C::read_burst( U8  device_addr_u8,
                              U8  register_addr_u8,
                              U8* p_data_u8,
                              U8  data_size_u8 )
{

    DD_I2C_ERROR     i2c_error_e   = DD_I2C_ERROR_FAIL;
    i2c_cmd_handle_t p_i2c_command = NULL;

    p_i2c_command = i2c_cmd_link_create();

    /* Check for NULL pointer */
    if ( NULL != p_i2c_command )
    {
        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );

        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                ( device_addr_u8 << 1 ) | I2C_MASTER_WRITE,
                                                TRUE ) );

        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                register_addr_u8,
                                                TRUE ) );

        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );

        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                ( device_addr_u8 << 1 ) | I2C_MASTER_READ,
                                                TRUE ) );

        /* Check for single-byte-read request */
        if ( data_size_u8 > 1U )
        {
            ESP_ERROR_CHECK( i2c_master_read( p_i2c_command,
                                              p_data_u8,
                                              ( data_size_u8 - 1 ),
                                              I2C_MASTER_ACK ) );

            ESP_ERROR_CHECK( i2c_master_read_byte( p_i2c_command,
                                                   &p_data_u8[data_size_u8 - 1],
                                                   I2C_MASTER_LAST_NACK ) );
        }
        else
        {
            ESP_ERROR_CHECK( i2c_master_read_byte( p_i2c_command,
                                                   p_data_u8,
                                                   I2C_MASTER_LAST_NACK ) );
        }

        ESP_ERROR_CHECK( i2c_master_stop( p_i2c_command ) );

        i2c_error_e = ( DD_I2C_ERROR ) i2c_master_cmd_begin( DD_I2C_PORT_NUM,
                                                             p_i2c_command,
                                                             DD_I2C_BUS_BUSY_TIME_OUT );

        i2c_cmd_link_delete( p_i2c_command );
    }
    else
    {
        /* Assert in case of NULL pointer */
        assert( NULL != p_i2c_command );
    }

    return handle_error( i2c_error_e,
                         DD_I2C_ACCESS_TYPE_RD_BURST,
                         device_addr_u8,
                         register_addr_u8 );
}

BOOLEAN DD_I2C_C::read_bit( U8       device_addr_u8,
                         U8       register_addr_u8,
                         U8       bit_position_u8,
                         BOOLEAN* bit_value_b )
{
    U8 register_value_u8;

    if ( FALSE == read_single( device_addr_u8, register_addr_u8, &register_value_u8 ) )
    {
        return FALSE;
    }

    *bit_value_b = ( register_value_u8 & ( 1 << bit_position_u8 ) ) != 0U ? TRUE : FALSE;

    return TRUE;
}

BOOLEAN DD_I2C_C::read_bits( U8  device_addr_u8,
                          U8  regisetr_addr_u8,
                          U8  start_bit_u8,
                          U8  length_u8,
                          U8* p_data_u8 )
{
    /* 01101001 read byte
     76543210 bit numbers
        bbb   args: bitStart=4, length=3
        010   masked
       -> 010 shifted */

    U8 register_value_u8;
    U8 bit_mask_u8;

    if ( FALSE == read_single( device_addr_u8, regisetr_addr_u8, &register_value_u8 ) )
    {
        return FALSE;
    }

    bit_mask_u8 = ( ( 1 << length_u8 ) - 1 ) << ( start_bit_u8 - length_u8 + 1 );

    register_value_u8 &= bit_mask_u8;

    register_value_u8 >>= ( start_bit_u8 - length_u8 + 1 );

    *p_data_u8 = register_value_u8;

    return TRUE;
}

BOOLEAN DD_I2C_C::write_single( U8 device_addr_u8,
                             U8 register_addr_u8,
                             U8 data_u8 )
{
    DD_I2C_ERROR     i2c_error_e   = DD_I2C_ERROR_FAIL;
    i2c_cmd_handle_t p_i2c_command = NULL;

    /* Get I2C command handle */
    p_i2c_command = i2c_cmd_link_create();

    /* Check for NULL pointer */
    if ( NULL != p_i2c_command )
    {
        /* Create command sequence */
        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );

        /* Add appropriate device address to command sequence */
        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                ( device_addr_u8 << 1 ) | I2C_MASTER_WRITE,
                                                TRUE ) );

        /* Added appropriate I2C device register to command sequence */
        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                register_addr_u8,
                                                TRUE ) );

        /* Add one bytes of data to command sequence */
        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                data_u8,
                                                TRUE ) );

        /* Finish I2C transmission command sequence */
        ESP_ERROR_CHECK( i2c_master_stop( p_i2c_command ) );

        /* Start command sequence transmission */
        i2c_error_e = ( DD_I2C_ERROR ) i2c_master_cmd_begin( DD_I2C_PORT_NUM,
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

    return handle_error( i2c_error_e,
                         DD_I2C_ACCESS_TYPE_WD_SINGLE,
                         device_addr_u8,
                         register_addr_u8 );
}

BOOLEAN DD_I2C_C::write_burst( U8  device_addr_u8,
                            U8  register_addr_u8,
                            U8* p_data_u8,
                            U8  data_size_u8 )
{
    DD_I2C_ERROR     i2c_error_e   = DD_I2C_ERROR_FAIL;
    i2c_cmd_handle_t p_i2c_command = NULL;

    /* Get I2C command handle */
    p_i2c_command = i2c_cmd_link_create();

    /* Check for NULL pointer */
    if ( NULL != p_i2c_command )
    {
        /* Create command sequence */
        ESP_ERROR_CHECK( i2c_master_start( p_i2c_command ) );

        /* Add appropriate device address to command sequence */
        ESP_ERROR_CHECK( i2c_master_write_byte( p_i2c_command,
                                                ( device_addr_u8 << 1 ) | I2C_MASTER_WRITE,
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
        ESP_ERROR_CHECK( i2c_master_stop( p_i2c_command ) );

        /* Start command sequence transmission */
        i2c_error_e = ( DD_I2C_ERROR ) i2c_master_cmd_begin( DD_I2C_PORT_NUM,
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

    return handle_error( i2c_error_e,
                         DD_I2C_ACCESS_TYPE_RD_BURST,
                         device_addr_u8,
                         register_addr_u8 );
}

BOOLEAN DD_I2C_C::write_bits( U8 device_addr_u8,
                           U8 register_addr_u8,
                           U8 bit_start_u8,
                           U8 length_u8,
                           U8 data_u8 )
{
    /*    010 value to write (data_u8)
     76543210 bit numbers
        bbb   args: bit_start_u8 = 4, data_size_u8 = 3
     00011100 mask byte
     10101111 original value (sample)
     10100011 original & ~mask
     10101011 masked | value */

    U8 register_value_u8;
    U8 bit_mask_u8;

    /* Read corresponding register */
    if ( FALSE == read_single( device_addr_u8, register_addr_u8, &register_value_u8 ) )
    {
        /* Return FALSE in case of error in read cycle */
        return FALSE;
    }

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
    if ( FALSE == write_single( device_addr_u8, register_addr_u8, register_value_u8 ) )
    {
        /* Return FALSE in case of error in write cycle */
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_I2C_C::read_modify_write_bit( U8      device_addr_u8,
                                      U8      register_addr_u8,
                                      U8      bit_position_u8,
                                      BOOLEAN bit_value_b )
{
    BOOLEAN status_b          = FALSE;
    U8      register_value_u8 = 0U;

    status_b = read_single( device_addr_u8,
                                   register_addr_u8,
                                   &register_value_u8 );

    /* Check for error in read cycle */
    if ( FALSE != status_b )
    {
        register_value_u8 = ( bit_value_b != 0 ) ? ( register_value_u8 | ( 1 << bit_position_u8 ) ) : ( register_value_u8 & ~( 1 << bit_position_u8 ) );

        status_b = write_single( device_addr_u8,
                                 register_addr_u8,
                                 register_value_u8 );
    }

    return status_b;
}

BOOLEAN DD_I2C_C::read_modify_write_mask( U8 device_addr_u8,
                                       U8 register_addr_u8,
                                       U8 mask_u8,
                                       U8 value_u8 )
{
    U8 register_value_u8 = 0U;

    /* Read original register content */
    if( FALSE == read_single( device_addr_u8, register_addr_u8, &register_value_u8 ) )
    {
        return FALSE;
    }

    /* Zero-out the register portions to be written */
    register_value_u8 &= mask_u8;

    /* Update register content */
    if( FALSE == write_single( device_addr_u8, register_addr_u8, ( register_value_u8 | value_u8 ) ) )
    {
        return FALSE;
    }

    return TRUE;
}

BOOLEAN DD_I2C_C::handle_error( DD_I2C_ERROR       error_e,
                                DD_I2C_ACCESS_TYPE access_type_e,
                                U8                 device_addr_u8,
                                U8                 register_addr_u8 )
{
    if ( DD_I2C_ERROR_OK != error_e )
    {
        ESP_LOGD( DD_I2C_LOG_MSG_TAG, "current_error_idx_u8 %i, last_error_idx_u8: %i", dd_i2c_error_s.current_error_idx_u8,
                                                                                        dd_i2c_error_s.last_error_idx_u8 );

        dd_i2c_error_s.is_error_present_b                                                  = TRUE;
        dd_i2c_error_s.error_info_vs[dd_i2c_error_s.current_error_idx_u8].error_e          = error_e;
        dd_i2c_error_s.error_info_vs[dd_i2c_error_s.current_error_idx_u8].access_type_e    = access_type_e;
        dd_i2c_error_s.error_info_vs[dd_i2c_error_s.current_error_idx_u8].device_addr_u8   = device_addr_u8;
        dd_i2c_error_s.error_info_vs[dd_i2c_error_s.current_error_idx_u8].register_addr_u8 = register_addr_u8;
        /* TODO: dd_i2c_error_s.error_info_vs[dd_i2c_error_s.current_error_idx_u8].time_stamp_f32 = current_time_stamp */

        ESP_LOGE( DD_I2C_LOG_MSG_TAG, "[%i]: %s, AccType: %i, DevAddr: 0x%x @ RegAddr: 0x%x", dd_i2c_error_s.current_error_idx_u8,
                                                                                              esp_err_to_name( error_e ),
                                                                                              access_type_e,
                                                                                              device_addr_u8,
                                                                                              register_addr_u8 );

        /* Get next index of ring buffer */
        dd_i2c_error_s.last_error_idx_u8 = dd_i2c_error_s.current_error_idx_u8++;
        dd_i2c_error_s.current_error_idx_u8 %= DD_I2C_ERROR_BUFFER_LENGTH;
    }
    else
    {
        dd_i2c_error_s.is_error_present_b = FALSE;
    }

    return ( !dd_i2c_error_s.is_error_present_b );
}
