/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_icm-20600.h
        @details ICM-20600 device driver implementation

        (c) SEWELA 2020

*********************************************************************/
#ifndef DD_ICM_20600_H_
#define DD_ICM_20600_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"


/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
/* Log message TAG string */
#define DD_ICM_20600_LOG_MSG_TAG                    "DD_ICM_20600"           /**< @details Domain log message tag string */
#define DD_ICM_20600_DEVICE_ID                      0x73                     /**< Unique device id */
#define DD_ICM_20600_A_SCALSE                       DD_ICM_20600_AFS_2G      /**< Accelerometer scale @unit G*/
#define DD_ICM_20600_G_SCALSE                       DD_ICM_20600_GFS_250DPS  /**< Gyroscope scale @unit Deg/s */
#define DD_ICM_20600_ALLOWED_FAC_DEVIATION          ( 1.0F )                 /**< Maximum allowed factory devision from factory calibration @unit % */


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

/* ICM-20600 Register Map*/

/* TODO: The following register map is valid for device MPU-9250
 * (InvenSense) and need to be update to be used with ICM-20600 (TDK) */

#define DD_ICM_20600_XGOFFS_TC                      0x00        // Bit 7 PWR_MODE, bits 6:1 XG_OFFS_TC, bit 0 OTP_BNK_VLD
#define DD_ICM_20600_YGOFFS_TC                      0x01
#define DD_ICM_20600_ZGOFFS_TC                      0x02
#define DD_ICM_20600_X_FINE_GAIN                    0x03        // [7:0] fine gain
#define DD_ICM_20600_Y_FINE_GAIN                    0x04
#define DD_ICM_20600_Z_FINE_GAIN                    0x05
#define DD_ICM_20600_XA_OFFSET_H                    0x06        // User-defined trim values for accelerometer
#define DD_ICM_20600_XA_OFFSET_L_TC                 0x07
#define DD_ICM_20600_YA_OFFSET_H                    0x08
#define DD_ICM_20600_YA_OFFSET_L_TC                 0x09
#define DD_ICM_20600_ZA_OFFSET_H                    0x0A
#define DD_ICM_20600_ZA_OFFSET_L_TC                 0x0B
#define DD_ICM_20600_SELF_TEST_X                    0x0D
#define DD_ICM_20600_SELF_TEST_Y                    0x0E
#define DD_ICM_20600_SELF_TEST_Z                    0x0F
#define DD_ICM_20600_SELF_TEST_A                    0x10
#define DD_ICM_20600_XG_OFFS_USRH                   0x13        // User-defined trim values for gyroscope; supported in MPU-6050?
#define DD_ICM_20600_XG_OFFS_USRL                   0x14
#define DD_ICM_20600_YG_OFFS_USRH                   0x15
#define DD_ICM_20600_YG_OFFS_USRL                   0x16
#define DD_ICM_20600_ZG_OFFS_USRH                   0x17
#define DD_ICM_20600_ZG_OFFS_USRL                   0x18
#define DD_ICM_20600_SMPLRT_DIV                     0x19
#define DD_ICM_20600_CONFIG                         0x1A
#define DD_ICM_20600_GYRO_CONFIG                    0x1B
#define DD_ICM_20600_ACCEL_CONFIG                   0x1C
#define DD_ICM_20600_FF_THR                         0x1D        // Free-fall
#define DD_ICM_20600_FF_DUR                         0x1E        // Free-fall
#define DD_ICM_20600_MOT_THR                        0x1F        // Motion detection threshold bits [7:0]
#define DD_ICM_20600_MOT_DUR                        0x20        // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define DD_ICM_20600_ZMOT_THR                       0x21        // Zero-motion detection threshold bits [7:0]
#define DD_ICM_20600_ZRMOT_DUR                      0x22        // Duration counter threshold for zero motion interrupt generation, 16 Hz rate, LSB = 64 ms
#define DD_ICM_20600_FIFO_EN                        0x23
#define DD_ICM_20600_I2C_MST_CTRL                   0x24
#define DD_ICM_20600_I2C_SLV0_ADDR                  0x25
#define DD_ICM_20600_I2C_SLV0_REG                   0x26
#define DD_ICM_20600_I2C_SLV0_CTRL                  0x27
#define DD_ICM_20600_I2C_SLV1_ADDR                  0x28
#define DD_ICM_20600_I2C_SLV1_REG                   0x29
#define DD_ICM_20600_I2C_SLV1_CTRL                  0x2A
#define DD_ICM_20600_I2C_SLV2_ADDR                  0x2B
#define DD_ICM_20600_I2C_SLV2_REG                   0x2C
#define DD_ICM_20600_I2C_SLV2_CTRL                  0x2D
#define DD_ICM_20600_I2C_SLV3_ADDR                  0x2E
#define DD_ICM_20600_I2C_SLV3_REG                   0x2F
#define DD_ICM_20600_I2C_SLV3_CTRL                  0x30
#define DD_ICM_20600_I2C_SLV4_ADDR                  0x31
#define DD_ICM_20600_I2C_SLV4_REG                   0x32
#define DD_ICM_20600_I2C_SLV4_DO                    0x33
#define DD_ICM_20600_I2C_SLV4_CTRL                  0x34
#define DD_ICM_20600_I2C_SLV4_DI                    0x35
#define DD_ICM_20600_I2C_MST_STATUS                 0x36
#define DD_ICM_20600_INT_PIN_CFG                    0x37
#define DD_ICM_20600_INT_ENABLE                     0x38
#define DD_ICM_20600_DMP_INT_STATUS                 0x39        // Check DMP interrupt
#define DD_ICM_20600_INT_STATUS                     0x3A
#define DD_ICM_20600_ACCEL_XOUT_H                   0x3B
#define DD_ICM_20600_ACCEL_XOUT_L                   0x3C
#define DD_ICM_20600_ACCEL_YOUT_H                   0x3D
#define DD_ICM_20600_ACCEL_YOUT_L                   0x3E
#define DD_ICM_20600_ACCEL_ZOUT_H                   0x3F
#define DD_ICM_20600_ACCEL_ZOUT_L                   0x40
#define DD_ICM_20600_TEMP_OUT_H                     0x41
#define DD_ICM_20600_TEMP_OUT_L                     0x42
#define DD_ICM_20600_GYRO_XOUT_H                    0x43
#define DD_ICM_20600_GYRO_XOUT_L                    0x44
#define DD_ICM_20600_GYRO_YOUT_H                    0x45
#define DD_ICM_20600_GYRO_YOUT_L                    0x46
#define DD_ICM_20600_GYRO_ZOUT_H                    0x47
#define DD_ICM_20600_GYRO_ZOUT_L                    0x48
#define DD_ICM_20600_EXT_SENS_DATA_00               0x49
#define DD_ICM_20600_EXT_SENS_DATA_01               0x4A
#define DD_ICM_20600_EXT_SENS_DATA_02               0x4B
#define DD_ICM_20600_EXT_SENS_DATA_03               0x4C
#define DD_ICM_20600_EXT_SENS_DATA_04               0x4D
#define DD_ICM_20600_EXT_SENS_DATA_05               0x4E
#define DD_ICM_20600_EXT_SENS_DATA_06               0x4F
#define DD_ICM_20600_EXT_SENS_DATA_07               0x50
#define DD_ICM_20600_EXT_SENS_DATA_08               0x51
#define DD_ICM_20600_EXT_SENS_DATA_09               0x52
#define DD_ICM_20600_EXT_SENS_DATA_10               0x53
#define DD_ICM_20600_EXT_SENS_DATA_11               0x54
#define DD_ICM_20600_EXT_SENS_DATA_12               0x55
#define DD_ICM_20600_EXT_SENS_DATA_13               0x56
#define DD_ICM_20600_EXT_SENS_DATA_14               0x57
#define DD_ICM_20600_EXT_SENS_DATA_15               0x58
#define DD_ICM_20600_EXT_SENS_DATA_16               0x59
#define DD_ICM_20600_EXT_SENS_DATA_17               0x5A
#define DD_ICM_20600_EXT_SENS_DATA_18               0x5B
#define DD_ICM_20600_EXT_SENS_DATA_19               0x5C
#define DD_ICM_20600_EXT_SENS_DATA_20               0x5D
#define DD_ICM_20600_EXT_SENS_DATA_21               0x5E
#define DD_ICM_20600_EXT_SENS_DATA_22               0x5F
#define DD_ICM_20600_EXT_SENS_DATA_23               0x60
#define DD_ICM_20600_MOT_DETECT_STATUS              0x61
#define DD_ICM_20600_I2C_SLV0_DO                    0x63
#define DD_ICM_20600_I2C_SLV1_DO                    0x64
#define DD_ICM_20600_I2C_SLV2_DO                    0x65
#define DD_ICM_20600_I2C_SLV3_DO                    0x66
#define DD_ICM_20600_I2C_MST_DELAY_CTRL             0x67
#define DD_ICM_20600_SIGNAL_PATH_RESET              0x68
#define DD_ICM_20600_MOT_DETECT_CTRL                0x69
#define DD_ICM_20600_USER_CTRL                      0x6A        // Bit 7 enable DMP, bit 3 reset DMP
#define DD_ICM_20600_PWR_MGMT_1                     0x6B        // Device defaults to the SLEEP mode
#define DD_ICM_20600_PWR_MGMT_2                     0x6C
#define DD_ICM_20600_DMP_BANK                       0x6D        // Activates a specific bank in the DMP
#define DD_ICM_20600_DMP_RW_PNT                     0x6E        // Set read/write pointer to a specific start address in specified DMP bank
#define DD_ICM_20600_DMP_REG                        0x6F        // Register in DMP from which to read or to which to write
#define DD_ICM_20600_DMP_REG_1                      0x70
#define DD_ICM_20600_DMP_REG_2                      0x71
#define DD_ICM_20600_FIFO_COUNTH                    0x72
#define DD_ICM_20600_FIFO_COUNTL                    0x73
#define DD_ICM_20600_FIFO_R_W                       0x74
#define DD_ICM_20600_WHO_AM_I                       0x75        // Should return 0x68


