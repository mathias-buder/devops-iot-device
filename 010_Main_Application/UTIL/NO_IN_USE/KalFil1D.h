/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: KalFil1D.h $
        $Revision: 1.1 $
        $Date: 2018/05/28 16:34:24CEST $

        TRW Ltd owns the copyright in this document and associated
        documents and all rights are reserved.  These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of TRW
        Ltd. Any copy of this drawing or document made by any method
        must also include a copy of this legend.

    $CopyrightDate: (c) ZF 2019 $

***************************************************************************************************/

/**
 * @file
 * @brief general details of file
 */

#ifndef _KALFIL1D_H_
#define _KALFIL1D_H_

/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/

/**
 * @details
 * @param[in, out] 'p_Kalman_model_s': pointer to the model used for 1D Kalman filter.
 * @param[in] 'p_initial_state_s': pointer to the initial state for the 1D Kalman filter.
 */
void Kalman_filter_init_OneDim( KALMAN_MODEL_ONEDIM_STRUCT* const p_Kalman_model_s,
                                const SCALAR_STATE_STRUCT* const p_initial_state_s );

/**
 * @details
 * @param[in, out] 'p_Kalman_model_s': pointer to the model used for 1D Kalman filter.
 * @param[in] 'p_input_s': pointer to control input
 * @param[in] 'process_variance_vf32': variance of process noise
 */
void Kalman_filter_prediction_OneDim( KALMAN_MODEL_ONEDIM_STRUCT* const p_Kalman_model_s,
                                      const SCALAR_STATE_STRUCT* const p_input_s,
                                      const F32 process_variance_f32 );

/**
 * @details
 * @param[in, out] 'p_Kalman_model_s': pointer to the model used for 1D Kalman filter.
 * @param[in] 'p_input_s': pointer to control input
 * @param[in] 'p_meas_feature_s': pointer to measurement information
 */
void Kalman_filter_update_OneDim( KALMAN_MODEL_ONEDIM_STRUCT* const p_Kalman_model_s,
                                  const SCALAR_STATE_STRUCT* const p_input_s,
                                  const SCALAR_MEAS_STRUCT* const p_meas_feature_s);

/**
 * @details Fuses two measurements of an scalar State.
 *          Updating an Kalman-filter with the fused measurement is equal to two successive updates.
 * @attention 'sample', 'variance' and 'valid-flag' will be considered. Meas_type, sensor_type, meas_origin will be ignored.
 * @param[in] 'p_meas_1_s' first measurement to fuse
 * @param[in] 'p_meas_2_s' second measurement to fuse
 * @return fused measurement.
 */
SCALAR_MEAS_STRUCT Kalman_filter_fuse_scalar_measurement(const SCALAR_MEAS_STRUCT* p_meas_1_s,
                                                         const SCALAR_MEAS_STRUCT* p_meas_2_s);

#endif /* _KALFIL1D_H_ */
