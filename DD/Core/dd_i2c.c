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

#include "dd_i2c.h"
#include "../../types.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/


PRIVATE void dd_i2c_register_select( U8 device_addr_u8,
                                     U8 register_addr_u8 );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/



/** Initialize I2C0
 */
BOOLEAN dd_i2c_init()
{
    esp_err_t     status_t = ESP_FAIL;
    i2c_config_t  dd_i2c_if_config_s;


    dd_i2c_if_config_s.mode             = DD_I2C_OPERATION_MODE;
    dd_i2c_if_config_s.sda_io_num       = DD_I2C_SDA_IO_NUM;
    dd_i2c_if_config_s.scl_io_num       = DD_I2C_SCL_IO_NUM;
    dd_i2c_if_config_s.sda_pullup_en    = DD_I2C_SDA_PULLUP_CFG;
    dd_i2c_if_config_s.scl_pullup_en    = DD_I2C_SCL_PULLUP_CFG;
    dd_i2c_if_config_s.master.clk_speed = DD_I2C_SCL_FREQ;

    i2c_param_config( DD_I2C_PORT_NUM,
                      &dd_i2c_if_config_s );

    status_t = i2c_driver_install( DD_I2C_PORT_NUM,
                                   dd_i2c_if_config_s.mode,
                                   0,
                                   0,
                                   0 );

    printf("I2C basic driver initialized with status: %s.\n", esp_err_to_name(status_t) );

    return ( status_t != ESP_OK ? FALSE : TRUE );
}





/** Read multiple bytes from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in dd_i2c_readTimeout)
 * @return I2C_TransferReturn_TypeDef http://downloads.energymicro.com/documentation/doxygen/group__I2C.html
 */
U8 dd_i2c_read_burst( U8  device_addr_u8,
                      U8  register_addr_u8,
                      U8* p_data_u8,
                      U8  data_size_u8 )
{
    i2c_cmd_handle_t i2c_command_t;

    /* Select appropriate I2C register */
    dd_i2c_register_select( device_addr_u8,
                            register_addr_u8 );

    i2c_command_t = i2c_cmd_link_create();

    ESP_ERROR_CHECK( i2c_master_start( i2c_command_t ) );

    ESP_ERROR_CHECK( i2c_master_write_byte(  i2c_command_t,
                                            (device_addr_u8 << 1) | I2C_MASTER_READ,
                                             DD_I2C_SCL_MASTER_ACK_CHECK ) );

//    if( data_size_u8 > 1 )
//    {
        ESP_ERROR_CHECK( i2c_master_read( i2c_command_t,
                                          p_data_u8,
                                          data_size_u8,
                                          DD_I2C_SCL_MASTER_ACK_CHECK ) );
//    }
//    else
//    {
//        ESP_ERROR_CHECK( i2c_master_read_byte( i2c_command_t,
//                                               p_data_u8,
//                                               DD_I2C_SCL_MASTER_ACK_CHECK ) );
//    }

    ESP_ERROR_CHECK( i2c_master_stop( i2c_command_t ) );

    ESP_ERROR_CHECK( i2c_master_cmd_begin(  DD_I2C_PORT_NUM,
                                            i2c_command_t,
                                            DD_I2C_BUS_BUSY_TIME_OUT ) );

    i2c_cmd_link_delete( i2c_command_t );

    return data_size_u8;
}


/** Read a single bit from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in dd_i2c_readTimeout)
 * @return Status of read operation (true = success)
 */
BOOLEAN dd_i2c_read_bit( U8  device_addr_u8,
                         U8  register_addr_u8,
                         U8  bit_position_u8 )
{
    U8 register_value_u8;

    dd_i2c_read_burst(device_addr_u8, register_addr_u8, &register_value_u8, sizeof(register_value_u8) );

    return ( register_value_u8 & (1 << bit_position_u8) );
}

