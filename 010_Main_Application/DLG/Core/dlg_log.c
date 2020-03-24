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

        @file dlg_log.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "../Config/dlg_log_cfg.h"
#include "dlg_log.h"
#include "dlg_database.h"
#include "dlg_types.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
extern F32 global_time_f32;
/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void dlg_log_create_icm_20600_data_frame( void )
{
    /* DD_ICM_20600_raw_accel ( 0x14 )*/
    dlg_log_database_s.icm_20600_data_s.accel_raw_data_x_u16       = p_dlg_icm_20600_data_s->accel_data_raw_u16[DD_ICM_20600_ACCEL_X];
    dlg_log_database_s.icm_20600_data_s.accel_raw_data_y_u16       = p_dlg_icm_20600_data_s->accel_data_raw_u16[DD_ICM_20600_ACCEL_Y];
    dlg_log_database_s.icm_20600_data_s.accel_raw_data_z_u16       = p_dlg_icm_20600_data_s->accel_data_raw_u16[DD_ICM_20600_ACCEL_Z];

    /* DD_ICM_20600_raw_gyro ( 0x15 ) */
    dlg_log_database_s.icm_20600_data_s.gyro_raw_data_x_u16        = p_dlg_icm_20600_data_s->gyro_data_raw_u16[DD_ICM_20600_GYRO_X];
    dlg_log_database_s.icm_20600_data_s.gyro_raw_data_y_u16        = p_dlg_icm_20600_data_s->gyro_data_raw_u16[DD_ICM_20600_GYRO_Y];
    dlg_log_database_s.icm_20600_data_s.gyro_raw_data_z_u16        = p_dlg_icm_20600_data_s->gyro_data_raw_u16[DD_ICM_20600_GYRO_Z];

    /* DD_ICM_20600_temperature ( 0x16 )*/
    dlg_log_database_s.icm_20600_data_s.temperature_raw_u16        = p_dlg_icm_20600_data_s->temperature_raw_u16;
    dlg_log_database_s.icm_20600_data_s.temperature_deg_f32        = p_dlg_icm_20600_data_s->temperature_deg_f32;

    /* DD_ICM_20600_selt_test ( 0x17 )*/
    dlg_log_database_s.icm_20600_data_s.self_test_xa_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.icm_20600_data_s.self_test_ya_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.icm_20600_data_s.self_test_za_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.icm_20600_data_s.self_test_xg_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.icm_20600_data_s.self_test_yg_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.icm_20600_data_s.self_test_zg_u8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_factory_trim ( 0x18 )*/
    dlg_log_database_s.icm_20600_data_s.factory_trim_xa_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_ya_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_za_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_xg_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.icm_20600_data_s.factory_trim_yg_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.icm_20600_data_s.factory_trim_zg_f32        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_factory_trim_dev ( 0x19 )*/
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_xa_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_ya_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_za_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_xg_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_yg_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_zg_f32    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_general ( 0x1A) */
    dlg_log_database_s.icm_20600_data_s.chip_id_u8                 = p_dlg_icm_20600_data_s->chip_id_u8;
    dlg_log_database_s.icm_20600_data_s.dev_state_u8               = (U8) p_dlg_icm_20600_data_s->dev_state_s.state_e;
    dlg_log_database_s.icm_20600_data_s.is_calibrated_u8           = p_dlg_icm_20600_data_s->is_calibrated_b;
    dlg_log_database_s.icm_20600_data_s.self_test_passed_u8        = p_dlg_icm_20600_data_s->self_test_passed_b;

    /* DD_ICM_20600_general ( 0x1B) */
    dlg_log_database_s.icm_20600_data_s.quaternion_q1_f32          = p_dlg_icm_20600_data_s->Quaternion_s.Q1_f32;
    dlg_log_database_s.icm_20600_data_s.quaternion_q2_f32          = p_dlg_icm_20600_data_s->Quaternion_s.Q2_f32;
    dlg_log_database_s.icm_20600_data_s.quaternion_q3_f32          = p_dlg_icm_20600_data_s->Quaternion_s.Q3_f32;
    dlg_log_database_s.icm_20600_data_s.quaternion_q4_f32          = p_dlg_icm_20600_data_s->Quaternion_s.Q4_f32;

    dlg_log_database_s.time_f32 = global_time_f32;
}
