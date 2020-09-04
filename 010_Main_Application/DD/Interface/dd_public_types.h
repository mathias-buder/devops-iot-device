/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_public_types.h
        @details Some detailed description

*********************************************************************/
#ifndef DD_PUBLIC_TYPES_H
#define DD_PUBLIC_TYPES_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <stdio.h>
#include <sys/stat.h>

#include <esp_err.h>

#include "../../types.h"

/* TODO: DD-Types include concept needs to be reworked */
#include "../Config/dd_types_cfg.h"

/*************************************************************/
/*                      ENUMERATORS                           */
/*************************************************************/

/*************************************************************/
/*                          DD                              */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_STATE_TAG
{
    DD_STATE_INIT = 0, /**< @details ... */
    DD_STATE_TEST,     /**< @details ... */
    DD_STATE_CALIB,    /**< @details ... */
    DD_STATE_RUN,      /**< @details ... */
    DD_STATE_SIZE      /**< @details ... */
} DD_STATE;

typedef struct DD_DEV_STATE_TAG
{
    DD_STATE state_e; /**< @details ... */
} DD_DEV_STATE;

/*************************************************************/
/*                          DD_I2C                           */
/*************************************************************/

/**
 * @details enumerator of ...
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
 * @details enumerator of ...
 */
typedef enum DD_I2C_ACCESS_TYPE_TAG
{
    DD_I2C_ACCESS_TYPE_RD_SINGLE = 0U, /**< @details ... */
    DD_I2C_ACCESS_TYPE_RD_BURST,       /**< @details ... */
    DD_I2C_ACCESS_TYPE_WD_SINGLE,      /**< @details ... */
    DD_I2C_ACCESS_TYPE_WD_BURST,       /**< @details ... */
    DD_I2C_ACCESS_TYPE_SIZE            /**< @details ... */
} DD_I2C_ACCESS_TYPE;

/*************************************************************/
/*                        DD_ICM_20600                       */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_AFS_TAG
{
    DD_ICM_20600_AFS_2G = 0,  /**< @details  2 * 9.81 m/s^2 */
    DD_ICM_20600_AFS_4G,      /**< @details  4 * 9.81 m/s^2 */
    DD_ICM_20600_AFS_8G,      /**< @details  8 * 9.81 m/s^2 */
    DD_ICM_20600_AFS_16G,     /**< @details 16 * 9.81 m/s^2 */
    DD_ICM_20600_SCALE_A_SIZE
} DD_ICM_20600_AFS;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_GFS_TAG
{
    DD_ICM_20600_GFS_250DPS = 0, /**< @details  250 °/s */
    DD_ICM_20600_GFS_500DPS,     /**< @details  500 °/s */
    DD_ICM_20600_GFS_1000DPS,    /**< @details 1000 °/s */
    DD_ICM_20600_GFS_2000DPS,    /**< @details 2000 °/s */
    DD_ICM_20600_SCALE_G_SIZE
} DD_ICM_20600_GFS;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_ACCEL_TYPE_TAG
{
    DD_ICM_20600_ACCEL_X = 0, /**< @details ... */
    DD_ICM_20600_ACCEL_Y,     /**< @details ... */
    DD_ICM_20600_ACCEL_Z,     /**< @details ... */
    DD_ICM_20600_ACCEL_SIZE   /**< @details ... */
} DD_ICM_20600_ACCEL_TYPE;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_GYRO_TYPE_TAG
{
    DD_ICM_20600_GYRO_X = 0, /**< @details ... */
    DD_ICM_20600_GYRO_Y,     /**< @details ... */
    DD_ICM_20600_GYRO_Z,     /**< @details ... */
    DD_ICM_20600_GYRO_SIZE   /**< @details ... */
} DD_ICM_20600_GYRO_TYPE;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_SELF_TEST_TAG
{
    DD_ICM_20600_SELF_TEST_XA = 0, /**< @details ... */
    DD_ICM_20600_SELF_TEST_YA,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_ZA,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_XG,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_YG,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_ZG,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_SIZE
} DD_ICM_20600_SELF_TEST;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/*************************************************************/
/*                          DD_I2C                           */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef struct DD_I2C_ERROR_INFO_TYPE_TAG
{
    DD_I2C_ERROR       error_e;          /**< @details ... */
    DD_I2C_ACCESS_TYPE access_type_e;    /**< @details ... */
    U8                 device_addr_u8;   /**< @details ... */
    U8                 register_addr_u8; /**< @details ... */
    F32                time_stamp_f32;   /**< @details ... */
} DD_I2C_ERROR_INFO_TYPE;

/**
 * @details enumerator of ...
 */
typedef struct DD_I2C_ERROR_TYPE_TAG
{
    DD_I2C_ERROR_INFO_TYPE error_info_vs[DD_I2C_ERROR_BUFFER_LENGTH]; /**< @details ... */
    U8                     current_error_idx_u8;                      /**< @details ... */
    U8                     last_error_idx_u8;                         /**< @details ... */
    BOOLEAN                is_error_present_b;                        /**< @details ... */
} DD_I2C_ERROR_TYPE;

