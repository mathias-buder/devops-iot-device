/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_i2c.h
        @details I2C device driver implementation

        (c) SEWELA 2020

*********************************************************************/
#ifndef DD_I2C_H_
#define DD_I2C_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

#include "../../types.h"


/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
#define DD_I2C_LOG_MSG_TAG            "DD_I2C"                      /**< @details Domain log message tag string */
#define DD_I2C_OPERATION_MODE         I2C_MODE_MASTER               /**< @details I2C transmission mode */
#define DD_I2C_PORT_NUM               I2C_NUM_0                     /**< @details I2C module number */
#define DD_I2C_SDA_IO_NUM             GPIO_NUM_21                   /**< @details SDA GPIO pin number */
#define DD_I2C_SCL_IO_NUM             GPIO_NUM_22                   /**< @details SCL GPIO pin number */
#define DD_I2C_SDA_PULLUP_CFG         GPIO_PULLUP_ENABLE            /**< @details SDA GPIO pin pullup resistor configuration */
#define DD_I2C_SCL_PULLUP_CFG         GPIO_PULLUP_ENABLE            /**< @details SCL GPIO pin pullup resistor configuration */
#define DD_I2C_SCL_FREQ               ( 400000 )                    /**< @details Transmission clock frequency @unit Hz */
#define DD_I2C_BUS_BUSY_TIME_OUT      ( 1000 / portTICK_PERIOD_MS ) /**< @details Time out in case I2C bus is busy @unit sec */
#define DD_I2C_ERROR_BUFFER_LENGTH    ( 10U )                       /**< @details Number of I2C errors that are stored inside DD_I2C_ERROR_INFO_TYPE */


/*************************************************************
*                      ENUMERATORS                           *
*************************************************************/

/**
 * @details I2C error types
 */
typedef enum DD_I2C_ERROR_TAG
{
    DD_I2C_ERROR_FAIL         = ESP_FAIL,             /*!< Generic esp_err_t code indicating failure */
    DD_I2C_ERROR_OK           = ESP_OK,               /*!< esp_err_t value indicating success (no error) */
    DD_I2C_ERROR_INVALID_SIZE = ESP_ERR_INVALID_SIZE, /** @details Acceleration raw data */
    DD_I2C_ERROR_INVALID_ARG  = ESP_ERR_INVALID_ARG,  /** @details Acceleration raw data */
    DD_I2C_ERROR_TIMEOUT      = ESP_ERR_TIMEOUT,      /*!< Operation timed out */
    DD_I2C_ERROR_SIZE
} DD_I2C_ERROR;

/**
 * @details I2C access types
 */
typedef enum DD_I2C_ACCESS_TYPE_TAG
{
    DD_I2C_ACCESS_TYPE_RD_SINGLE = 0U, /**< @details Read single byte */
    DD_I2C_ACCESS_TYPE_RD_BURST,       /**< @details Read multiple bytes */
    DD_I2C_ACCESS_TYPE_WD_SINGLE,      /**< @details Write single byte */
    DD_I2C_ACCESS_TYPE_WD_BURST        /**< @details Write multiple bytes */
} DD_I2C_ACCESS_TYPE;



/*************************************************************
*      STRUCTURES                                            *
*************************************************************/
/**
 * @details I2C driver error information structure (for each error)
 */
typedef struct DD_I2C_ERROR_INFO_TYPE_TAG
{
    DD_I2C_ERROR       error_e;          /**< @details Current I2C error type */
    DD_I2C_ACCESS_TYPE access_type_e;    /**< @details Current I2C access type */
    U8                 device_addr_u8;   /**< @details I2C address of device that is currently accessed */
    U8                 register_addr_u8; /**< @details Register address of device that is currently accessed */
    F32                time_stamp_f32;   /**< @details Time stamp when error occurred */
} DD_I2C_ERROR_INFO_TYPE;

/**
 * @details I2C driver error data structure
 */
typedef struct DD_I2C_ERROR_DATA_TYPE_TAG
{
    DD_I2C_ERROR_INFO_TYPE error_info_vs[DD_I2C_ERROR_BUFFER_LENGTH]; /**< @details List of all I2C errors */
    U8                     current_idx_u8;                            /**< @details Index of current I2C error */
    U8                     last_idx_u8;                               /**< @details Index of last/previous I2C error */
    BOOLEAN                is_present_b;                              /**< @details Flag to indicate whether I2C error is currently present or not */
} DD_I2C_ERROR_DATA_TYPE;

/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/
class DD_I2C_C {

  private:

    static DD_I2C_ERROR_DATA_TYPE error_s;    /**< @details Structure to store I2C errors */

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
     * @details This function initialized the I2C interface
     */
    static DD_I2C_ERROR_DATA_TYPE* init( void );

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

    /**
     * @details This function returns a pointer to the current I2C error
     * @return pointer to the I2C error of type DD_I2C_ERROR_INFO_TYPE
     */
    static DD_I2C_ERROR_INFO_TYPE* get_last_error( void );
};

#endif /* DD_I2C_H_ */
