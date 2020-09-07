
/* PRQA S 0292 17 */
/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: ModTypes.h $
        $Revision: 1.1 $
        $Date: 2018/05/28 16:34:25CEST $

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

#ifndef _MODTYPES_H_
#define _MODTYPES_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../FOUNDATION/types.h"
#include "../UTIL/util_types.h"
#include "../UTIL/matrix_indices.h"
#include "../UTIL/KalFilChk_Cfg.h"

/*************************************************************/
/*      DEFINES                                              */
/*************************************************************/

#define MINIMAL_DISTANCE_MEAS_MODEL     (0.01F)     /* [m] */
#define MEAS_SEQUENCE_DIMENSION         (6U)        /* maximal number of scalar measurements used for sequential updating of Kalman filter*/
#define STATE_DIMENSION_MAX             5U          /* maximal dimension of state mean vector, this must be the maximum of all
                                                       possible models (currently max of STATE_DIMENSION_CAA and STATE_DIMENSION_CACV)  */


/*************************************************************/
/*      DATATYPES                                            */
/*************************************************************/

/*  define state indices
 *  for CAA model
 *  coordinate conventions: the state vector can be used with both left- and right-handed coordinates:
 *                          left-handed coordinates:  y is positive to the right and all angles are positive in clock-wise direction
 *                          right-handed coordinates: y is positive to the left and all angles are positive in counter-clockwise direction
 */
enum ENV_CAA_STATE_INDICES
{
    INDEX_X_REL,           /*  relative x position [m] in host vehicle coordinates */
    INDEX_Y_REL,           /*  relative y position [m] in host vehicle coordinates */
    INDEX_PSI_REL,         /*  angle of speed over ground (V_ABS, see below) with respect to host vehicle longitudinal axis [rad] */
    INDEX_V_ABS,           /*  magnitude of speed over ground, can be positive or negative [m/s]
                               (can only be used with combination of INDEX_PSI_REL, like vx_abs = v_abs * cos(psi_rel), vy_abs = v_abs * sin(psi_rel) */
    INDEX_A_ABS,           /*  magnitude of acceleration over ground, can be positive or negative [m/s^2]
                               (can only be used with combination of INDEX_PSI_REL, like ax_abs ~ a_abs * cos(psi_rel), ay_abs ~ a_abs * sin(psi_rel) */
    STATE_DIMENSION_CAA    /*  dimension of state, this MUST be the last entry! */
};

/*  define state indices
 *  for CACV model
 *  the first two indices are reused from CAA model
 */
enum ENV_CACV_STATE_INDICES
{
    INDEX_VX_REL = 2,      /*  relative longitudinal speed in host vehicle coordinates */
    INDEX_VY_REL,          /*  relative lateral speed in host vehicle coordinates */
    INDEX_AX_REL,          /*  relative longitudinal acceleration in host vehicle coordinates */
    STATE_DIMENSION_CACV   /*  dimension of state, this MUST be the last entry! */
};

/*  define process noise indices for CAA model */
enum ENV_CAA_PROCESS_NOISE_INDICES
{
    INDEX_CAA_PROCESS_NOISE_A_ABS,
    INDEX_CAA_PROCESS_NOISE_PSI_REL,
    PROCESS_NOISE_DIMENSION_CAA                 /*  dimension of process noise, this MUST be the last entry! */
};

/*  define process noise indices for CACV model */
enum ENV_CACV_PROCESS_NOISE_INDICES
{
    INDEX_CACV_PROCESS_NOISE_AX,
    INDEX_CACV_PROCESS_NOISE_VY,
    PROCESS_NOISE_DIMENSION_CACV                 /*  dimension of process noise, this MUST be the last entry! */
};

/*  define input indices */
enum ENV_INPUT_INDICES
{
    INDEX_INPUT_YAWRATE,         /*  ego yawrate [rad/s], mathematically positive */
    INDEX_INPUT_SPEED,           /*  ego speed [m/s] */
    INPUT_DIMENSION              /*  dimension of input state, this MUST be the last entry! */
};