/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup DriverStructures
 */
typedef struct DD_ICM_20600_DATA_TAG
{
    U8               chip_id_u8;                                           /**< @details Unique chip id */
    F32              temperature_deg_f32;                                  /**< @details Internal core (die) temperature @unit °C */
    U16              temperature_raw_u16;                                  /**< @details Internal core (die) temperature raw data */
    DD_ICM_20600_AFS accel_full_scale_sel_e;                               /**< @details Acceleration raw data */
    DD_ICM_20600_GFS gyro_full_scale_sel_e;                                /**< @details Acceleration raw data */
    F32              accel_resolution_f32;                                 /**< @details Acceleration raw data */
    F32              gyro_resolution_f32;                                  /**< @details Acceleration raw data */
    S16              accel_data_raw_s16[DD_ICM_20600_ACCEL_SIZE];          /**< @details Acceleration raw data */
    S16              gyro_data_raw_s16[DD_ICM_20600_GYRO_SIZE];            /**< @details Acceleration raw data */
    U8               self_test_vu8[DD_ICM_20600_SELF_TEST_SIZE];           /**< @details main icm-20600 device state */
    BOOLEAN          self_test_passed_b;                                   /**< @details main icm-20600 device state */
    BOOLEAN          is_calibrated_b;                                      /**< @details main icm-20600 device state */
    F32              factory_trim_vf32[DD_ICM_20600_SELF_TEST_SIZE];       /**< @details main icm-20600 device state */
    F32              fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_SIZE]; /**< @details main icm-20600 device state */
    DD_DEV_STATE     dev_state_s;                                          /**< @details main icm-20600 device state */
} DD_ICM_20600_DATA;

/*************************************************************/
/*                        DD_MAX_30102                       */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_MAX_30102_PTR_TYPE_TAG
{
    DD_MAX_30102_PTR_TYPE_READ = 0U,   /**< @details ... */
    DD_MAX_30102_PTR_TYPE_WRITE,        /**< @details ... */
    DD_MAX_30102_PTR_TYPE_SIZE
} DD_MAX_30102_PTR_TYPE;

typedef enum DD_MAX_30102_LED_TYPE_TAG
{
    DD_MAX_30102_LED_TYPE_RED = 0U,   /**< @details ... */
    DD_MAX_30102_LED_TYPE_IR,        /**< @details ... */
    DD_MAX_30102_LED_TYPE_PROX,
    DD_MAX_30102_LED_TYPE_SIZE
} DD_MAX_30102_LED_TYPE;

typedef enum DD_MAX_30102_SLOT_TAG
{
    DD_MAX_30102_SLOT_1 = 0U,   /**< @details ... */
    DD_MAX_30102_SLOT_2,        /**< @details ... */
    DD_MAX_30102_SLOT_3,
    DD_MAX_30102_SLOT_4,
    DD_MAX_30102_SLOT_SIZE
} DD_MAX_30102_SLOT;

typedef enum DD_MAX_30102_SLOT_MODE_TAG
{
    DD_MAX_30102_SLOT_MODE_NONE      = 0U,  /**< @details ... */
    DD_MAX_30102_SLOT_MODE_LED_RED,         /**< @details ... */
    DD_MAX_30102_SLOT_MODE_LED_IR,          /**< @details ... */
    DD_MAX_30102_SLOT_MODE_LED_RED_PILOT,
    DD_MAX_30102_SLOT_MODE_LED_IR_PILOT,
    DD_MAX_30102_SLOT_MODE_SIZE
} DD_MAX_30102_SLOT_MODE;

typedef enum DD_MAX_30102_MODE_TAG
{
    DD_MAX_30102_MODE_SLEEP = 0U,
    DD_MAX_30102_MODE_HR,        /**< @details ... */
    DD_MAX_30102_MODE_SPO2,      /**< @details ... */
    DD_MAX_30102_MODE_MULTI_LED  /**< @details ... */
} DD_MAX_30102_MODE;

typedef enum DD_MAX_30102_ADC_RANGE_TAG
{
    DD_MAX_30102_ADC_RANGE_2048 = 0U, /**< @details ... */
    DD_MAX_30102_ADC_RANGE_4096,     /**< @details ... */
    DD_MAX_30102_ADC_RANGE_8192,     /**< @details ... */
    DD_MAX_30102_ADC_RANGE_16384,
    DD_MAX_30102_ADC_RANGE_SIZE
} DD_MAX_30102_ADC_RANGE;

typedef enum DD_MAX_30102_SAMPLE_RATE_TAG
{
    DD_MAX_30102_SAMPLE_RATE_50 = 0U, /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_100,    /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_200,    /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_400,    /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_800,    /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_1000,   /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_1600,   /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_3200,   /**< @details ... */
    DD_MAX_30102_SAMPLE_RATE_SIZE
} DD_MAX_30102_SAMPLE_RATE;