/** write a single bit in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
BOOLEAN dd_i2c_read_modify_write( U8 device_addr_u8,
                                  U8 register_addr_u8,
                                  U8 bit_position_u8,
                                  BOOLEAN bit_value_b )
{
    BOOLEAN status_b            = FALSE;
    U8      register_value_u8   = 0U;

    dd_i2c_read_burst( device_addr_u8,
                       register_addr_u8,
                       &register_value_u8,
                       sizeof(register_value_u8) );

    register_value_u8 = ( bit_value_b != 0 ) ? ( register_value_u8 | (1 << bit_position_u8) ) : ( register_value_u8 & ~(1 << bit_position_u8) );

    status_b = dd_i2c_write_single( device_addr_u8,
                                    register_addr_u8,
                                    register_value_u8 );

//    status_b = dd_i2c_write_burst( device_addr_u8,
//                                   register_addr_u8,
//                                   &register_value_u8,
//                                   sizeof(register_value_u8) );

    return status_b;
}


PRIVATE void dd_i2c_register_select( U8 device_addr_u8,
                                     U8 register_addr_u8 )
{
    i2c_cmd_handle_t i2c_command_t;

    i2c_command_t = i2c_cmd_link_create();

    ESP_ERROR_CHECK( i2c_master_start( i2c_command_t) );

    ESP_ERROR_CHECK( i2c_master_write_byte(  i2c_command_t,
                                            (device_addr_u8 << 1) | I2C_MASTER_WRITE,
                                             DD_I2C_SCL_MASTER_ACK_CHECK) );

    ESP_ERROR_CHECK( i2c_master_write_byte( i2c_command_t,
                                            register_addr_u8,
                                            DD_I2C_SCL_MASTER_ACK_CHECK) );

    ESP_ERROR_CHECK( i2c_master_stop( i2c_command_t ) );

    ESP_ERROR_CHECK( i2c_master_cmd_begin(  DD_I2C_PORT_NUM,
                                            i2c_command_t,
                                           (1000/portTICK_PERIOD_MS) ) );

    i2c_cmd_link_delete( i2c_command_t );
}




/** Write single byte to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New byte value to write
 * @return Status of operation (true = success)
 */
BOOLEAN dd_i2c_write_single( U8 device_addr_u8,
                             U8 register_addr_u8,
                             U8 data_u8 )
{
    i2c_cmd_handle_t i2c_command_t;

    /* Get I2C command handle */
    i2c_command_t = i2c_cmd_link_create();

    /* Create command sequence */
    ESP_ERROR_CHECK( i2c_master_start(i2c_command_t) );

    /* Add appropriate device address to command sequence */
    ESP_ERROR_CHECK( i2c_master_write_byte(  i2c_command_t,
                                            (device_addr_u8 << 1) | I2C_MASTER_WRITE,
                                             DD_I2C_SCL_MASTER_ACK_CHECK) );

    /* Added appropriate I2C device register to command sequence */
    ESP_ERROR_CHECK( i2c_master_write_byte( i2c_command_t,
                                            register_addr_u8,
                                            DD_I2C_SCL_MASTER_ACK_CHECK) );

    /* Add one bytes of data to command sequence */
    ESP_ERROR_CHECK( i2c_master_write_byte( i2c_command_t,
                                            data_u8,
                                            DD_I2C_SCL_MASTER_ACK_CHECK) );

    /* Finish I2C transmission command sequence */
    ESP_ERROR_CHECK( i2c_master_stop(i2c_command_t) );

    /* Start command sequence transmission */
    ESP_ERROR_CHECK( i2c_master_cmd_begin( DD_I2C_PORT_NUM,
                                           i2c_command_t,
                                           DD_I2C_BUS_BUSY_TIME_OUT ) );

    /* Delete command sequence and release/return I2C resource */
    i2c_cmd_link_delete( i2c_command_t );

    return true;
}

/** Write single byte to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param length Number of bytes to write
 * @param data Array of bytes to write
 * @return Status of operation (true = success)
 */
BOOLEAN dd_i2c_write_burst( U8  device_addr_u8,
                            U8  register_addr_u8,
                            U8 *p_data_u8,
                            U8  data_size_u8 )
{
    i2c_cmd_handle_t i2c_command_t;

    /* Get I2C command handle */
    i2c_command_t = i2c_cmd_link_create();

    /* Create command sequence */
    ESP_ERROR_CHECK( i2c_master_start( i2c_command_t ) );

    /* Add appropriate device address to command sequence */
    ESP_ERROR_CHECK( i2c_master_write_byte(  i2c_command_t,
                                            (device_addr_u8 << 1) | I2C_MASTER_WRITE,
                                             DD_I2C_SCL_MASTER_ACK_CHECK) );

    /* Added appropriate I2C device register to command sequence */
    ESP_ERROR_CHECK( i2c_master_write_byte( i2c_command_t,
                                            register_addr_u8,
                                            DD_I2C_SCL_MASTER_ACK_CHECK) );

    /* Add data_size_u8 bytes of data to command sequence */
    ESP_ERROR_CHECK( i2c_master_write( i2c_command_t,
                                       p_data_u8,
                                       data_size_u8,
                                       DD_I2C_SCL_MASTER_ACK_CHECK) );

    /* Transmit (data_size_u8 - 1) bytes of data with DD_I2C_SCL_MASTER_ACK_CHECK set to FALSE */
//    ESP_ERROR_CHECK( i2c_master_write(  i2c_command_t,
//                                        p_data_u8,
//                                       (data_size_u8 - 1),
//                                        FALSE) );

//    ESP_ERROR_CHECK( i2c_master_write_byte( i2c_command_t,
//                                            p_data_u8[data_size_u8 - 1],
//                                            DD_I2C_SCL_MASTER_ACK_CHECK) );

    /* Finish I2C transmission command sequence */
    ESP_ERROR_CHECK( i2c_master_stop( i2c_command_t) );

    /* Start command sequence transmission */
    ESP_ERROR_CHECK( i2c_master_cmd_begin( DD_I2C_PORT_NUM,
                                           i2c_command_t,
                                           DD_I2C_BUS_BUSY_TIME_OUT ) );

    /* Delete command sequence and release/return I2C resource */
    i2c_cmd_link_delete( i2c_command_t );

    return true;
}






























