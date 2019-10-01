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
PRIVATE BOOLEAN dd_i2c_byte_write( U8 device_addr_u8,
                                   U8 register_addr_u8,
                                   U8 data_u8 );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/




/** Initialize I2C0
 */
esp_err_t dd_i2c_init()
{
    esp_err_t status_t = ESP_FAIL;

    i2c_config_t i2c_if_config_s;
    i2c_if_config_s.mode = I2C_MODE_MASTER;
    i2c_if_config_s.sda_io_num = 21;
    i2c_if_config_s.sda_pullup_en = GPIO_PULLUP_DISABLE;
    i2c_if_config_s.scl_io_num = 22;
    i2c_if_config_s.scl_pullup_en = GPIO_PULLUP_DISABLE;
    i2c_if_config_s.master.clk_speed = 100000;
    i2c_param_config(DD_I2C_IF_NUM, &i2c_if_config_s);
    status_t = i2c_driver_install( DD_I2C_IF_NUM,
                                   i2c_if_config_s.mode,
                                   0,
                                   0,
                                   0 );

    while(1)
    {
        for (int i = 3; i >= 0; i--) {
            printf("Restarting in %d seconds...\n", i);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        dd_i2c_byte_write(0x10, 0x20, 128);
    }

    return status_t;
}


///** Default timeout value for read operations.
// */
//U16 dd_i2c_readTimeout = I2CDEV_DEFAULT_READ_TIMEOUT;
///** Read a single bit from an 8-bit device register.
// * @param devAddr I2C slave device address
// * @param regAddr Register regAddr to read from
// * @param bitNum Bit position to read (0-7)
// * @param data Container for single bit value
// * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in dd_i2c_readTimeout)
// * @return Status of read operation (true = success)
// */
//U8 dd_i2c_readBit(U8 devAddr, U8 regAddr, U8 bitNum, U8 *data, U16 timeout) {
//
//
//    U8 b;
//    U8 count = readByte(devAddr, regAddr, &b, timeout);
//    *data = b & (1 << bitNum);
//    return count;
//}
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
//
///** Read single byte from an 8-bit device register.
// * @param devAddr I2C slave device address
// * @param regAddr Register regAddr to read from
// * @param data Container for byte value read from device
// * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in dd_i2c_readTimeout)
// * @return Status of read operation (true = success)
// */
//U8 dd_i2c_readByte(U8 devAddr, U8 regAddr, U8 *data, U16 timeout) {
//    return readBytes(devAddr, regAddr, 1, data, timeout);
//}
//
///** Read multiple bytes from an 8-bit device register.
// * @param devAddr I2C slave device address
// * @param regAddr First register regAddr to read from
// * @param length Number of bytes to read
// * @param data Buffer to store read data in
// * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in dd_i2c_readTimeout)
// * @return I2C_TransferReturn_TypeDef http://downloads.energymicro.com/documentation/doxygen/group__I2C.html
// */
//U8 dd_i2c_readBytes(U8 devAddr, U8 regAddr, U8 length, U8 *data, U16 timeout) {
//    i2c_cmd_handle_t cmd;
//    dd_i2c_register_select(devAddr, regAddr);
//
//    cmd = i2c_cmd_link_create();
//    ESP_ERROR_CHECK(i2c_master_start(cmd));
//    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_READ, 1));
//
//    if(length>1)
//        ESP_ERROR_CHECK(i2c_master_read(cmd, data, length-1, 0));
//
//    ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+length-1, 1));
//
//    ESP_ERROR_CHECK(i2c_master_stop(cmd));
//    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM, cmd, 1000/portTICK_PERIOD_MS));
//    i2c_cmd_link_delete(cmd);
//
//    return length;
//}
//
//BOOLEAN dd_i2c_writeWord(U8 devAddr, U8 regAddr, U16 data){
//
//    U8 data1[] = {(U8)(data>>8), (U8)(data & 0xff)};
//    U8 data2[] = {(U8)(data & 0xff), (U8)(data>>8)};
//    writeBytes(devAddr, regAddr, 2, data1);
//
//    return true;
//}
//
//PRIVATE void dd_i2c_register_select( U8 device_addr_u8,
//                                     U8 register_addr_u8 )
//{
//    i2c_cmd_handle_t command_e;
//
//    command_e = i2c_cmd_link_create();
//
//    ESP_ERROR_CHECK( i2c_master_start(command_e) );
//
//    ESP_ERROR_CHECK( i2c_master_write_byte( command_e, (device_addr_u8 << 1) | I2C_MASTER_WRITE, TRUE) );
//
//    ESP_ERROR_CHECK( i2c_master_write_byte(command_e, register_addr_u8, TRUE) );
//
//    ESP_ERROR_CHECK( i2c_master_stop(command_e) );
//
//    ESP_ERROR_CHECK( i2c_master_cmd_begin(I2C_NUM, command_e, 1000/portTICK_PERIOD_MS) );
//
//    i2c_cmd_link_delete(command_e);
//}
//
///** write a single bit in an 8-bit device register.
// * @param devAddr I2C slave device address
// * @param regAddr Register regAddr to write to
// * @param bitNum Bit position to write (0-7)
// * @param value New bit value to write
// * @return Status of operation (true = success)
// */
//PRIVATE BOOLEAN dd_i2c_bit_write(U8 devAddr, U8 regAddr, U8 bitNum, U8 data) {
//    U8 b;
//    readByte(devAddr, regAddr, &b);
//    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
//    return writeByte(devAddr, regAddr, b);
//}
//
///** Write multiple bits in an 8-bit device register.
// * @param devAddr I2C slave device address
// * @param regAddr Register regAddr to write to
// * @param bitStart First bit position to write (0-7)
// * @param length Number of bits to write (not more than 8)
// * @param data Right-aligned value to write
// * @return Status of operation (true = success)
// */
//PRIVATE BOOLEAN dd_i2c_bits_write(U8 devAddr, U8 regAddr, U8 bitStart, U8 length, U8 data) {
//    //      010 value to write
//    // 76543210 bit numbers
//    //    xxx   args: bitStart=4, length=3
//    // 00011100 mask byte
//    // 10101111 original value (sample)
//    // 10100011 original & ~mask
//    // 10101011 masked | value
//    U8 b = 0;
//    if (readByte(devAddr, regAddr, &b) != 0) {
//        U8 mask = ((1 << length) - 1) << (bitStart - length + 1);
//        data <<= (bitStart - length + 1); // shift data into correct position
//        data &= mask; // zero all non-important bits in data
//        b &= ~(mask); // zero all important bits in existing byte
//        b |= data; // combine data with existing byte
//        return writeByte(devAddr, regAddr, b);
//    } else {
//        return false;
//    }
//}
//
///** Write single byte to an 8-bit device register.
// * @param devAddr I2C slave device address
// * @param regAddr Register address to write to
// * @param data New byte value to write
// * @return Status of operation (true = success)
// */
PRIVATE BOOLEAN dd_i2c_byte_write( U8 device_addr_u8,
                                   U8 register_addr_u8,
                                   U8 data_u8 )
{

    i2c_cmd_handle_t p_i2c_command_t;

    p_i2c_command_t = i2c_cmd_link_create();

    ESP_ERROR_CHECK( i2c_master_start(p_i2c_command_t) );

    ESP_ERROR_CHECK( i2c_master_write_byte(p_i2c_command_t, (device_addr_u8 << 1) | I2C_MASTER_WRITE, FALSE));

    ESP_ERROR_CHECK(i2c_master_write_byte(p_i2c_command_t, register_addr_u8, FALSE));

    ESP_ERROR_CHECK(i2c_master_write_byte(p_i2c_command_t, data_u8, FALSE));

    ESP_ERROR_CHECK(i2c_master_stop(p_i2c_command_t));

    ESP_ERROR_CHECK(i2c_master_cmd_begin(DD_I2C_IF_NUM, p_i2c_command_t, 1000/portTICK_PERIOD_MS));

    i2c_cmd_link_delete(p_i2c_command_t);

    return true;
}
//
///** Write single byte to an 8-bit device register.
// * @param devAddr I2C slave device address
// * @param regAddr Register address to write to
// * @param length Number of bytes to write
// * @param data Array of bytes to write
// * @return Status of operation (true = success)
// */
//PRIVATE BOOLEAN dd_i2c_bytes_write(U8 devAddr, U8 regAddr, U8 length, U8 *data){
//    i2c_cmd_handle_t cmd;
//
//    cmd = i2c_cmd_link_create();
//    ESP_ERROR_CHECK(i2c_master_start(cmd));
//    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, TRUE));
//    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, regAddr, 1));
//    ESP_ERROR_CHECK(i2c_master_write(cmd, data, length-1, 0));
//    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, data[length-1], 1));
//    ESP_ERROR_CHECK(i2c_master_stop(cmd));
//    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM, cmd, 1000/portTICK_PERIOD_MS));
//    i2c_cmd_link_delete(cmd);
//    return true;
//}
//
//
///**
// * read word
// * @param devAddr
// * @param regAddr
// * @param data
// * @param timeout
// * @return
// */
//PRIVATE U8 dd_i2c_word_read( U8 devAddr,
//                             U8 regAddr,
//                             U16 *data,
//                             U16 timeout )
//{
//    U8 msb[2] = {0,0};
//
//    readBytes( devAddr, regAddr, 2, msb );
//    *data = (U16) ( (msb[0] << 8) | msb[1] );
//
//    return 0;
//}

