typedef enum SENSOR_TYPES_TAG
{
    MEAS_SENSOR_RADAR,
    MEAS_SENSOR_VIDEO,
    MEAS_SENSOR_TYPE_DIMENSION
}SENSOR_TYPES;

/* define the measurement type, this tells the Kalman filter how to interpret the measurement signal
 * in the update step */
typedef enum MEASUREMENT_TYPES_TAG
{
    MEAS_TYPE_UNKNOWN,          /* unknown type as default */
    MEAS_X_REL,                 /* relative longitudinal position [m] */
    MEAS_TAN_PHI_REL,           /* tangent of angle of relative position [rad] */
    MEAS_VX_REL,                /* relative longitudinal velocity [m/s] */
    MEAS_Y_REL,                 /* relative lateral position [m]*/
    MEAS_R_REL,                 /* relative radial distance [m]*/
    MEAS_VR_REL,                /* relative radial velocity [m/s]*/
    MEAS_DPHI_REL,              /* angular speed of relative position [rad/s] */
    MEAS_TYPE_DIMENSION         /* number of possible measurement types */
}MEASUREMENT_TYPES;

/* define model type indices*/
enum KALMAN_MODEL_TYPES
{
    MODEL_UNDEFINED,              /* */
    MODEL_CAA,                    /* constant angle and acceleration model (over ground) */
    MODEL_CACV,                   /* constant acceleration (x) and constant velocity (y) model (relative to host vehicle) */
    MODEL_TYPE_DIMENSION          /* number of possible Kalman filter models, this MUST be the last entry!*/
};

/* define measurement indices for initialization of state, the four signals below can be obtained from video measurements */
enum MEAS_INIT_TYPES
{
    MEAS_INIT_X_REL,              /* [m] */
    MEAS_INIT_VX_REL,             /* [m/s] */
    MEAS_INIT_PHI_REL,            /* [rad] */
    MEAS_INIT_PHI_DOT_REL,        /* [rad/s] */
    MEAS_INIT_DIMENSION           /* number of measurements used for state initialization */
};

typedef struct INPUT_STRUCT_TAG   /*  input struct */
{
    /* mean of input vector */
    F32            mean_vf32[INPUT_DIMENSION];
    /* state covariance matrix with only independent entries (upper triangular matrix with NR_ELEMENTS_SYM(INPUT_DIMENSION) elements */
    F32            covariance_vf32[NR_ELEMENTS_SYM(INPUT_DIMENSION)];
} INPUT_STRUCT;

typedef struct STATE_STRUCT_TAG   /*  state struct */
{
    /* mean of state vector */
    F32            mean_vf32[STATE_DIMENSION_MAX];
    /* state covariance matrix with only independent entries (upper triangular matrix with NR_ELEMENTS_SYM(STATE_DIMENSION_MAX) elements)*/
    F32            covariance_vf32[NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)];
} STATE_STRUCT;

typedef struct SCALAR_STATE_STRUCT_TAG   /*  scalar state struct */
{
    /*  state vector */
    F32            mean_f32;
    /*  state variance */
    F32            variance_f32;
} SCALAR_STATE_STRUCT;

typedef struct SCALAR_MEAS_STRUCT_TAG   /*  scalar measurement struct */
{
    /* scalar measurement sample */
    F32             sample_f32;
    /* variance of scalar measurement */
    F32             variance_f32;
    /* measurement valid type, either invalid or valid*/
    BOOLEAN         measurement_valid_flag_b;
    /* measurement sensor type*/
    SENSOR_TYPES    sensor_type_e;
    /* measurement type*/
    MEASUREMENT_TYPES meas_type_e;

    /* Cartesian position of measurement origin with respect to filter origin */
    struct {
        F32 x_f32;
        F32 y_f32;
    }measurement_origin_s;

} SCALAR_MEAS_STRUCT;

typedef struct INNOVATION_SEQUENCE_TAG
{
    /** @details current value */
    F32                 current_value_f32;

    /** @details moving average */
    F32                 moving_average_f32;

    /** @details ring buffer */
    F32                 buffer_vf32[DIM_INNOVATION_SEQUENCE_BACKGROUND_BUFFER];
    CACHED_RING_STRUCT  ring_struct_s;

}INNOVATION_SEQUENCE;

