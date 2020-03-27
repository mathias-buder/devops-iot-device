/*********************************************************************
    MODULE SPECIFICATION:

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dlg_types.h
        @details Some detailed description

*********************************************************************/

#ifndef DLG_CORE_TYPES_H_
#define DLG_CORE_TYPES_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../DD/Interface/dd_public_types.h"

/*************************************************************/
/*      ENUMERATORS                                          */
/*************************************************************/


/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef struct DLG_LOG_I2C_DATA_TAG
{
    U8  is_error_present_b; /**< @details ... */
    U8  access_type_u8;     /**< @details ... */
    U8  device_addr_u8;     /**< @details ... */
    U8  register_addr_u8;   /**< @details ... */
    S16 error_code_s16;     /**< @details ... */
} DLG_LOG_I2C_DATA;

/**
 * @details enumerator of ...
 */
typedef struct DLG_LOG_ICM_20600_DATA_TAG
{
    F32 temperature_deg_f32;     /**< @details Internal core (die) temperature @unit °C */
    F32 factory_trim_xa_f32;     /**< @details ... */
    F32 factory_trim_ya_f32;     /**< @details ... */
    F32 factory_trim_za_f32;     /**< @details ... */
    F32 factory_trim_xg_f32;     /**< @details ... */
    F32 factory_trim_yg_f32;     /**< @details ... */
    F32 factory_trim_zg_f32;     /**< @details ... */
    F32 factory_trim_dev_xa_f32; /**< @details ... */
    F32 factory_trim_dev_ya_f32; /**< @details ... */
    F32 factory_trim_dev_za_f32; /**< @details ... */
    F32 factory_trim_dev_xg_f32; /**< @details ... */
    F32 factory_trim_dev_yg_f32; /**< @details ... */
    F32 factory_trim_dev_zg_f32; /**< @details ... */

    U8  chip_id_u8;              /**< @details Unique chip id */
    U8  dev_state_u8;            /**< @details ... */
    U8  is_calibrated_u8;        /**< @details ... */
    U8  self_test_passed_u8;     /**< @details main icm-20600 device state */
    U8  self_test_xa_u8;         /**< @details ... */
    U8  self_test_ya_u8;         /**< @details ... */
    U8  self_test_za_u8;         /**< @details ... */
    U8  self_test_xg_u8;         /**< @details ... */
    U8  self_test_yg_u8;         /**< @details ... */
    U8  self_test_zg_u8;         /**< @details ... */

    U16 accel_raw_data_x_u16;    /**< @details ... */
    U16 accel_raw_data_y_u16;    /**< @details ... */
    U16 accel_raw_data_z_u16;    /**< @details ... */
    U16 gyro_raw_data_x_u16;     /**< @details ... */
    U16 gyro_raw_data_y_u16;     /**< @details ... */
    U16 gyro_raw_data_z_u16;     /**< @details ... */
    U16 temperature_raw_u16;     /**< @details Internal core (die) temperature raw data */
} DLG_LOG_ICM_20600_DATA;

/**
 * @details enumerator of ...
 */

typedef struct DLG_LOG_DATA_TAG
{
    F32                    time_stamp_f32;   /**< @details ... */
    DLG_LOG_ICM_20600_DATA icm_20600_data_s; /**< @details ... */
    DLG_LOG_I2C_DATA       i2c_data_s;       /**< @details ... */
} /* __attribute__( ( packed ) ) */ DLG_LOG_DATA;

#endif /* DLG_CORE_TYPES_H_ */
