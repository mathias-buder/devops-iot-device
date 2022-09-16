/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_tmp_102.h
        @details TMP-102 device driver implementation

*********************************************************************/
#ifndef DD_TMP_102_H_
#define DD_TMP_102_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"

/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
#define DD_TMP_102_LOG_MSG_TAG          "DD_TMP_102"   /**< @details Domain log message tag string */

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
/* Pointer Register Byte */
#define DD_TMP_102_TEMP                 0x00U       /**< Temperature Register (Read Only) */
#define DD_TMP_102_CONFIG               0x01U       /**< Configuration Register (Read/Write) */
#define DD_TMP_102_TEMP_LOW             0x02U       /**< T_LOW Register (Read/Write) */
#define DD_TMP_102_TEMP_HIGH            0x03U       /**< T_HIGH Register (Read/Write) */

/* Configuration register */
#define DD_TMP_102_CONFIG_OS_ENABLE     0x8000U
#define DD_TMP_102_CONFIG_OS_DISABLE    0x0000U
#define DD_TMP_102_CONFIG_F_1FAULT      0x0000U
#define DD_TMP_102_CONFIG_F_2FAULTS     0x0800U
#define DD_TMP_102_CONFIG_F_4FAULTS     0x1000U
#define DD_TMP_102_CONFIG_F_6FAULTS     0x1800U
#define DD_TMP_102_CONFIG_POL_LOW       0x0000U
#define DD_TMP_102_CONFIG_POL_HIGH      0x0400U
#define DD_TMP_102_CONFIG_TM_COMP       0x0000U
#define DD_TMP_102_CONFIG_TM_TSTAT      0x0200U
#define DD_TMP_102_CONFIG_SD_CC         0x0000U
#define DD_TMP_102_CONFIG_SD_SD         0x0100U
#define DD_TMP_102_CONFIG_CR_0P25HZ     0x0000U
#define DD_TMP_102_CONFIG_CR_1HZ        0x0040U
#define DD_TMP_102_CONFIG_CR_4HZ        0x0080U
#define DD_TMP_102_CONFIG_CR_8HZ        0x00C0U
#define DD_TMP_102_CONFIG_AL_ENABLE     0x0020U
#define DD_TMP_102_CONFIG_AL_DISABLE    0x0000U
#define DD_TMP_102_CONFIG_EM_12BIT      0x0000U
#define DD_TMP_102_CONFIG_EM_13BIT      0x0010U

/* ADC specific */
#define DD_TMP_102_TEMP_LSB                  ( 0.0625F ) /**< Temperature ADC LSB @unit °C */

/*************************************************************
*                      ENUMERATORS                           *
*************************************************************/


/*************************************************************
*      STRUCTURES                                            *
*************************************************************/
/**
 * @brief   TMP-102 Output Interface Data Structure
 * @details TMP-102 Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct DD_TMP_102_DATA_OUT_TYPE_TAG
{
    S16 temperature_raw_s16; /**< @details Raw ADC temperature reading @unit LSB */
    F32 temperature_deg_f32; /**< @details Current temperature reading @unit °C */
} DD_TMP_102_DATA_OUT_TYPE;


/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/

class DD_TMP_102_C {

  private:

    DD_TMP_102_DATA_OUT_TYPE data_out_s;  /**< @details Global device output data structure */
    U8                       i2c_addr_u8; /**< @details I2C device address */

    /**
     * @details This function reads the raw temperature
     * @param[in] p_value_s16 Pointer to variable that stores the last temperature ADC reading
     * @return TRUE in case no error during I2C transmission occurs
     */
    BOOLEAN get_temperature_raw( S16* p_value_s16 );

  public:

    /**
     * @details Default constructor
     * @param[in] i2c_addr_u8 I2C device address
     */
    DD_TMP_102_C( U8 i2c_addr_u8 );

    /**
     * @details Default destructor
     */
    ~DD_TMP_102_C();

    /**
     * @details This function initialized the TMP-102 device
     * @return Pointer to global device data structure
     */
    DD_TMP_102_DATA_OUT_TYPE* init( void );

    /**
     * @details This is the drivers main function that shall be called
     * cyclicly and will provide all data through DD_INA_219_DATA_OUT_TYPE
     */
    void main( void );
};

#endif /* DD_TMP_102_H_ */