typedef struct INNOVATION_CHECK_STRUCT_TAG
{
    /** @details sequence of NIS (Normalized Innovation Squared) */
    INNOVATION_SEQUENCE     NIS_sequence_s;

    /** @details NIS confidence bound */
    CONFIDENCE_BOUND        NIS_confidence_bound_s;

    /** @details NIS test result trilean */
    TEST_RESULT_TRILEAN     NIS_test_result_e;

    /** @details sequence of innovation */
#ifdef simulator
    INNOVATION_SEQUENCE     innovation_sequence_s;
#endif

} INNOVATION_CHECK_STRUCT;

typedef struct SCALAR_MEAS_UPDATE_ANALYSIS_TAG
{
    /** @details innovation between predicted state and measurement */
    F32             innovation_value_f32;

    /** @details variance of innovation between predicted state and measurement */
    F32             innovation_variance_f32;

#ifdef simulator
    /** @details predicted feature vector */
    F32             pred_feature_vector_f32;

    /** @details predicted feature variance */
    F32             pred_feature_variance_f32;

    /** @details predicted feature input noise. they are the diagonal elements from cov. matrix for sequential updating */
    F32             pred_feature_input_noise_f32;

    /** @details Kalman gain column array for all state vector */
    F32             Kalman_gain_column_vf32[STATE_DIMENSION_MAX];

    /** @details difference of state vector caused by each update step */
    F32             delta_vf32[STATE_DIMENSION_MAX];

    /** @details state covariance before each update step */
    F32             pre_update_covariance_vf32[NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)];

    /** @details state covariance after each update step */
    F32             post_update_covariance_vf32[NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)];

    /** @details C row array for all state vector */
    F32             C_row_vf32[STATE_DIMENSION_MAX];
#endif

} SCALAR_MEAS_UPDATE_ANALYSIS;


typedef struct MEAS_SEQUENCE_TAG
{
    /** @details number of measurements in sequence */
    U8                          number_measurements_u8;

    /** @details array of measurements in sequence */
    SCALAR_MEAS_STRUCT          meas_vs[MEAS_SEQUENCE_DIMENSION];

    /** @details array of analysis information of each Kalman scalar update */
    SCALAR_MEAS_UPDATE_ANALYSIS meas_update_analysis_vs[MEAS_SEQUENCE_DIMENSION];
} MEAS_SEQUENCE;