typedef enum DD_MAX_30102_PULSE_WIDTH_TAG
{
    DD_MAX_30102_PULSE_WIDTH_69 = 0, /**< @details ... */
    DD_MAX_30102_PULSE_WIDTH_118,    /**< @details ... */
    DD_MAX_30102_PULSE_WIDTH_215,    /**< @details ... */
    DD_MAX_30102_PULSE_WIDTH_411,    /**< @details ... */
    DD_MAX_30102_PULSE_WIDTH_SIZE
} DD_MAX_30102_PULSE_WIDTH;

typedef enum DD_MAX_30102_SAMPLE_AVG_TAG
{
    DD_MAX_30102_SAMPLE_AVG_NONE = 0U, /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_2,        /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_4,        /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_8,        /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_16,       /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_32,       /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_SIZE
} DD_MAX_30102_SAMPLE_AVG;

typedef enum DD_MAX_30102_TEMP_COMP_TAG
{
    DD_MAX_30102_TEMP_COMP_INT = 0U, /**< @details ... */
    DD_MAX_30102_TEMP_COMP_FRAC,     /**< @details ... */
    DD_MAX_30102_TEMP_COMP_SIZE
} DD_MAX_30102_TEMP_COMP;

typedef enum DD_MAX_30102_INT_TYPE_TAG
{
    DD_MAX_30102_INT_TYPE_A_FULL = 0U,  /**< @details ... */
    DD_MAX_30102_INT_TYPE_PRG_RDY,      /**< @details ... */
    DD_MAX_30102_INT_TYPE_ALC_OVF,      /**< @details ... */
    DD_MAX_30102_INT_TYPE_PROX_INT,     /**< @details ... */
    DD_MAX_30102_INT_TYPE_PWR_RDY,      /**< @details ... */
    DD_MAX_30102_INT_TYPE_DIE_TEMP_RDY, /**< @details ... */
    DD_MAX_30102_INT_TYPE_SIZE
} DD_MAX_30102_INT_TYPE;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief   MAX-30102 Output Interface Data Structure
 * @details MAX-30102 Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct DD_MAX_30102_DATA_TAG
{
    BOOLEAN           int_status_vb[DD_MAX_30102_INT_TYPE_SIZE];
    U8                part_id_u8;
    U8                rev_id_u8;
    U8                read_ptr_u8;
    U8                write_ptr_u8;
    U8                active_leds_u8;
    U32               red_data_raw_u32;
    U32               ir_data_raw_u32;
    U8                temperature_raw_vu8[DD_MAX_30102_TEMP_COMP_SIZE];
    F32               temperature_f32;
    DD_MAX_30102_MODE mode_e;

} DD_MAX_30102_DATA;

/*************************************************************/
/*                           DD_SD                           */
/*************************************************************/
/**
 * @details enumerator of ...
 */
typedef enum DD_SD_FILE_MODE_TAG
{
    DD_SD_FILE_MODE_WRITE,
    DD_SD_FILE_MODE_WRITE_BINARY,
    DD_SD_FILE_MODE_SIZE
} DD_SD_FILE_MODE;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief  File Structure
 * @details
 * @ingroup DriverStructures
 */
typedef struct DD_SD_FILE_TAG
{
    FILE*       p_handle;                                 /**< @details FILE pointer */
    struct stat attrib_s;                                 /**< @details POSIX structure containing file attributes */
    char        full_path_vc[DD_SD_MAX_FILE_PATH_LENGTH]; /**< @details Complete file path (incl. mount point) */
    BOOLEAN     is_open_b;                                /**< @details Flag indicating that file is currently open */
} DD_SD_FILE;

/**
 * @brief  SD-Card Output Interface Data Structure
 * @details
 * @ingroup DriverStructures
 */
typedef struct DD_SD_DATA_TAG
{
    DD_SD_FILE file_s;
    BOOLEAN    is_fs_mounted_b; /**< @details Flag indicating that file system is mounted on SD card */
} DD_SD_DATA;


/*************************************************************/
/*                        DD_ADC                         */
/*************************************************************/


/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief   ADC Output Interface Data Structure
 * @details ADC Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct DD_ADC_DATA_TAG
{
    U16 raw_data_u16;          /**< @details Current raw 12-bit ADC value @unit [LSB] */
    U16 previous_raw_data_u16; /**< @details Previous raw 12-bit ADC value @unit [LSB] */
    U16 voltage_u16;           /**< @details Actual voltage measured at the ADC input pin @unit [mV] */
    F32 raw_level_f32;         /**< @details ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    F32 filtered_level_f32;    /**< @details Filtered ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */

} DD_ADC_DATA;

#endif /* DD_PUBLIC_TYPES_H */
