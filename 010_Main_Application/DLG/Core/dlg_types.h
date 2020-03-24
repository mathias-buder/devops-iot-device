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
typedef struct DLG_LOG_ICM_20600_DATA_TAG
{
    F32 temperature_deg_f32;        /** @details Internal core (die) temperature @unit °C */
    F32 factory_trim_xa_f32;        /** @details Acceleration raw data */
    F32 factory_trim_ya_f32;        /** @details Acceleration raw data */
    F32 factory_trim_za_f32;        /** @details Acceleration raw data */
    F32 factory_trim_xg_f32;        /** @details Acceleration raw data */
    F32 factory_trim_yg_f32;        /** @details Acceleration raw data */
    F32 factory_trim_zg_f32;        /** @details Acceleration raw data */
    F32 factory_trim_dev_xa_f32;    /** @details Acceleration raw data */
    F32 factory_trim_dev_ya_f32;    /** @details Acceleration raw data */
    F32 factory_trim_dev_za_f32;    /** @details Acceleration raw data */
    F32 factory_trim_dev_xg_f32;    /** @details Acceleration raw data */
    F32 factory_trim_dev_yg_f32;    /** @details Acceleration raw data */
    F32 factory_trim_dev_zg_f32;    /** @details Acceleration raw data */
    F32 quaternion_q1_f32;          /** @details Acceleration raw data */
    F32 quaternion_q2_f32;          /** @details Acceleration raw data */
    F32 quaternion_q3_f32;          /** @details Acceleration raw data */
    F32 quaternion_q4_f32;          /** @details Acceleration raw data */

    U8  chip_id_u8;                 /** @details Unique chip id */
    U8  dev_state_u8;               /** @details Acceleration raw data */
    U8  is_calibrated_u8;           /** @details Acceleration raw data */
    U8  self_test_passed_u8;        /** @details main icm-20600 device state */
    U8  self_test_xa_u8;            /** @details Acceleration raw data */
    U8  self_test_ya_u8;            /** @details Acceleration raw data */
    U8  self_test_za_u8;            /** @details Acceleration raw data */
    U8  self_test_xg_u8;            /** @details Acceleration raw data */
    U8  self_test_yg_u8;            /** @details Acceleration raw data */
    U8  self_test_zg_u8;            /** @details Acceleration raw data */

    U16 accel_raw_data_x_u16;       /** @details Acceleration raw data */
    U16 accel_raw_data_y_u16;       /** @details Acceleration raw data */
    U16 accel_raw_data_z_u16;       /** @details Acceleration raw data */
    U16 gyro_raw_data_x_u16;        /** @details Acceleration raw data */
    U16 gyro_raw_data_y_u16;        /** @details Acceleration raw data */
    U16 gyro_raw_data_z_u16;        /** @details Acceleration raw data */
    U16 temperature_raw_u16;        /** @details Internal core (die) temperature raw data */

} DLG_LOG_ICM_20600_DATA;


//typedef struct DLG_LOG_DATA_TAG
//{
//    DLG_LOG_ICM_20600_DATA icm_20600_data_s;
//} __attribute__( ( packed ) ) DLG_LOG_DATA;

typedef struct DLG_LOG_DATA_TAG
{
    DLG_LOG_ICM_20600_DATA icm_20600_data_s;
    F32                    time_f32;
} DLG_LOG_DATA;

#endif /* SENSE_CORE_TYPES_H_ */