#define DD_ICM_20600_PWR_MGMT_1_DEVICE_RESET_B      0x80
#define DD_ICM_20600_PWR_MGMT_1_SLEEP_B             0x40
#define DD_ICM_20600_PWR_MGMT_1_CYCLE_B             0x20
#define DD_ICM_20600_PWR_MGMT_1_GYRO_STANDBY_B      0x10
#define DD_ICM_20600_PWR_MGMT_1_TEMP_DIS_B          0x08
#define DD_ICM_20600_PWR_MGMT_1_CLKSEL_B            0x07

/* Gyroscope Configuration */
#define DD_ICM_20600_GYRO_CFG_FS_SEL_MASK           0xE7 /* (byte)~0b00011000 */
#define DD_ICM_20600_GYRO_CFG_FS_SEL_250_DPS        0x00
#define DD_ICM_20600_GYRO_CFG_FS_SEL_500_DPS        0x08
#define DD_ICM_20600_GYRO_CFG_FS_SEL_1000_DPS       0x10
#define DD_ICM_20600_GYRO_CFG_FS_SEL_2000_DPS       0x18

#define DD_ICM_20600_GYRO_REL_250_DPS               ( 0.00762939453125 )
#define DD_ICM_20600_GYRO_REL_500_DPS               ( 0.0152587890625F )
#define DD_ICM_20600_GYRO_REL_1000_DPS              ( 0.030517578125F )
#define DD_ICM_20600_GYRO_REL_2000_DPS              ( 0.06103515625F )


