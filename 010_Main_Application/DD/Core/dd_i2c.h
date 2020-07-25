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

/**
 * @file dd_i2c.h
 * @details Some detailed description
 */

#ifndef DD_I2C_H_
#define DD_I2C_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <driver/i2c.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <esp_err.h>
#include "../Config/dd_i2c_cfg.h"
#include "../Config/dd_types_cfg.h"

#include "dd_types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

class DD_I2C_C
{
  private:
    /**
     * @details This function performs a read/write test on the specified I2C device to make sure the
     * low-level interface is working correctly.
     * @param[in] error_t I2C error code
     * @return TRUE if NO error is present
     */
    static BOOLEAN handle_error( DD_I2C_ERROR       error_e,
                                 DD_I2C_ACCESS_TYPE access_type_e,
                                 U8                 device_addr_u8,
                                 U8                 register_addr_u8 );

  public:
    /**
     * @details Default constructor
     */
    DD_I2C_C();

    /**
     * @details Default destructor
     */
    ~DD_I2C_C();

    /**
     * @details This function initialized the I2C interface
     */
    static void init( void );

    /**
     * @details This function reads the content of an 8-bit register.
     * @param[in] device_addr_u8 I2C slave device address
     * @param[in] register_addr_u8 Register address to read from
     * @param[in] data_u8 Pointer to storage of register content
     * @return always TRUE ( Error handling currently done using macro ESP_ERROR_CHECK( ) )
     */
    static BOOLEAN read_single( U8  device_addr_u8,
                                U8  register_addr_u8,
                                U8* data_u8 );

    /**
     * @details This function reads N-Byte of data starting at the address given by register_addr_u8.
     * @param[in] device_addr_u8 I2C slave device address
     * @param[in] register_addr_u8 Register address to read from
     * @param[in] data_u8 Pointer to storage of register content
     * @param[in] data_size_u8 Size in byte of data_u8
     * @return always TRUE ( Error handling currently done using macro ESP_ERROR_CHECK( ) )
     */
    static BOOLEAN read_burst( U8  device_addr_u8,
                               U8  register_addr_u8,
                               U8* p_data_u8,
                               U8  data_size_u8 );

    /**
     * @details This function reads the value of an bit within the register given by register_addr_u8
     * @param[in] device_addr_u8 I2C slave device address
     * @param[in] register_addr_u8 Register address to read from
     * @param[in] bit_position_u8 Position of requested bit within the register given by register_addr_u8
     * @param[in] bit_value_b Pointer to the bit value
     * @return always TRUE ( Error handling currently done using macro ESP_ERROR_CHECK( ) )
     */
    static BOOLEAN read_bit( U8       device_addr_u8,
                             U8       register_addr_u8,
                             U8       bit_position_u8,
                             BOOLEAN* bit_value_b );

    /**
     * @details This function reads the value of N-bits within the register given by register_addr_u8
     * @param[in] device_addr_u8 I2C slave device address
     * @param[in] register_addr_u8 Register address to read from
     * @param[in] start_bit_u8 Start position of first bit
     * @param[in] length_u8 Total number of bits to be read
     * @param[in] p_data_u8 Pointer to the storage of requested bits to be read
     * @return always TRUE ( Error handling currently done using macro ESP_ERROR_CHECK( ) )
     */
    static BOOLEAN read_bits( U8  device_addr_u8,
                              U8  regisetr_addr_u8,
                              U8  start_bit_u8,
                              U8  length_u8,
                              U8* p_data_u8 );

    /**
     * @details This function writes the content of an 8-bit register.
     * @param[in] device_addr_u8 I2C slave device address
     * @param[in] register_addr_u8 Register address to read from
     * @param[in] data_u8 Pointer to the data to be written to the register given by register_addr_u8
     * @return always TRUE ( Error handling currently done using macro ESP_ERROR_CHECK( ) )
     */
    static BOOLEAN write_single( U8 device_addr_u8,
                                 U8 register_addr_u8,
                                 U8 data_u8 );

    /**
     * @details This function reads N-Byte of data starting at the address given by register_addr_u8.
     * @param[in] device_addr_u8 I2C slave device address
     * @param[in] register_addr_u8 Register address to read from
     * @param[in] data_u8 Pointer to the data to be written starting a address given by register_addr_u8
     * @param[in] data_size_u8 Size in byte of data_u8
     * @return always TRUE ( Error handling currently done using macro ESP_ERROR_CHECK( ) )
     */
    static BOOLEAN write_burst( U8  device_addr_u8,
                                U8  register_addr_u8,
                                U8* p_data_u8,
                                U8  data_size_u8 );

    /**
     * @details This function writes the value of N-bits within the register given by register_addr_u8
     * @param[in] device_addr_u8 I2C slave device address
     * @param[in] register_addr_u8 Register address to read from
     * @param[in] start_bit_u8 Start position of first bit
     * @param[in] length_u8 Total number of bits to be written
     * @param[in] data_u8 Bits to be written to the register given by register_addr_u8
     * @return always TRUE ( Error handling currently done using macro ESP_ERROR_CHECK( ) )
     */
    static BOOLEAN write_bits( U8 device_addr_u8,
                               U8 register_addr_u8,
                               U8 bit_start_u8,
                               U8 length_u8,
                               U8 data_u8 );

    /**
     * @details This function reads the an register given by register_addr_u8 and modifies the bit given by
     * bit_position_u8 with the value given by bit_value_b
     * @param[in] device_addr_u8 I2C slave device address
     * @param[in] register_addr_u8 Register address to read from
     * @param[in] bit_position_u8 Position of the bit to be modified
     * @param[in] bit_value_b Value to be written to the bit given by bit_position_u8
     * @return always TRUE ( Error handling currently done using macro ESP_ERROR_CHECK( ) )
     */
    static BOOLEAN read_modify_write_bit( U8      device_addr_u8,
                                          U8      register_addr_u8,
                                          U8      bit_position_u8,
                                          BOOLEAN bit_value_b );

    /**
     * @details This function reads the an register given by register_addr_u8 and modifies the bit given by
     * bit_position_u8 with the value given by bit_value_b
     * @param[in] device_addr_u8 I2C slave device address
     * @param[in] register_addr_u8 Register address to read from
     * @param[in] mask_u8 Bitmask to mask out the required bit-region
     * @param[in] value_u8 Value to be written to the registers bit-region masked out with mask_u8
     * @return always TRUE ( Error handling currently done using macro ESP_ERROR_CHECK( ) )
     */
    static BOOLEAN read_modify_write_mask( U8 device_addr_u8,
                                           U8 register_addr_u8,
                                           U8 mask_u8,
                                           U8 value_u8 );
};

#endif /* DD_I2C_H_ */
