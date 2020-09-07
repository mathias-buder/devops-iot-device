/**************************************************************************************************

    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: KalFilGen.h $
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

#ifndef _KALFILGEN_H_
#define _KALFILGEN_H_

/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/

/**
 * @details This function initializes model of dynamical system used for Kalman filter
 * @param[out] 'p_Kalman_model_s': pointer to Kalman model
 * @param[in] 'model_type_e': type of Kalman model
 * @param[in] 'input_vector_vf32': mean vector of control input state for initialization with a size of INPUT_DIMENSION
 * @param[in] 'initial_meas_feature_vf32': sample vector of measurement for initialization with a size of MEAS_INIT_DIMENSION
 * @param[in] 'initial_state_covariance_vf32': initial covariance matrix of Kalman state with a size of NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)
 */
void Kalman_filter_init( KALMAN_MODEL_STRUCT* const p_Kalman_model_s,
                         const enum KALMAN_MODEL_TYPES model_type_e,
                         const F32* const input_vector_vf32,
                         const F32* const initial_meas_feature_vf32,
                         const F32* const initial_state_covariance_vf32 );


/**
 * @details:
 * brief description: computes predicted state pdf xi_{k|k} --> xi_{k+1|k}.
 *                    It must be called before the measurement update is called within the same ECU cycle.
 *
 * technical details: temporal propagation of state pdf based upon the stochastic model: xi_{k+1} = f( xi_k, u_k, dt ) + v_k
 *                    f() describes the prediction model and depends upon the old state xi_k, the (optional) control input state u_k, and the time interval dt.
 *                    v_k is the process noise characterizing the uncertainty of the prediction model and is assumed to be a zero mean, white, Gaussian stochastic process.
 *                    The process noise covariance matrix Q is given by Q = G*V*G^T + Q_{stab},
 *                    where V is a diagonal process covariance matrix with 'process_variance_vf32' as diagonal elements, G is the projector of these noise sources onto the state elements (G is computed internally),
 *                    and Q_{stab} offers the possibility to enter additional noise levels; it is recommended to use a diagonal matrix for Q_{stab}.
 *
 * @param[in, out] 'p_Kalman_model_s': pointer to model of dynamical system
 *                                (prediction equation, measurement equation,
 *                                type of dynamical system, dimension of dynamical system, ...)
 *                                used for Kalman filter
 * @param[in] 'p_input_s': pointer to control input state/distribution of "u" of dynamical system
 * @param[in] 'process_variance_vf32': vector of variances of process noise:
 *                                     variances of the derivatives of the highest derivative of each distinct dimension in the state:
 *                                     for example for state (x, x', x'', y, y') the process variance vector consists of the variances of
 *                                     (x''', y'')
 * @param[in] 'stabilizing_noise_covariance_vf32': is equal to Q_{stab} and is a covariance matrix of additional process noise (upper triangular matrix with NR_ELEMENTS_SYM(INPUT_DIMENSION))
 * @param[in] 'dt_f32': time interval for prediction
 */

void Kalman_filter_prediction(  KALMAN_MODEL_STRUCT* const p_Kalman_model_s,
                                const INPUT_STRUCT* const p_input_s,
                                const F32* const process_variance_vf32,
                                const F32* const stabilizing_noise_covariance_vf32,
                                const F32 dt_f32 );

/**
 * @details:
 * brief description: measurement update of state pdf: xi_{k+1|k} --> xi_{k+1|k+1} with new sensor information.
 *                    It can only be called after the prediction xi_{k|k} --> xi_{k+1|k} has been performed.
 *
 * technical details: the measurement equation z_{k+1} = h( xi_{k+1}, u_{k+1} ) + r_{k+1}
 *                    is used to update/correct the predicted state with new sensory information
 *                    Here, h is the measurement equation depending upon the state xi_{k+1} and the control input state u_{k+1} (optional).
 *                    r is a white, zero mean Gaussian stochastic process characterizing the measurement noise.
 *                    It is parametrized by its covariance matrix (contained in 'p_meas_feature_s').
 *
 *                    The update is done in a computationally efficient form with only scalar updates,
 *                    e. g. for a 3-dim. radar measurement with (r, r', phi)
 *                    the update is done sequentially with only scalar updates (first r, then r', then phi).
 *
 *
 * @param[in, out] 'p_Kalman_model_s': pointer to model of dynamical system
 *                                (prediction equation, measurement equation,
 *                                type of dynamical system, dimension of dynamical system, ...)
 *                                used for Kalman filter
 * @param[in] 'p_input_s': pointer to control input state/distribution of "u" of dynamical system
 * @param[in, out] 'p_meas_sequence_s': pointer to sequence of measurement information "z", consisting of:
 *                                vector of scalar measurement samples,
 *                                diagonal covariance matrix: not full covariance matrix but vector of variances (= diagonal elements),
 *                                finally for each element of the vector of measurement samples its type must be specified, e. g. x relative, vx relative, ...
 *                                additionally the analysis information during Kalman update is stored in analysis struct.
 */
void Kalman_filter_update(  KALMAN_MODEL_STRUCT* const p_Kalman_model_s,
                            const INPUT_STRUCT* const p_input_s,
                            MEAS_SEQUENCE* const p_meas_sequence_s );

#endif /* _KALFILGEN_H_ */
