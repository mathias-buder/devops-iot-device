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
 * @file dd_icm-20600.h
 * @details Some detailed description
 */

#ifndef DD_ICM_20600_H
#define DD_ICM_20600_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_types.h"


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
/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/



/**
 * @details Some details ...
 * @param[in]
 * @return
 */

BOOLEAN dd_icm_20600_init(void);

void dd_icm_20600_main(void);

#endif /* DD_ICM_20600_H */