typedef struct KALMAN_MODEL_STRUCT_TAG
{
    /* Functions required in the Kalman prediction: */
    /**
     * @details: This function performs prediction of mean of Kalman state using prediction function f.
     *           Note that the input state must be in the same left-handed or in right-handed coordinates as the state vector.
     *
     * @param[in,out] state_vector_vf32 is mean vector of Kalman state with a size of STATE_DIMENSION_MAX
     * @param[in] input_vector_vf32 is mean vector of control input state with a size of INPUT_DIMENSION
     * @param[in] dt_f32 is the time interval dt
     */
    void (*set_predicted_state_vector)(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32);

    /**
     * @details: This function gets the A matrix (sometimes also called F) which is the Jacobi matrix of f with respect to the state. This is required for the covariance propagation in the Kalman prediction.
     *           Note that the input vector must be in the same left-handed or in right-handed coordinates as the state vector.
     *
     * @param[out] A_matrix_vf32 is the A matrix (full matrix with a size of NR_ELEMENTS(STATE_DIMENSION_MAX) )
     * @param[in] state_vector_vf32 is mean vector of Kalman state with a size of STATE_DIMENSION_MAX
     * @param[in] input_vector_vf32 is mean vector of control input state with a size of INPUT_DIMENSION
     * @param[in] dt_f32 is the time interval dt
     */
    void (*get_A_matrix)(F32* const A_matrix_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32);

    /**
     * @details: This function gets the process noise matrix Q. This is required for the covariance propagation in the Kalman prediction.
     *           Note that the input state must be in the same left-handed or in right-handed coordinates as the state vector.
     *
     * @param[out] Q_matrix_vf32 is the process noise matrix Q (upper triangular matrix with a size of NR_ELEMENTS_SYM(STATE_DIMENSION_MAX))
     * @param[in] state_vector_vf32 is mean vector of Kalman state with a size of STATE_DIMENSION_MAX
     * @param[in] p_input_state_s is pointer to input state struct of INPUT_STRUCT containing mean vector and covariance matrix
     * @param[in] process_variance_vf32 is vector of variances of process noise with a size of the maximal number of independent dimensions (in our case 2)
     * @param[in] stabilizing_noise_covariance_vf32 is a covariance matrix for additional process noise (upper triangular matrix with a size of NR_ELEMENTS_SYM(STATE_DIMENSION_MAX))
     * @param[in] dt_f32 is the time interval dt
     */
    void (*get_process_noise_matrix)(F32* const Q_matrix_vf32, const F32* const state_vector_vf32, const INPUT_STRUCT* const p_input_state_s, const F32* const process_variance_vf32, const F32* const stabilizing_noise_covariance_vf32, const F32 dt_f32);

    /* Functions required in the Kalman update: */
    /**
     * @details: This function gets the C matrix (sometimes also called H) for each scalar measurement type which is the Jacobi matrix of h with respect to the state, this is required for the Kalman update.
     *           Since a vector measurement is split up in scalar measurements C is a row vector here
     *           Note that the input vector must be in the same left-handed or in right-handed coordinates as the state vector.
     *
     * @param[out] C_row_vf32 is row vector with a length of STATE_DIMENSION_MAX
     * @param[in] state_vector_vf32 is mean vector of Kalman state with a size of STATE_DIMENSION_MAX
     * @param[in] input_vector_vf32 is mean vector of control input state with a size of INPUT_DIMENSION
     * @param[in] meas_type_e is the scalar measurement type
     */
    void (*get_C_row)(F32* const C_row_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e);

    /**
     * @details: This function performs the transformation of the predicted state to predicted measurement, this is required for the Kalman update to compute the innovation
     *            Note that the input vector must be in the same left-handed or in right-handed coordinates as the state vector.
     *
     * @param[out] p_pred_feature_f32 is the scalar mean value of predicted measurement of type meas_type_e
     * @param[in] state_vector_vf32 is mean vector of Kalman state with a size of STATE_DIMENSION_MAX
     * @param[in] input_vector_vf32 is mean vector of control input state with a size of INPUT_DIMENSION
     * @param[in] meas_type_e is the scalar measurement type
     */
    void (*get_predicted_feature)(F32* const p_pred_feature_f32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e);

    /**
     * @details: This function translates the state vector in given (measurement) coordinate origin
     * @param[out] translated_state_vector_vf32 is state vector in given coordinate origin
     * @param[in] state_vector_vf32 is state vector in filter coordinate origin
     * @param[in] measurement_origin_x_relative_f32 is x position of given coordinate origin with respect to filter origin
     * @param[in] measurement_origin_y_relative_f32 is y position of given coordinate origin with respect to filter origin
     */
    void (*get_translated_state_vector)(F32* const translated_state_vector_vf32, const F32* const state_vector_vf32, const F32 measurement_origin_x_relative_f32, const F32 measurement_origin_y_relative_f32);

    /**
     * @details: This function gets the contribution to the noise variance of the predicted measurement from the input/control state
     *           Note that the input vector must be in the same left-handed or in right-handed coordinates as the state vector.
     *
     * @param[out] p_pred_feature_input_noise_f32 is the scalar noise variance for the predicted measurement of type meas_type_e
     * @param[in] state_vector_vf32 is mean vector of Kalman state with a size of STATE_DIMENSION_MAX
     * @param[in] input_vector_vf32 is mean vector of control input state with a size of INPUT_DIMENSION
     * @param[in] meas_type_e is the scalar measurement type
     */
    void (*get_predicted_feature_input_noise)(F32* const p_pred_feature_input_noise_f32, const F32* const state_vector_vf32, const INPUT_STRUCT* const p_input_state_s, const MEASUREMENT_TYPES meas_type_e);

    /**
     * @details: This function initializes the state based upon the measurements provided in 'meas_feature_vf32', this can be used for video-based measurements.
     *           Actually an initialization function is not directly part of the Kalman filter and should be declared outside of this structure.
     *           Note that the measurement and input vector must be in the same left-handed or in right-handed coordinates as the state vector.
     *
     * @param[out] state_vector_vf32 is mean vector of Kalman state with a size of STATE_DIMENSION_MAX
     * @param[in] input_vector_vf32 is mean vector of control input state with a size of INPUT_DIMENSION
     * @param[in] meas_feature_vf32 is vector of measurement values for initialization with a size of MEAS_INIT_DIMENSION
     */
    void (*init_state_vector)(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32* const meas_feature_vf32);

    /** @details: model_type_u8 contains values of enum KALMAN_MODEL_TYPES. */
    U8  model_type_u8;

    /** @details: model_state_dim_u8 describes the dimension of state,
     * last entry of ENV_CAA_STATE_INDICES (=STATE_DIMENSION_CAA) or ENV_CACV_STATE_INDICES (=STATE_DIMENSION_CACV) */
    U8  model_state_dim_u8;

    /** @details: model_state_dim_u8 describes the dimension of input. */
    U8  model_input_dim_u8;

    /** @details: current Kalman state */
    STATE_STRUCT    state_s;

    BOOLEAN         has_been_initialised_b;

    BOOLEAN         is_in_error_state_b;

#ifdef simulator
    /* analysis variables for Kalman prediction */

    /** @details Input struct used for prediction or update */
    INPUT_STRUCT    input_for_prediction_s;
    INPUT_STRUCT    input_for_update_s;

    /** @details Kalman state before prediction */
    STATE_STRUCT    state_previous_s;

    /** @details Kalman state after prediction */
    STATE_STRUCT    state_predicted_s;

    F32             process_variance_vf32[INPUT_DIMENSION];

    /** @details stabilizing noise covariance matrix NR_ELEMENTS_SYM(STATE_DIMENSION_MAX) */
    F32             stabilizing_noise_covariance_matrix_vf32[NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)];

    /** @details pure prediction time (time to last sensor update) */
    F32             pure_prediction_time_f32;

    /** @details Q_vf32 is the process noise matrix. It is a symmetric matrix and will be completely rewritten during Kalman prediction */
    F32             Q_matrix_vf32[NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)];

    /** @details A_vf32 is the Jacobi matrix. It is a full matrix and will be completely rewritten during Kalman prediction */
    F32             A_matrix_vf32[NR_ELEMENTS(STATE_DIMENSION_MAX, STATE_DIMENSION_MAX)];