/* Accelerometer Configuration */
#define DD_ICM_20600_ACCL_CFG_FS_SEL_MASK           0xE7 /* (byte)~0b00011000 */
#define DD_ICM_20600_ACCL_CFG_FS_SEL_2G             0x00
#define DD_ICM_20600_ACCL_CFG_FS_SEL_4G             0x08
#define DD_ICM_20600_ACCL_CFG_FS_SEL_8G             0x10
#define DD_ICM_20600_ACCL_CFG_FS_SEL_16G            0x18

#define DD_ICM_20600_ACCL_REL_2G                   ( 0.00006103515625F )
#define DD_ICM_20600_ACCL_REL_4G                   ( 0.0001220703125F )
#define DD_ICM_20600_ACCL_REL_8G                   ( 0.000244140625F )
#define DD_ICM_20600_ACCL_REL_16G                  ( 0.00048828125F )


#define DD_ICM_20600_TEMP_SENSITIVITY               326.8F /* LSB / °C */
#define DD_ICM_20600_TEMP_SENSITIVITY_INV           ( 0.00305997552019583843F ) /* 1.0F / 326.8F */
#define DD_ICM_20600_TEMP_ROOM_OFFSET               21.0F  /* °C */


/*************************************************************
*                      ENUMERATORS                           *
*************************************************************/