/** Default timeout value for read operations.
 */
//U16 dd_i2c_readTimeout = I2CDEV_DEFAULT_READ_TIMEOUT;

//
///** Read multiple bits from an 8-bit device register.
// * @param devAddr I2C slave device address
// * @param regAddr Register regAddr to read from
// * @param bitStart First bit position to read (0-7)
// * @param length Number of bits to read (not more than 8)
// * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
// * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in dd_i2c_readTimeout)
// * @return Status of read operation (true = success)
// */
//PRIVATE U8 dd_i2c_bits_read( U8 device_addr_u8,
//                             U8 regisetr_addr_u8,
//                             U8 start_bit_u8,
//                             U8 length_u8,
//                             U8* data,
//                             U16 timeout )
//{
//    // 01101001 read byte
//    // 76543210 bit numbers
//    //    xxx   args: bitStart=4, length=3
//    //    010   masked
//    //   -> 010 shifted
//    U8 count, b;
//    if ( (count = readByte(device_addr_u8, regisetr_addr_u8, &b, timeout) ) != 0 )
//    {
//        U8 mask = ((1 << length_u8) - 1) << (start_bit_u8 - length_u8 + 1);
//
//        b &= mask;
//
//        b >>= (start_bit_u8 - length_u8 + 1);
//
//        *data = b;
//    }
//    return count;
//}







///** Write multiple bits in an 8-bit device register.
// * @param devAddr I2C slave device address
// * @param regAddr Register regAddr to write to
// * @param bitStart First bit position to write (0-7)
// * @param length Number of bits to write (not more than 8)
// * @param data Right-aligned value to write
// * @return Status of operation (true = success)
// */
//PRIVATE BOOLEAN dd_i2c_bits_write( U8 device_addr_u8,
//                                   U8 register_addr_u8,
//                                   U8 bit_start_u8,
//                                   U8 data_u8,
//                                   U8 data_size_u8 )
//{
//    /*    010 value to write
//     76543210 bit numbers
//        xxx   args: bitStart=4, length=3
//     00011100 mask byte
//     10101111 original value (sample)
//     10100011 original & ~mask
//     10101011 masked | value     */
//
//    U8 b = 0;
//
//    if (dd_i2c_read_burst(device_addr_u8, register_addr_u8, &b) != 0)
//    {
//        U8 mask = ((1 << data_size_u8) - 1) << (bit_start_u8 - data_size_u8 + 1);
//        data_u8 <<= (bit_start_u8 - data_size_u8 + 1); // shift data_u8 into correct position
//        data_u8 &= mask; // zero all non-important bits in data
//        b &= ~(mask); // zero all important bits in existing byte
//        b |= data_u8; // combine data_u8 with existing byte
//        return dd_i2c_write_single(device_addr_u8, register_addr_u8, b);
//    }
//    else
//    {
//        return FALSE;
//    }
//}



//BOOLEAN dd_i2c_writeWord( U8  device_addr_u8,
//                          U8  register_addr_u8,
//                          U16 data_u16 )
//{
//
//    U8 data1[] = {(U8)(data_u16 >> 8), (U8)(data_u16 & 0xFF)};
//
//    U8 data2[] = {(U8)(data_u16 & 0xff), (U8)(data_u16 >> 8)};
//
//    writeBytes(device_addr_u8, register_addr_u8, 2, data1);
//
//    return true;
//}




















