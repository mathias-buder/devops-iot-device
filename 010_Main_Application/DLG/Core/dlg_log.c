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
    dlg_log_database_s.icm_20600_data_s.accel_msg_id_b8            = DLG_ICM_20600_ACCEL_ID;
    dlg_log_database_s.icm_20600_data_s.accel_raw_data_x_b16       = p_dlg_icm_20600_data_s->accel_data_raw_u16[DD_ICM_20600_ACCEL_X];
    dlg_log_database_s.icm_20600_data_s.accel_raw_data_y_b16       = p_dlg_icm_20600_data_s->accel_data_raw_u16[DD_ICM_20600_ACCEL_Y];
    dlg_log_database_s.icm_20600_data_s.accel_raw_data_z_b16       = p_dlg_icm_20600_data_s->accel_data_raw_u16[DD_ICM_20600_ACCEL_Z];
    dlg_log_database_s.icm_20600_data_s.gyro_msg_id_b8             = DLG_ICM_20600_GYRO_ID;

    /* DD_ICM_20600_raw_gyro ( 0x15 ) */
    dlg_log_database_s.icm_20600_data_s.gyro_raw_data_x_b16        = p_dlg_icm_20600_data_s->gyro_data_raw_u16[DD_ICM_20600_GYRO_X];
    dlg_log_database_s.icm_20600_data_s.gyro_raw_data_y_b16        = p_dlg_icm_20600_data_s->gyro_data_raw_u16[DD_ICM_20600_GYRO_Y];
    dlg_log_database_s.icm_20600_data_s.gyro_raw_data_z_b16        = p_dlg_icm_20600_data_s->gyro_data_raw_u16[DD_ICM_20600_GYRO_Z];
    dlg_log_database_s.icm_20600_data_s.temperature_msg_id_b8      = DLG_ICM_20600_TEMPERATURE_ID;

    /* DD_ICM_20600_temperature ( 0x16 )*/
    dlg_log_database_s.icm_20600_data_s.temperature_raw_b16        = p_dlg_icm_20600_data_s->temperature_raw_u16;
    dlg_log_database_s.icm_20600_data_s.temperature_deg_b10        = ( p_dlg_icm_20600_data_s->temperature_deg_f32 + 10.0F ) * 10.0F;
    dlg_log_database_s.icm_20600_data_s.self_test_msg_id_b8        = DLG_ICM_20600_SELF_TEST_ID;

    /* DD_ICM_20600_selt_test ( 0x17 )*/
    dlg_log_database_s.icm_20600_data_s.self_test_xa_b8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XA];
    dlg_log_database_s.icm_20600_data_s.self_test_ya_b8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YA];
    dlg_log_database_s.icm_20600_data_s.self_test_za_b8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZA];
    dlg_log_database_s.icm_20600_data_s.self_test_xg_b8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_XG];
    dlg_log_database_s.icm_20600_data_s.self_test_yg_b8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_YG];
    dlg_log_database_s.icm_20600_data_s.self_test_zg_b8            = p_dlg_icm_20600_data_s->self_test_vu8[DD_ICM_20600_SELF_TEST_ZG];

    /* DD_ICM_20600_factory_trim ( 0x18 )*/
    dlg_log_database_s.icm_20600_data_s.factory_trim_msg_id_b8     = DLG_ICM_20600_FACTORY_TRIM_ID;
    dlg_log_database_s.icm_20600_data_s.factory_trim_xa_b10        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XA] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_ya_b10        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YA] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_za_b10        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZA] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_xg_b10        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_XG] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_yg_b10        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_YG] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_zg_b10        = p_dlg_icm_20600_data_s->factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZG] * 100.0F;

    /* DD_ICM_20600_factory_trim_dev ( 0x19 )*/
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_msg_id_b8 = DLG_ICM_20600_FACTORY_TRIM_DEV_ID;
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_xa_b10    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XA] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_ya_b10    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YA] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_za_b10    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZA] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_xg_b10    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_XG] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_yg_b10    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_YG] * 100.0F;
    dlg_log_database_s.icm_20600_data_s.factory_trim_dev_zg_b10    = p_dlg_icm_20600_data_s->fac_trim_deviation_vf32[DD_ICM_20600_SELF_TEST_ZG] * 100.0F;

    /* DD_ICM_20600_general ( 0x1A) */
    dlg_log_database_s.icm_20600_data_s.general_msg_id_b8          = DLG_ICM_20600_GENERAL_ID;
    dlg_log_database_s.icm_20600_data_s.chip_id_b8                 = p_dlg_icm_20600_data_s->chip_id_u8;
    dlg_log_database_s.icm_20600_data_s.dev_state_b8               = (U8) p_dlg_icm_20600_data_s->dev_state_s.state_e;
    dlg_log_database_s.icm_20600_data_s.is_calibrated_b1           = p_dlg_icm_20600_data_s->is_calibrated_b;
    dlg_log_database_s.icm_20600_data_s.self_test_passed_b1        = p_dlg_icm_20600_data_s->self_test_passed_b;

    /* DD_ICM_20600_quaternion ( 0x1B )*/
    dlg_log_database_s.icm_20600_data_s.quaternion_msg_id_b8       = DLG_ICM_20600_QUATERNION_ID;
    dlg_log_database_s.icm_20600_data_s.quaternion_q1_b16          = ( U16 )( p_dlg_icm_20600_data_s->Quaternion_s.Q1_f32 * 1000.0F );
    dlg_log_database_s.icm_20600_data_s.quaternion_q2_b16          = ( U16 )( p_dlg_icm_20600_data_s->Quaternion_s.Q2_f32 * 1000.0F );
    dlg_log_database_s.icm_20600_data_s.quaternion_q3_b16          = ( U16 )( p_dlg_icm_20600_data_s->Quaternion_s.Q3_f32 * 1000.0F );
    dlg_log_database_s.icm_20600_data_s.quaternion_q4_b16          = ( U16 )( p_dlg_icm_20600_data_s->Quaternion_s.Q4_f32 * 1000.0F );
}