/**
 * @details Accelerometer scale @unit G
 */
typedef enum DD_ICM_20600_AFS_TAG
{
    DD_ICM_20600_AFS_2G = 0U, /**< @details  2 * 9.81 m/s^2 */
    DD_ICM_20600_AFS_4G,      /**< @details  4 * 9.81 m/s^2 */
    DD_ICM_20600_AFS_8G,      /**< @details  8 * 9.81 m/s^2 */
    DD_ICM_20600_AFS_16G,     /**< @details 16 * 9.81 m/s^2 */
    DD_ICM_20600_SCALE_A_SIZE
} DD_ICM_20600_AFS;

/**
 * @details Gyroscope scale @unit Deg/s
 */
typedef enum DD_ICM_20600_GFS_TAG
{
    DD_ICM_20600_GFS_250DPS = 0U, /**< @details  250 °/s */
    DD_ICM_20600_GFS_500DPS,     /**< @details  500 °/s */
    DD_ICM_20600_GFS_1000DPS,    /**< @details 1000 °/s */
    DD_ICM_20600_GFS_2000DPS,    /**< @details 2000 °/s */
    DD_ICM_20600_SCALE_G_SIZE
} DD_ICM_20600_GFS;

/**
 * @details Accelerometer axis slot
 */
typedef enum DD_ICM_20600_ACCEL_TYPE_TAG
{
    DD_ICM_20600_ACCEL_X = 0U, /**< @details Accelerometer x-axis slot */
    DD_ICM_20600_ACCEL_Y,      /**< @details Accelerometer y-axis slot */
    DD_ICM_20600_ACCEL_Z,      /**< @details Accelerometer z-axis slot */
    DD_ICM_20600_ACCEL_SIZE    /**< @details Maximum number of Accelerometer axis slots */
} DD_ICM_20600_ACCEL_TYPE;

/**
 * @details Gyroscope axis slot
 */
typedef enum DD_ICM_20600_GYRO_TYPE_TAG
{
    DD_ICM_20600_GYRO_X = 0U, /**< @details Gyroscope x-axis slot */
    DD_ICM_20600_GYRO_Y,      /**< @details Gyroscope y-axis slot */
    DD_ICM_20600_GYRO_Z,      /**< @details Gyroscope z-axis slot */
    DD_ICM_20600_GYRO_SIZE    /**< @details Maximum number of Gyroscope axis slots */
} DD_ICM_20600_GYRO_TYPE;

/**
 * @details  Self-test axis slot
 */
