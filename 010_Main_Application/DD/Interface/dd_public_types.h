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
#include <esp_err.h>

#include "../../types.h"
#include "../Core/dd_types.h"

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
    DD_I2C_ERROR_FAIL         = ESP_FAIL, /*!< Generic esp_err_t code indicating failure */
    DD_I2C_ERROR_OK           = ESP_OK,   /*!< esp_err_t value indicating success (no error) */
    DD_I2C_ERROR_INVALID_SIZE = ESP_ERR_INVALID_SIZE,
    DD_I2C_ERROR_INVALID_ARG  = ESP_ERR_INVALID_ARG,
    DD_I2C_ERROR_TIMEOUT      = ESP_ERR_TIMEOUT, /*!< Operation timed out */
    DD_I2C_ERROR_SIZE
} DD_I2C_ERROR;

/**
 * @details enumerator of ...
 */
typedef enum DD_I2C_ACCESS_TYPE_TAG
{
    DD_I2C_ACCESS_TYPE_RD_SINGLE = 0U,
    DD_I2C_ACCESS_TYPE_RD_BURST,
    DD_I2C_ACCESS_TYPE_WD_SINGLE,
    DD_I2C_ACCESS_TYPE_WD_BURST,
    DD_I2C_ACCESS_TYPE_SIZE
} DD_I2C_ACCESS_TYPE;


/*************************************************************/
/*                        DD_ICM_20600                       */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_SCALE_A_TAG
{
    DD_ICM_20600_AFS_2G = 0,  /**< @details ... */
    DD_ICM_20600_AFS_4G,      /**< @details ... */
    DD_ICM_20600_AFS_8G,      /**< @details ... */
    DD_ICM_20600_AFS_16G,     /**< @details ... */
    DD_ICM_20600_SCALE_A_SIZE /**< @details ... */
} DD_ICM_20600_SCALE_A;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_SCALE_G_TAG
{
    DD_ICM_20600_GFS_250DPS = 0, /**< @details ... */
    DD_ICM_20600_GFS_500DPS,     /**< @details ... */
    DD_ICM_20600_GFS_1000DPS,    /**< @details ... */
    DD_ICM_20600_GFS_2000DPS,    /**< @details ... */
    DD_ICM_20600_SCALE_G_SIZE
} DD_ICM_20600_SCALE_G;

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
    DD_ICM_20600_SELF_TEST_SIZE    /**< @details ... */
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
    DD_I2C_ERROR       error_e;
    DD_I2C_ACCESS_TYPE access_type_e;
    U8                 register_addr_u8;
    F32                time_stamp_f32;
} DD_I2C_ERROR_INFO_TYPE;

/**
 * @details enumerator of ...
 */
typedef struct DD_I2C_ERROR_TYPE_TAG
{
    DD_I2C_ERROR_INFO_TYPE error_info_vs[I2C_ERROR_BUFFER_LENGTH];
    U8                     current_error_idx_u8;
    U8                     last_error_idx_u8;
    BOOLEAN                state_b;
} DD_I2C_ERROR_TYPE;


/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup DriverStructures
 */
typedef struct DD_ICM_20600_DATA_TAG
{
    U8                      chip_id_u8;                                           /** @details Unique chip id */
    F32                     temperature_deg_f32;                                  /** @details Internal core (die) temperature @unit °C */
    U16                     temperature_raw_u16;                                  /** @details Internal core (die) temperature raw data */
    U16                     accel_data_raw_u16[DD_ICM_20600_ACCEL_SIZE];          /** @details Acceleration raw data */
    U16                     gyro_data_raw_u16[DD_ICM_20600_GYRO_SIZE];            /** @details Acceleration raw data */
    U8                      self_test_vu8[DD_ICM_20600_SELF_TEST_SIZE];           /** @details main icm-20600 device state */
    BOOLEAN                 self_test_passed_b;                                   /** @details main icm-20600 device state */
    BOOLEAN                 is_calibrated_b;                                      /** @details main icm-20600 device state */
    F32                     factory_trim_vf32[DD_ICM_20600_SELF_TEST_SIZE];       /** @details main icm-20600 device state */
    F32                     fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_SIZE]; /** @details main icm-20600 device state */
    DD_DEV_STATE            dev_state_s;                                          /** @details main icm-20600 device state */
} DD_ICM_20600_DATA;

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
 * @brief  SD-Card Output Interface Data Structure
 * @details
 * @ingroup DriverStructures
 */
typedef struct DD_SD_DATA_TAG
{
    FILE*   p_file;                                   /** @details FILE pointer */
    char    file_path_vc[DD_SD_MAX_FILE_PATH_LENGTH]; /** @details Complete file path (incl. mount point) */
    U32     file_size_u32;                            /** @details Total file size */
    BOOLEAN is_file_open_b;                           /** @details Flag indicating that file is currently open */
    BOOLEAN is_fs_mounted_b;                          /** @details Flag indicating that file system is mounted on SD card */
} DD_SD_DATA;

#endif /* DD_PUBLIC_TYPES_H */

