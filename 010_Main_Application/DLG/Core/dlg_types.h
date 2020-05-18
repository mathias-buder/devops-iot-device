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
#include "../Config/dlg_log_cfg.h"

/*************************************************************/
/*      ENUMERATORS                                          */
/*************************************************************/


/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @details enumerator of ...
 */
typedef struct DLG_DATA_TAG
{
    char  file_name_vc[DLG_LOG_MAX_FILE_NAME_LENGTH];
    FILE* p_file_handle;
    U32   num_data_chunk_per_file_u32;
    BOOLEAN logging_enabled_b;
} DLG_DATA;

/**
 * @details enumerator of ...
 */
typedef struct DLG_LOG_DATA_TAG
{
    F32 dlg_time_stamp_f32;                /**< @details ... */
    F32 icm_20600_temperature_deg_f32;     /**< @details Internal core (die) temperature @unit °C */
    F32 icm_20600_factory_trim_xa_f32;     /**< @details ... */
    F32 icm_20600_factory_trim_ya_f32;     /**< @details ... */
    F32 icm_20600_factory_trim_za_f32;     /**< @details ... */
    F32 icm_20600_factory_trim_xg_f32;     /**< @details ... */
    F32 icm_20600_factory_trim_yg_f32;     /**< @details ... */
    F32 icm_20600_factory_trim_zg_f32;     /**< @details ... */
    F32 icm_20600_factory_trim_dev_xa_f32; /**< @details ... */
    F32 icm_20600_factory_trim_dev_ya_f32; /**< @details ... */
    F32 icm_20600_factory_trim_dev_za_f32; /**< @details ... */
    F32 icm_20600_factory_trim_dev_xg_f32; /**< @details ... */
    F32 icm_20600_factory_trim_dev_yg_f32; /**< @details ... */
    F32 icm_20600_factory_trim_dev_zg_f32; /**< @details ... */
    F32 max_30102_tenperature_f32;         /**< @details ... */

    U32 max_30102_red_data_raw_u32;        /**< @details ... */
    U32 max_30102_ir_data_raw_u32;         /**< @details ... */

    U16 icm_20600_temperature_raw_u16;     /**< @details Internal core (die) temperature raw data */

    S16 icm_20600_accel_raw_data_x_s16;    /**< @details ... */
    S16 icm_20600_accel_raw_data_y_s16;    /**< @details ... */
    S16 icm_20600_accel_raw_data_z_s16;    /**< @details ... */
    S16 icm_20600_gyro_raw_data_x_s16;     /**< @details ... */
    S16 icm_20600_gyro_raw_data_y_s16;     /**< @details ... */
    S16 icm_20600_gyro_raw_data_z_s16;     /**< @details ... */
    S16 i2c_error_code_s16;                /**< @details ... */

    U8  icm_20600_chip_id_u8;              /**< @details Unique chip id */
    U8  icm_20600_dev_state_u8;            /**< @details ... */
    U8  icm_20600_is_calibrated_u8;        /**< @details ... */
    U8  icm_20600_self_test_passed_u8;     /**< @details main icm-20600 device state */
    U8  icm_20600_self_test_xa_u8;         /**< @details ... */
    U8  icm_20600_self_test_ya_u8;         /**< @details ... */
    U8  icm_20600_self_test_za_u8;         /**< @details ... */
    U8  icm_20600_self_test_xg_u8;         /**< @details ... */
    U8  icm_20600_self_test_yg_u8;         /**< @details ... */
    U8  icm_20600_self_test_zg_u8;         /**< @details ... */
    U8  i2c_is_error_present_b;            /**< @details ... */
    U8  i2c_access_type_u8;                /**< @details ... */
    U8  i2c_device_addr_u8;                /**< @details ... */
    U8  i2c_register_addr_u8;              /**< @details ... */
    U8  dlg_global_msg_cnt_u8;             /**< @details ... */
    U8  max_30102_int_status_u8;           /**< @details ... */
    U8  max_30102_part_id_u8;              /**< @details ... */
    U8  max_30102_rev_id_u8;               /**< @details ... */
    U8  max_30102_read_ptr_u8;             /**< @details ... */
    U8  max_30102_write_ptr_u8;            /**< @details ... */
    U8  max_30102_mode_u8;                 /**< @details ... */
    U8  max_30102_tenperature_raw_int_u8;  /**< @details ... */
    U8  max_30102_tenperature_frac_u8;     /**< @details ... */

} DLG_LOG_DATA;

#endif /* DLG_CORE_TYPES_H_ */
