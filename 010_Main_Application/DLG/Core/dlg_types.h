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
    /* DD_ICM_20600_raw_accel ( 0x14 )*/
    U32 accel_msg_id_b8            :  8U;
    U32 accel_raw_data_x_b16       : 16U; /** @details Acceleration raw data */
    U32 accel_raw_data_y_b16       : 16U; /** @details Acceleration raw data */
    U32 accel_raw_data_z_b16       : 16U; /** @details Acceleration raw data */

    /* DD_ICM_20600_raw_gyro ( 0x15 ) */
    U32 gyro_msg_id_b8             :  8U;
    U32 gyro_raw_data_x_b16        : 16U; /** @details Acceleration raw data */
    U32 gyro_raw_data_y_b16        : 16U; /** @details Acceleration raw data */
    U32 gyro_raw_data_z_b16        : 16U; /** @details Acceleration raw data */

    /* DD_ICM_20600_temperature ( 0x16 )*/
    U32 temperature_msg_id_b8      :  8U;
    U32 temperature_raw_b16        : 16U; /** @details Internal core (die) temperature raw data */
    U32 temperature_deg_b10        : 10U; /** @details Internal core (die) temperature @unit °C */

    /* DD_ICM_20600_selt_test ( 0x17 )*/
    U32 self_test_msg_id_b8        :  8U;
    U32 self_test_xa_b8            :  8U; /** @details Acceleration raw data */
    U32 self_test_ya_b8            :  8U; /** @details Acceleration raw data */
    U32 self_test_za_b8            :  8U; /** @details Acceleration raw data */
    U32 self_test_xg_b8            :  8U; /** @details Acceleration raw data */
    U32 self_test_yg_b8            :  8U; /** @details Acceleration raw data */
    U32 self_test_zg_b8            :  8U; /** @details Acceleration raw data */

    /* DD_ICM_20600_factory_trim ( 0x18 )*/
    U32 factory_trim_msg_id_b8     :  8U;
    U32 factory_trim_xa_b10        : 10U;
    U32 factory_trim_ya_b10        : 10U;
    U32 factory_trim_za_b10        : 10U;
    U32 factory_trim_xg_b10        : 10U;
    U32 factory_trim_yg_b10        : 10U;
    U32 factory_trim_zg_b10        : 10U;

    /* DD_ICM_20600_factory_trim_dev ( 0x19 )*/
    U32 factory_trim_dev_msg_id_b8 :  8U;
    U32 factory_trim_dev_xa_b10    : 10U;
    U32 factory_trim_dev_ya_b10    : 10U;
    U32 factory_trim_dev_za_b10    : 10U;
    U32 factory_trim_dev_xg_b10    : 10U;
    U32 factory_trim_dev_yg_b10    : 10U;
    U32 factory_trim_dev_zg_b10    : 10U;

    /* DD_ICM_20600_general ( 0x1A) */
    U32 general_msg_id_b8          :  8U;
    U32 chip_id_b8                 :  8U; /** @details Unique chip id */
    U32 dev_state_b8               :  8U;
    U32 is_calibrated_b1           :  1U;
    U32 self_test_passed_b1        :  1U; /** @details main icm-20600 device state */

    /* DD_ICM_20600_quaternion ( 0x1B )*/
    U32 quaternion_msg_id_b8       :  8U;
    U32 quaternion_q1_b16          : 16U; /** @details Acceleration raw data */
    U32 quaternion_q2_b16          : 16U; /** @details Acceleration raw data */
    U32 quaternion_q3_b16          : 16U; /** @details Acceleration raw data */
    U32 quaternion_q4_b16          : 16U; /** @details Acceleration raw data */

} DLG_LOG_ICM_20600_DATA;




typedef struct DLG_LOG_DATA_TAG
{
    DLG_LOG_ICM_20600_DATA icm_20600_data_s;
} DLG_LOG_DATA;

#endif /* SENSE_CORE_TYPES_H_ */
