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
 * @file dd_types.h
 * @details This files defines the domain specific types
 * used in the device driver domain.
 */

#ifndef DD_TYPES_H
#define DD_TYPES_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <esp_err.h>

#include "../../types.h"
#include "../Config/dd_types_Cfg.h"
#include "../Interface/dd_public_types.h"


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
    DD_I2C_ERROR_TIMEOUT      = ESP_ERR_TIMEOUT /*!< Operation timed out */
} DD_I2C_ERROR;

/**
 * @details enumerator of ...
 */
typedef struct DD_I2C_ERROR_TYPE_TAG
{
    DD_I2C_ERROR current_t;
    DD_I2C_ERROR previous_t;
    BOOLEAN      state_b;
} DD_I2C_ERROR_TYPE;

/*************************************************************/
/*                        DD_ICM_20600                       */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_SCALE_A_TAG
{
    DD_ICM_20600_AFS_2G = 0, /**< @details ... */
    DD_ICM_20600_AFS_4G,     /**< @details ... */
    DD_ICM_20600_AFS_8G,     /**< @details ... */
    DD_ICM_20600_AFS_16G,    /**< @details ... */
    DD_ICM_20600_SCALE_A_SIZE
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
    DD_ICM_20600_ACCEL_SIZE
} DD_ICM_20600_ACCEL_TYPE;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_GYRO_TYPE_TAG
{
    DD_ICM_20600_GYRO_X = 0, /**< @details ... */
    DD_ICM_20600_GYRO_Y,     /**< @details ... */
    DD_ICM_20600_GYRO_Z,     /**< @details ... */
    DD_ICM_20600_GYRO_SIZE
} DD_ICM_20600_GYRO_TYPE;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_SELF_TEST_TAG
{
    DD_ICM_20600_SELF_TEST_XA = 0, /**< @details ... */
    DD_ICM_20600_SELF_TEST_YA,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_ZA,     /**< @details ... */
    DD_ICM_20600_SELF_TEST_XG,
    DD_ICM_20600_SELF_TEST_YG,
    DD_ICM_20600_SELF_TEST_ZG,
    DD_ICM_20600_SELF_TEST_SIZE
} DD_ICM_20600_SELF_TEST;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

typedef struct DD_ICM_20600_QUATERNION_TAG
{
    F32 Q1_f32;
    F32 Q2_f32;
    F32 Q3_f32;
    F32 Q4_f32;
} DD_ICM_20600_QUATERNION;

/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup SensorStructures
 */
typedef struct DD_ICM_20600_DATA_TAG
{
    U8                      chip_id_u8;                                  /** @details Unique chip id */
    F32                     temperature_deg_f32;                         /** @details Internal core (die) temperature @unit °C */
    U16                     temperature_raw_u16;                         /** @details Internal core (die) temperature raw data */
    U16                     accel_data_raw_u16[DD_ICM_20600_ACCEL_SIZE]; /** @details Acceleration raw data */
    U16                     gyro_data_raw_u16[DD_ICM_20600_GYRO_SIZE];   /** @details Acceleration raw data */
    DD_ICM_20600_QUATERNION Quaternion_s;                                /** @details Acceleration raw data */
    U8                      self_test_vu8[DD_ICM_20600_SELF_TEST_SIZE];
    BOOLEAN                 self_test_passed_b;
    BOOLEAN                 is_calibrated_b;
    F32                     factory_trim_vf32[DD_ICM_20600_SELF_TEST_SIZE];
    F32                     fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_SIZE];
    DD_DEV_STATE            dev_state_s; /** @details main icm-20600 device state */
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
    DD_MAX_30102_SLOT_MONE_PILOT,
    DD_MAX_30102_SLOT_MODE_LED_RED_PILOT,
    DD_MAX_30102_SLOT_MODE_LED_IR_PILOT,
    DD_MAX_30102_SLOT_MODE_SIZE
} DD_MAX_30102_SLOT_MODE;


typedef enum DD_MAX_30102_MODE_TAG
{
    DD_MAX_30102_MODE_RED = 0U,   /**< @details ... */
    DD_MAX_30102_MODE_RED_IR,    /**< @details ... */
    DD_MAX_30102_MODE_MULTI_LED, /**< @details ... */
    DD_MAX_30102_MODE_SIZE
} DD_MAX_30102_LED_MODE;

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
    DD_MAX_30102_SAMPLE_AVG_1 = 0U, /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_2,    /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_4,    /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_8,    /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_16,    /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_32,   /**< @details ... */
    DD_MAX_30102_SAMPLE_AVG_SIZE
} DD_MAX_30102_SAMPLE_AVG;

#endif /* DD_TYPES_H */