#endif

} KALMAN_MODEL_STRUCT;


typedef struct KALMAN_MODEL_ONEDIM_STRUCT_TAG
{
    /** @details: current Kalman state */
    SCALAR_STATE_STRUCT state_s;
    BOOLEAN             has_been_initialised_b;
    BOOLEAN             is_in_error_state_b;

    /* analysis variables for Kalman update */

    /** @details measurement related analysis variables for the Kalman update */
    SCALAR_MEAS_UPDATE_ANALYSIS meas_update_analysis_s;

#ifdef simulator
    /* analysis variables for Kalman prediction */

    /** @details Input struct used for prediction or update */
    SCALAR_STATE_STRUCT input_for_prediction_s;
    SCALAR_STATE_STRUCT input_for_update_s;

    /** @details Kalman state before prediction */
    SCALAR_STATE_STRUCT state_previous_s;

    /** @details Kalman state after prediction */
    SCALAR_STATE_STRUCT state_predicted_s;

    F32                 process_variance_f32;

    /* analysis variables for Kalman update */

    /** @details last measurement feature for the Kalman update */
    SCALAR_MEAS_STRUCT  meas_feature_s;
#endif

} KALMAN_MODEL_ONEDIM_STRUCT;

#endif /* _MODTYPES_H_ */