typedef enum DD_ICM_20600_SELF_TEST_TAG
{
    DD_ICM_20600_SELF_TEST_XA = 0U, /**< @details Self-test Accelerometer x-axis slot */
    DD_ICM_20600_SELF_TEST_YA,      /**< @details Self-test Accelerometer y-axis slot */
    DD_ICM_20600_SELF_TEST_ZA,      /**< @details Self-test Accelerometer z-axis slot */
    DD_ICM_20600_SELF_TEST_XG,      /**< @details Self-test Gyroscope x-axis slot */
    DD_ICM_20600_SELF_TEST_YG,      /**< @details Self-test Gyroscope y-axis slot */
    DD_ICM_20600_SELF_TEST_ZG,      /**< @details Self-test Gyroscope z-axis slot */
    DD_ICM_20600_SELF_TEST_SIZE     /**< @details Maximum number of self-test axis slots */
} DD_ICM_20600_SELF_TEST;

/**
 * @details Device states
 */
typedef enum DD_ICM_20600_STATE_TAG
{
    DD_ICM_20600_STATE_INIT = 0U, /**< @details Initialization */
    DD_ICM_20600_STATE_TEST,      /**< @details Test (Self-Test) */
    DD_ICM_20600_STATE_CALIB,     /**< @details Calibration */
    DD_ICM_20600_STATE_RUN,       /**< @details Running */
    DD_ICM_20600_STATE_SIZE       /**< @details Maximum number of states */
} DD_ICM_20600_STATE;

/**
 * @details Error states
 */
typedef enum DD_ICM_20600_ERROR_TAG
{
    DD_ICM_20600_ERROR_NONE = 0U, /**< @details No error present */
    DD_ICM_20600_ERROR_INIT,      /**< @details Error occurred during initialization */
    DD_ICM_20600_ERROR_SELF_TEST, /**< @details Error occurred during self-test */
    DD_ICM_20600_ERROR_CALIB,     /**< @details Error occurred during calibration */
    DD_ICM_20600_ERROR_RUNTIME    /**< @details Error occurred during normal runtime */
} DD_ICM_20600_ERROR;

/*************************************************************
*      STRUCTURES                                            *
*************************************************************/

/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure contains all data
            provided by ICM-20600.
 * @ingroup DriverStructures
 */
typedef struct DD_ICM_20600_DATA_OUT_TYPE_TAG
{
    U8                 device_id_u8;                                         /**< @details Unique device id */
    F32                temperature_deg_f32;                                  /**< @details Internal core (die) temperature @unit °C */
    U16                temperature_raw_u16;                                  /**< @details Internal core (die) temperature raw @unit LSB */
    DD_ICM_20600_AFS   accel_full_scale_sel_e;                               /**< @details Acceleration scale @unit G */
    DD_ICM_20600_GFS   gyro_full_scale_sel_e;                                /**< @details Gyroscope scale @unit Deg/s */
    F32                accel_resolution_f32;                                 /**< @details Acceleration raw data */
    F32                gyro_resolution_f32;                                  /**< @details Acceleration raw data */
    S16                accel_data_raw_s16[DD_ICM_20600_ACCEL_SIZE];          /**< @details Gyroscope raw data @unit LSB */
    S16                gyro_data_raw_s16[DD_ICM_20600_GYRO_SIZE];            /**< @details Acceleration raw data  @unit LSB */
    U8                 self_test_vu8[DD_ICM_20600_SELF_TEST_SIZE];           /**< @details Self-Test result */
    BOOLEAN            self_test_passed_b;                                   /**< @details Flag to indicate whether the self-test has passed or not */
    BOOLEAN            is_calibrated_b;                                      /**< @details Flag to indicate whether the device is calibrated or not */
    F32                factory_trim_vf32[DD_ICM_20600_SELF_TEST_SIZE];       /**< @details Factory trim values */
    F32                fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_SIZE]; /**< @details Deviation from factory trim values */
    DD_ICM_20600_STATE state_e;                                              /**< @details Main device state */
    DD_ICM_20600_ERROR error_e;                                              /**< @details Main error state */
} DD_ICM_20600_DATA_OUT_TYPE;

/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/

class DD_ICM_20600_C {

  private:
    DD_ICM_20600_DATA_OUT_TYPE data_out_s;  /**< @details Global device output data structure */
    U8                         i2c_addr_u8; /**< @details I2C device address */

    /**
     * @details This function performs a device soft reset
     * @return TRUE if no I2C transmission error occurred, FALSE otherwise
     */
    BOOLEAN soft_reset( void );

    /**
     * @details This function reads the device id
     * @param[in] data_u8 Reference to variable to store the id
     * @return TRUE if no I2C transmission error occurred, FALSE otherwise
     */
    BOOLEAN get_device_id( U8& data_u8 );

    /**
     * @details This function configures the full-scale range of the accelerometer
     * @param[in] scale_e Accelerometer full-scale range
     * @return TRUE if no I2C transmission error occurred, FALSE otherwise
     */
    BOOLEAN set_accel_full_scale( const DD_ICM_20600_AFS scale_e );

    /**
     * @details This function configures the full-scale range of the gyroscope
     * @param[in] scale_e gyroscope full-scale range
     * @return TRUE if no I2C transmission error occurred, FALSE otherwise
     */
    BOOLEAN set_gyro_full_scale( const DD_ICM_20600_GFS scale_e );

    /**
     * @details This function reads the device die-temperature
     * @param[in] p_data_u8 Reference to variable to store die-temperature
     * @return TRUE if no I2C transmission error occurred, FALSE otherwise
     */
    BOOLEAN get_temperature( DD_ICM_20600_DATA_OUT_TYPE& data_s );

    /**
     * @details This function reads the raw accelerometer data of all three axis
     * @param[in] data_s Reference to variable to store the raw accelerometer data
     * @return TRUE if no I2C transmission error occurred, FALSE otherwise
     */
    BOOLEAN get_accel_data_raw( DD_ICM_20600_DATA_OUT_TYPE& data_s );

    /**
     * @details This function reads the raw gyroscope data of all three axis
     * @param[in] data_u8 Reference to variable to store the raw gyroscope data
     * @return TRUE if no I2C transmission error occurred, FALSE otherwise
     */
    BOOLEAN get_gyro_data_raw( DD_ICM_20600_DATA_OUT_TYPE& data_s );

    /**
     * @details This function runs a self-test algorithm that will test the accelerometer
     *          and gyroscope functionality.
     * @param[in] data_s Reference to variable to store detailed self-test results
     * @return TRUE if self-test has passed, FALSE otherwise
     */
    BOOLEAN self_test( DD_ICM_20600_DATA_OUT_TYPE& data_s );

    /**
     * @details This function runs a calibration algorithm that will gather
     *          at-rest compensation values from accelerometer gyroscope.
     * @param[in] gyro_bias_f32 Reference to variable to store gyroscope compensation values
     * @param[in] accel_bias_f32 Reference to variable to store accelerometer compensation values
     * @return TRUE if no I2C transmission error occurred, FALSE otherwise
     */
    BOOLEAN calibrate( F32 ( &p_gyro_bias_f32 )[3U], F32 ( &p_accel_bias_f32 )[3U] );

  public:
    /**
     * @details Default constructor
     * @param[in] i2c_addr_u8 I2C device address
     */
    DD_ICM_20600_C( U8 i2c_addr_u8 );

    /**
     * @details Default destructor
     */
    ~DD_ICM_20600_C();

    /**
     * @details This function initialized the ICM-20600 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    DD_ICM_20600_DATA_OUT_TYPE* init( void );

    /**
     * @details This is the drivers main function that shall be called
     * cyclicly and will provide all data through DD_INA_219_DATA_OUT_TYPE
     */
    void main( void );
};

#endif /* DD_ICM_20600_H_ */
