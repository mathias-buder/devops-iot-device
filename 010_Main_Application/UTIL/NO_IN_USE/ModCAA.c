
/* PRQA S 0292 17 */
/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: ModCAA.c $
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

/**********************************/
/* GLOBAL QAC WARNING SUPPRESSION */
/**********************************/

/*  DEVIATION:     0492
    DESCRIPTION:   Array subscripting applied to a function parameter declared as a pointer.
    JUSTIFICATION: This file contains well used legacy code. The QAC help text (below) associated
    with this warning also implies that it is a question of company coding standards rather than
    outright 'bad' code:-

    "This is perfectly legitimate in the C language providing the pointer addresses
    an array element; but some coding standards recommend that if a parameter refers to an
    array, it should be declared as an array." */
/* PRQA S 0492 EOF*/

/*  DEVIATION:     0781
    DESCRIPTION:   '...' is being used as a structure/union member as well as being a label, tag or ordinary identifier.
    JUSTIFICATION: This file contains well used legacy code. */
/* PRQA S 0781 EOF*/

/**
 * @file
 * @brief general details of file
 */

/*************************************************************
*      FILE DEFINITIONS                                     *
*************************************************************/


/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/

#include <mathf.h>
#include <string.h>

#include "../FOUNDATION/FOUNDATION.h"

#include "../UTIL/ModTypes.h"
#include "../UTIL/ModCAA.h"
#include "../UTIL/matrix_manipulation.h"

/*************************************************************/
/*      LOCAL VARIABLES                                      */
/*************************************************************/

PRIVATE const F32 MINIMAL_DPSI_EGO = 0.001F;

/*************************************************************/
/*      LOCAL FUNCTION DECLARATIONS                          */
/*************************************************************/

/**
 * @details
 * @param[in, out] 'state_vector_vf32'
 * @param[in] 'input_vector_vf32'
 * @param[in] 'dt_f32'
 */
PRIVATE void set_CAA_predicted_state_vector(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32);

/**
 * @details
 * @param[out] 'A_matrix_vf32'
 * @param[in] 'state_vector_vf32'
 * @param[in] 'input_vector_vf32'
 * @param[in] 'dt_f32'
 */
PRIVATE void get_CAA_A_matrix(F32* const A_matrix_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32);

/**
 * @details
 * @param[out] 'Q_matrix_vf32'
 * @param[in] 'state_vector_vf32'
 * @param[in] 'p_input_state_s'
 * @param[in] 'process_variance_vf32'
 * @param[in] 'stabilizing_noise_covariance_vf32'
 * @param[in] 'dt_f32'
 */
PRIVATE void get_CAA_process_noise_matrix( F32* const Q_matrix_vf32,
                                   const F32* const state_vector_vf32,
                                   const INPUT_STRUCT* const p_input_state_s,
                                   const F32* const process_variance_vf32,
                                   const F32* const stabilizing_noise_covariance_vf32,
                                   const F32 dt_f32 );

/**
 * @details
 * @param[out] 'C_row_vf32'
 * @param[in] 'state_vector_vf32'
 * @param[in] 'input_vector_vf32'
 * @param[in] 'meas_type_e'
 */
PRIVATE void get_CAA_C_row(F32* const C_row_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e);

/**
 * @details
 * @param[out] 'p_pred_feature_f32'
 * @param[in] 'state_vector_vf32'
 * @param[in] 'input_vector_vf32'
 * @param[in] 'meas_type_e'
 */
PRIVATE void get_CAA_predicted_feature(F32* const p_pred_feature_f32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e);

/**
 * @details
 * @param[out] 'p_pred_feature_input_noise_f32'
 * @param[in] 'state_vector_vf32'
 * @param[in] 'p_input_state_s'
 * @param[in] 'meas_type_e'
 */
PRIVATE void get_CAA_predicted_feature_input_noise(F32* const p_pred_feature_input_noise_f32, const F32* const state_vector_vf32, const INPUT_STRUCT* const p_input_state_s, const MEASUREMENT_TYPES meas_type_e);

/**
 * @details This function translates the state vector in given coordinate origin
 */
PRIVATE void get_CAA_translated_state_vector(F32* const translated_state_vector_vf32, const F32* const state_vector_vf32, const F32 measurement_origin_x_relative_f32, const F32 measurement_origin_y_relative_f32);

/**
 * @details
 * @param[out] 'state_vector_vf32'
 * @param[in] 'input_vector_vf32'
 * @param[in] 'meas_feature_vf32'
 */
PRIVATE void init_CAA_state_vector(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32* const meas_feature_vf32);

/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

PRIVATE void set_CAA_predicted_state_vector(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32)
{
    /* local variables*/
    F32                v_ego_f32                    = input_vector_vf32[INDEX_INPUT_SPEED];
    F32                dpsi_ego_f32                 = input_vector_vf32[INDEX_INPUT_YAWRATE];
    F32                x_f32                        = state_vector_vf32[INDEX_X_REL];
    F32                y_f32                        = state_vector_vf32[INDEX_Y_REL];
    F32                psi_f32                      = state_vector_vf32[INDEX_PSI_REL];
    F32                v_f32                        = state_vector_vf32[INDEX_V_ABS];
    F32                a_f32                        = state_vector_vf32[INDEX_A_ABS];
    F32                cosf_dpsi_ego_dt_f32         = cosf(dpsi_ego_f32 * dt_f32);
    F32                sinf_dpsi_ego_dt_f32         = sinf(dpsi_ego_f32 * dt_f32);
    F32                cosf_dpsi_ego_dt__psi_f32    = cosf((dpsi_ego_f32 * dt_f32) - psi_f32);
    F32                sinf_dpsi_ego_dt__psi_f32    = sinf((dpsi_ego_f32 * dt_f32) - psi_f32);
    F32                cosf_psi_f32                 = cosf(psi_f32);
    F32                sinf_psi_f32                 = sinf(psi_f32);

    if (fabsf(dpsi_ego_f32) > MINIMAL_DPSI_EGO)
    {
        /* vector field, exact with one expansion in dpsi_ego_f32*/
        state_vector_vf32[INDEX_X_REL]   =                              (cosf_dpsi_ego_dt_f32*x_f32)
                                                                        + (sinf_dpsi_ego_dt_f32*y_f32)
                                                                        + (-v_ego_f32*dt_f32)
                                                                        + (cosf_dpsi_ego_dt__psi_f32 * dt_f32 * v_f32)
                                                                        + (cosf_dpsi_ego_dt__psi_f32 * a_f32 * dt_f32 * dt_f32 * 0.5F);
        state_vector_vf32[INDEX_Y_REL]   =                              (-sinf_dpsi_ego_dt_f32 * x_f32)
                                                                        + (cosf_dpsi_ego_dt_f32 * y_f32)
                                                                        + (v_ego_f32 * 0.5F * dpsi_ego_f32 * dt_f32 * dt_f32)
                                                                        + (-sinf_dpsi_ego_dt__psi_f32 * dt_f32 * v_f32)
                                                                        + (-sinf_dpsi_ego_dt__psi_f32 * a_f32 * dt_f32 * dt_f32 * 0.5F);
        state_vector_vf32[INDEX_PSI_REL] = (-dpsi_ego_f32 * dt_f32) + psi_f32;
        state_vector_vf32[INDEX_V_ABS]   = (a_f32 * dt_f32) + v_f32;
        state_vector_vf32[INDEX_A_ABS]   = a_f32;
    }
    else
    {
        /* vector field*/
        state_vector_vf32[INDEX_X_REL]   = x_f32 + (a_f32 * cosf_psi_f32 * dt_f32 * dt_f32 * 0.5F) + (v_f32 * cosf_psi_f32 * dt_f32) + (-v_ego_f32 * dt_f32);
        state_vector_vf32[INDEX_Y_REL]   = y_f32 + (a_f32 * sinf_psi_f32 * dt_f32 * dt_f32 * 0.5F) + (v_f32 * sinf_psi_f32 * dt_f32);
        state_vector_vf32[INDEX_PSI_REL] = psi_f32;
        state_vector_vf32[INDEX_V_ABS]   = (a_f32 * dt_f32) + v_f32;
        state_vector_vf32[INDEX_A_ABS]   = a_f32;
    }
}


PRIVATE void get_CAA_A_matrix(F32* const A_matrix_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32)
{
    /* local variables*/
    F32                dpsi_ego_f32                 = input_vector_vf32[INDEX_INPUT_YAWRATE];
    F32                psi_f32                      = state_vector_vf32[INDEX_PSI_REL];
    F32                v_f32                        = state_vector_vf32[INDEX_V_ABS];
    F32                a_f32                        = state_vector_vf32[INDEX_A_ABS];
    F32                cosf_dpsi_ego_dt_f32         = cosf(dpsi_ego_f32 * dt_f32);
    F32                sinf_dpsi_ego_dt_f32         = sinf(dpsi_ego_f32 * dt_f32);
    F32                cosf_dpsi_ego_dt__psi_f32    = cosf((dpsi_ego_f32 * dt_f32) - psi_f32);
    F32                sinf_dpsi_ego_dt__psi_f32    = sinf((dpsi_ego_f32 * dt_f32) - psi_f32);
    F32                cosf_psi_f32                 = cosf(psi_f32);
    F32                sinf_psi_f32                 = sinf(psi_f32);

    /* set to identity matrix*/
    SetSquareIdentityMatrix( A_matrix_vf32, STATE_DIMENSION_CAA );

    if (fabsf(dpsi_ego_f32) > MINIMAL_DPSI_EGO)
    {
        /* A matrix*/
        A_matrix_vf32[GEN_IDX(0, 0, STATE_DIMENSION_CAA)] = cosf_dpsi_ego_dt_f32;
        A_matrix_vf32[GEN_IDX(0, 1, STATE_DIMENSION_CAA)] = sinf_dpsi_ego_dt_f32;
        A_matrix_vf32[GEN_IDX(0, 2, STATE_DIMENSION_CAA)] = sinf_dpsi_ego_dt__psi_f32 * dt_f32 * ((2.0F * v_f32) + (a_f32 * dt_f32)) * 0.5F;
        A_matrix_vf32[GEN_IDX(0, 3, STATE_DIMENSION_CAA)] = cosf_dpsi_ego_dt__psi_f32 * dt_f32;
        A_matrix_vf32[GEN_IDX(0, 4, STATE_DIMENSION_CAA)] = cosf_dpsi_ego_dt__psi_f32 * dt_f32 * dt_f32 * 0.5F;
        A_matrix_vf32[GEN_IDX(1, 0, STATE_DIMENSION_CAA)] = -sinf_dpsi_ego_dt_f32;
        A_matrix_vf32[GEN_IDX(1, 1, STATE_DIMENSION_CAA)] = cosf_dpsi_ego_dt_f32;
        A_matrix_vf32[GEN_IDX(1, 2, STATE_DIMENSION_CAA)] = cosf_dpsi_ego_dt__psi_f32 * dt_f32 * ((2.0F *v_f32) + (a_f32 * dt_f32)) * 0.5F;
        A_matrix_vf32[GEN_IDX(1, 3, STATE_DIMENSION_CAA)] = -sinf_dpsi_ego_dt__psi_f32*dt_f32;
        A_matrix_vf32[GEN_IDX(1, 4, STATE_DIMENSION_CAA)] = -sinf_dpsi_ego_dt__psi_f32*dt_f32*dt_f32*0.5F;
        A_matrix_vf32[GEN_IDX(3, 4, STATE_DIMENSION_CAA)] = dt_f32;
    }
    else
    {
        /* A matrix*/
        A_matrix_vf32[GEN_IDX(0, 2, STATE_DIMENSION_CAA)] = -sinf_psi_f32 * dt_f32 * ((a_f32 * dt_f32) + (2.0F * v_f32)) * 0.5F;
        A_matrix_vf32[GEN_IDX(0, 3, STATE_DIMENSION_CAA)] = cosf_psi_f32*dt_f32;
        A_matrix_vf32[GEN_IDX(0, 4, STATE_DIMENSION_CAA)] = cosf_psi_f32*dt_f32*dt_f32*0.5F;
        A_matrix_vf32[GEN_IDX(1, 2, STATE_DIMENSION_CAA)] = cosf_psi_f32 * dt_f32 * ((a_f32 * dt_f32) + (2.0F * v_f32)) * 0.5F;
        A_matrix_vf32[GEN_IDX(1, 3, STATE_DIMENSION_CAA)] = sinf_psi_f32*dt_f32;
        A_matrix_vf32[GEN_IDX(1, 4, STATE_DIMENSION_CAA)] = sinf_psi_f32*dt_f32*dt_f32*0.5F;
        A_matrix_vf32[GEN_IDX(3, 4, STATE_DIMENSION_CAA)] = dt_f32;
    }
}

/*  DEVIATION:     4700
    DESCRIPTION:   Metric value out of threshold range: get_CAA_process_noise_matrix() : STPAR = 6
    JUSTIFICATION: UTIL function needs multiple inputs. Splitting this into subsequent function calls would make it more complex.
                   Code is still easy to read.
*/
/* PRQA S 4700 48 */
PRIVATE void get_CAA_process_noise_matrix( F32* const Q_matrix_vf32,
                                   const F32* const state_vector_vf32,
                                   const INPUT_STRUCT* const p_input_state_s,
                                   const F32* const process_variance_vf32,
                                   const F32* const stabilizing_noise_covariance_vf32,
                                   const F32 dt_f32 )
{
    /* local variables*/
    F32                dpsi_ego_f32             = p_input_state_s->mean_vf32[INDEX_INPUT_YAWRATE];
    F32                x_f32                    = state_vector_vf32[INDEX_X_REL];
    F32                y_f32                    = state_vector_vf32[INDEX_Y_REL];
    F32                v_ego_var_f32            = p_input_state_s->covariance_vf32[SYM_IDX(INDEX_INPUT_SPEED, INDEX_INPUT_SPEED, INPUT_DIMENSION)];
    F32                dpsi_ego_var_f32         = p_input_state_s->covariance_vf32[SYM_IDX(INDEX_INPUT_YAWRATE, INDEX_INPUT_YAWRATE, INPUT_DIMENSION)];
    F32                V_target_yaw_f32         = process_variance_vf32[INDEX_CAA_PROCESS_NOISE_PSI_REL];
    F32                V_target_a_f32           = process_variance_vf32[INDEX_CAA_PROCESS_NOISE_A_ABS];
    F32                cosf_psi_f32             = cosf(state_vector_vf32[INDEX_PSI_REL]);
    F32                sinf_psi_f32             = sinf(state_vector_vf32[INDEX_PSI_REL]);
    F32                v_f32                    = state_vector_vf32[INDEX_V_ABS];

    /* set all elements to zero*/
    (void)memset(Q_matrix_vf32, 0, NR_ELEMENTS_SYM(STATE_DIMENSION_CAA) * sizeof(F32));

    if (fabsf(dpsi_ego_f32) > MINIMAL_DPSI_EGO)
    {
        /* Q matrix (Q_TAYLOR_APPROX == 3)*/
        Q_matrix_vf32[SYM_IDX(0, 0, STATE_DIMENSION_CAA)] = ((y_f32 * y_f32 * dpsi_ego_var_f32) + v_ego_var_f32) * dt_f32 * dt_f32;
        Q_matrix_vf32[SYM_IDX(0, 1, STATE_DIMENSION_CAA)] = -y_f32 * dpsi_ego_var_f32 * x_f32 * dt_f32 * dt_f32;
        Q_matrix_vf32[SYM_IDX(0, 2, STATE_DIMENSION_CAA)] = -y_f32 * dpsi_ego_var_f32 * dt_f32 * dt_f32;
        Q_matrix_vf32[SYM_IDX(1, 1, STATE_DIMENSION_CAA)] = x_f32 * x_f32 * dpsi_ego_var_f32 * dt_f32 * dt_f32;
        Q_matrix_vf32[SYM_IDX(1, 2, STATE_DIMENSION_CAA)] = dt_f32 * dt_f32 * x_f32 * dpsi_ego_var_f32;
        Q_matrix_vf32[SYM_IDX(2, 2, STATE_DIMENSION_CAA)] = (V_target_yaw_f32 + dpsi_ego_var_f32) * dt_f32 * dt_f32;
        Q_matrix_vf32[SYM_IDX(4, 4, STATE_DIMENSION_CAA)] = dt_f32 * dt_f32 * V_target_a_f32;
    }
    else
    {
        /* Q matrix*/
        Q_matrix_vf32[SYM_IDX(0, 0, STATE_DIMENSION_CAA)] = v_ego_var_f32*dt_f32*dt_f32;
        Q_matrix_vf32[SYM_IDX(0, 2, STATE_DIMENSION_CAA)] = -sinf_psi_f32*v_f32*V_target_yaw_f32*dt_f32*dt_f32*dt_f32*0.5F;
        Q_matrix_vf32[SYM_IDX(1, 2, STATE_DIMENSION_CAA)] = cosf_psi_f32*v_f32*V_target_yaw_f32*dt_f32*dt_f32*dt_f32*0.5F;
        Q_matrix_vf32[SYM_IDX(2, 2, STATE_DIMENSION_CAA)] = V_target_yaw_f32*dt_f32*dt_f32;
        Q_matrix_vf32[SYM_IDX(3, 4, STATE_DIMENSION_CAA)] = dt_f32*dt_f32*dt_f32*V_target_a_f32*0.5F;
        Q_matrix_vf32[SYM_IDX(4, 4, STATE_DIMENSION_CAA)] = dt_f32*dt_f32*V_target_a_f32;
    }

    /* add stabilizing noise to system */
    SymMatrixAddition( Q_matrix_vf32, 1.0F, Q_matrix_vf32, 1.0F, stabilizing_noise_covariance_vf32, STATE_DIMENSION_CAA );

}

/*  DEVIATION:     4700
    DESCRIPTION:   Metric value out of threshold range: get_CAA_C_row() : STCYC = 12
    JUSTIFICATION: UTIL function, code is still easily readable. Splitting this into subsequent function calls would make it more complex.
*/
/* PRQA S 4700 134 */
PRIVATE void get_CAA_C_row(F32* const C_row_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e)
{
    /* local variables*/
    F32 cos_psi_rel_f32     = cosf(state_vector_vf32[INDEX_PSI_REL]);
    F32 sin_psi_rel_f32     = sinf(state_vector_vf32[INDEX_PSI_REL]);
    F32 v_ego_f32           = input_vector_vf32[INDEX_INPUT_SPEED];
    F32 x_f32               = state_vector_vf32[INDEX_X_REL];
    F32 y_f32               = state_vector_vf32[INDEX_Y_REL];
    F32 v_abs_f32           = state_vector_vf32[INDEX_V_ABS];
    F32 r_rel_f32;
    F32 r_rel_squared_f32;
    F32 r_rel_pow3_f32;

    /* assign C matrix*/
    /* first set everything to zero*/
    (void)memset( C_row_vf32, 0, STATE_DIMENSION_CAA * sizeof(F32) );

    switch (meas_type_e)
    {
        case MEAS_X_REL:
        {
            C_row_vf32[INDEX_X_REL] = 1.0F;
            break;
        }
        case MEAS_TAN_PHI_REL:
        {
            if ( fabsf(state_vector_vf32[INDEX_X_REL]) > MINIMAL_DISTANCE_MEAS_MODEL )
            {
                C_row_vf32[INDEX_X_REL] = -state_vector_vf32[INDEX_Y_REL] / squaref( state_vector_vf32[INDEX_X_REL] );
                C_row_vf32[INDEX_Y_REL] = 1.0F / state_vector_vf32[INDEX_X_REL];
            }
            else
            {
                /* leave as zero*/
            }
            break;
        }
        case MEAS_VX_REL:
        {
            C_row_vf32[INDEX_Y_REL]     = input_vector_vf32[INDEX_INPUT_YAWRATE];
            C_row_vf32[INDEX_PSI_REL]   = -state_vector_vf32[INDEX_V_ABS] * sin_psi_rel_f32;
            C_row_vf32[INDEX_V_ABS]     = cos_psi_rel_f32;
            break;
        }
        case MEAS_Y_REL:
        {
            C_row_vf32[INDEX_Y_REL]     = 1.0F;
            break;
        }
        case MEAS_R_REL:
        {
            r_rel_f32                       = sqrtf( squaref( state_vector_vf32[INDEX_X_REL] ) + squaref( state_vector_vf32[INDEX_Y_REL] ) );
            if ( r_rel_f32 > MINIMAL_DISTANCE_MEAS_MODEL )
            {
                C_row_vf32[INDEX_X_REL]     = state_vector_vf32[INDEX_X_REL] / r_rel_f32;
                C_row_vf32[INDEX_Y_REL]     = state_vector_vf32[INDEX_Y_REL] / r_rel_f32;
            }
            else
            {
                /* leave as zero*/
            }
            break;
        }
        case MEAS_VR_REL:
        {
            /* from ...\Simulator\matlab\scripts\Radial_Velocity_Sensor_Model.m */
            r_rel_f32                       = sqrtf( squaref( x_f32 ) + squaref( y_f32 ) );
            if ( r_rel_f32 > MINIMAL_DISTANCE_MEAS_MODEL )
            {
                /* raise to the third power for denominator*/
                r_rel_pow3_f32               = r_rel_f32 * r_rel_f32 * r_rel_f32;

                /* DEVIATION:     3395
                   DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. A * or / operation is the operand of a + or - operator.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* DEVIATION:     3393
                   DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. An arithmetic operation (* / + -) is the operand of a different operator with the same precedence.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* PRQA S 3395 1*/ /* PRQA S 3393 1*/
                C_row_vf32[INDEX_X_REL]      = -y_f32*(v_ego_f32*y_f32-v_abs_f32*y_f32*cos_psi_rel_f32+v_abs_f32*x_f32*sin_psi_rel_f32) / r_rel_pow3_f32;

                /* DEVIATION:     3395
                   DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. A * or / operation is the operand of a + or - operator.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* DEVIATION:     3393
                   DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. An arithmetic operation (* / + -) is the operand of a different operator with the same precedence.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* PRQA S 3395 1*/ /* PRQA S 3393 1*/
                C_row_vf32[INDEX_Y_REL]      =  x_f32*(v_ego_f32*y_f32-v_abs_f32*y_f32*cos_psi_rel_f32+v_abs_f32*x_f32*sin_psi_rel_f32) / r_rel_pow3_f32;

                /* DEVIATION:     3395
                   DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. A * or / operation is the operand of a + or - operator.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* DEVIATION:     3393
                   DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. An arithmetic operation (* / + -) is the operand of a different operator with the same precedence.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* PRQA S 3395 1*/ /* PRQA S 3393 1*/
                C_row_vf32[INDEX_PSI_REL]    = v_abs_f32*(y_f32*cos_psi_rel_f32-x_f32*sin_psi_rel_f32) / r_rel_f32;

                /* DEVIATION:     3395
                   DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. A * or / operation is the operand of a + or - operator.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* PRQA S 3395 1*/
                C_row_vf32[INDEX_V_ABS]      = (x_f32*cos_psi_rel_f32+y_f32*sin_psi_rel_f32) / r_rel_f32;
            }
            else
            {
                /* leave as zero*/
            }
            break;
        }
        case MEAS_DPHI_REL:
        {
            r_rel_squared_f32       = squaref( state_vector_vf32[INDEX_X_REL] ) + squaref( state_vector_vf32[INDEX_Y_REL] );
            if ( r_rel_squared_f32 > (MINIMAL_DISTANCE_MEAS_MODEL * MINIMAL_DISTANCE_MEAS_MODEL) )
            {
                C_row_vf32[INDEX_X_REL]     =  -((v_abs_f32 * sin_psi_rel_f32 * x_f32 * x_f32) + (-v_abs_f32 * sin_psi_rel_f32 * y_f32*y_f32) + (-2.0F * x_f32 * y_f32 * v_abs_f32 * cos_psi_rel_f32) + (2.0F * x_f32 * y_f32 * v_ego_f32)) / squaref(r_rel_squared_f32);
                C_row_vf32[INDEX_Y_REL]     =  -((v_abs_f32 * cos_psi_rel_f32 * x_f32 * x_f32) + (-v_abs_f32 * cos_psi_rel_f32 * y_f32 * y_f32)+ (-v_ego_f32 * x_f32 * x_f32) + (v_ego_f32 * y_f32 * y_f32) + (2.0F * y_f32 * v_abs_f32 * sin_psi_rel_f32 * x_f32)) / squaref(r_rel_squared_f32);
                C_row_vf32[INDEX_PSI_REL]   =  (v_abs_f32 * ((cos_psi_rel_f32 * x_f32) + (y_f32 * sin_psi_rel_f32))) / r_rel_squared_f32;
                C_row_vf32[INDEX_V_ABS]     =  ((sin_psi_rel_f32 * x_f32) - (y_f32 * cos_psi_rel_f32)) / r_rel_squared_f32;
            }
            else
            {
                /* leave as zero*/
            }
            break;
        }
        default:
        {
            ASSERT(FALSE)
            break;
        }
    }
}

/*  DEVIATION:     4700
    DESCRIPTION:   Metric value out of threshold range: get_CAA_predicted_feature() : STCYC = 11
    JUSTIFICATION: UTIL function, code is still easily readable. Splitting this into subsequent function calls would make it more complex.
*/
/* PRQA S 4700 90 */
PRIVATE void get_CAA_predicted_feature(F32* const p_pred_feature_f32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e)
{
    /* local variables*/
    F32 cos_psi_rel_f32     = cosf(state_vector_vf32[INDEX_PSI_REL]);
    F32 v_ego_f32           = input_vector_vf32[INDEX_INPUT_SPEED];
    F32 dpsi_ego_f32        = input_vector_vf32[INDEX_INPUT_YAWRATE];
    F32 x_f32               = state_vector_vf32[INDEX_X_REL];
    F32 y_f32               = state_vector_vf32[INDEX_Y_REL];
    F32 psi_rel_f32         = state_vector_vf32[INDEX_PSI_REL];
    F32 v_abs_f32           = state_vector_vf32[INDEX_V_ABS];
    F32 r_rel_squared_f32;
    F32 r_rel_f32;


    switch (meas_type_e)
    {
        case MEAS_X_REL:
        {
            *p_pred_feature_f32    = state_vector_vf32[INDEX_X_REL];
            break;
        }
        case MEAS_TAN_PHI_REL:
        {
            if ( fabsf(state_vector_vf32[INDEX_X_REL]) > MINIMAL_DISTANCE_MEAS_MODEL )
            {
                *p_pred_feature_f32    = state_vector_vf32[INDEX_Y_REL] / state_vector_vf32[INDEX_X_REL];
            }
            else
            {
                *p_pred_feature_f32    = 0.0F;
            }
            break;
        }
        case MEAS_VX_REL:
        {
            *p_pred_feature_f32    = ( (state_vector_vf32[INDEX_V_ABS] * cos_psi_rel_f32)
                                        - input_vector_vf32[INDEX_INPUT_SPEED] )
                                        + (state_vector_vf32[INDEX_Y_REL] * input_vector_vf32[INDEX_INPUT_YAWRATE]);
            break;
        }
        case MEAS_Y_REL:
        {
            *p_pred_feature_f32    = state_vector_vf32[INDEX_Y_REL];
            break;
        }
        case MEAS_R_REL:
        {
            *p_pred_feature_f32    = sqrtf( squaref( state_vector_vf32[INDEX_X_REL] ) + squaref( state_vector_vf32[INDEX_Y_REL] ) );
            break;
        }
        case MEAS_VR_REL:
        {
            /* from ...\Simulator\matlab\scripts\Radial_Velocity_Sensor_Model.m */
            r_rel_f32                  = sqrtf( squaref( x_f32 ) + squaref( y_f32 ) );
            if ( r_rel_f32 > MINIMAL_DISTANCE_MEAS_MODEL )
            {
                /* DEVIATION:     3395
                   DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. A * or / operation is the operand of a + or - operator.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* PRQA S 3395 1*/
                *p_pred_feature_f32    = (-v_ego_f32*x_f32+v_abs_f32*x_f32*cos_psi_rel_f32+v_abs_f32*y_f32*sinf(state_vector_vf32[INDEX_PSI_REL])) / r_rel_f32;
            }
            else
            {
                *p_pred_feature_f32    = 0.0F;
            }
            break;
        }
        case MEAS_DPHI_REL:
        {
            r_rel_squared_f32       = squaref( state_vector_vf32[INDEX_X_REL] ) + squaref( state_vector_vf32[INDEX_Y_REL] );
            if ( r_rel_squared_f32 > (MINIMAL_DISTANCE_MEAS_MODEL * MINIMAL_DISTANCE_MEAS_MODEL) )
            {
                *p_pred_feature_f32    = ( ( ( (v_abs_f32 * sinf(psi_rel_f32) * x_f32) - (x_f32 * x_f32 * dpsi_ego_f32) ) + (-y_f32 * v_abs_f32 * cos_psi_rel_f32) ) + ( (y_f32 * v_ego_f32) - (y_f32 * y_f32 * dpsi_ego_f32) ) )
                                           / r_rel_squared_f32;
            }
            else
            {
                *p_pred_feature_f32    = 0.0F;
            }
            break;
        }
        default:
        {
            *p_pred_feature_f32    = 0.0F;       /* what would be a good value for the default/error case?*/
            ASSERT(FALSE)
            break;
        }
    }
}


PRIVATE void get_CAA_predicted_feature_input_noise(F32* const p_pred_feature_input_noise_f32, const F32* const state_vector_vf32, const INPUT_STRUCT* const p_input_state_s, const MEASUREMENT_TYPES meas_type_e)
{
    /* local variables*/
    F32 r_rel_squared_f32;

    /* assign predicted feature input noise*/
    switch (meas_type_e)
    {
        case MEAS_X_REL:
        {
            *p_pred_feature_input_noise_f32    = 0.0F;
            break;
        }
        case MEAS_TAN_PHI_REL:
        {
            *p_pred_feature_input_noise_f32    = 0.0F;
            break;
        }
        case MEAS_VX_REL:
        {
            *p_pred_feature_input_noise_f32    = (squaref(state_vector_vf32[INDEX_Y_REL]) * p_input_state_s->covariance_vf32[SYM_IDX(INDEX_INPUT_YAWRATE, INDEX_INPUT_YAWRATE, INPUT_DIMENSION)])
                                                   + p_input_state_s->covariance_vf32[SYM_IDX(INDEX_INPUT_SPEED, INDEX_INPUT_SPEED, INPUT_DIMENSION)];
            break;
        }
        case MEAS_Y_REL:
        {
            *p_pred_feature_input_noise_f32    = 0.0F;
            break;
        }
        case MEAS_R_REL:
        {
            *p_pred_feature_input_noise_f32    = 0.0F;
            break;
        }
        case MEAS_VR_REL:
        {
            /* from ...\Simulator\matlab\scripts\Radial_Velocity_Sensor_Model.m */
            r_rel_squared_f32       = squaref( state_vector_vf32[INDEX_X_REL] ) + squaref( state_vector_vf32[INDEX_Y_REL] );
            if ( r_rel_squared_f32 > (MINIMAL_DISTANCE_MEAS_MODEL * MINIMAL_DISTANCE_MEAS_MODEL) )
            {
                /* DEVIATION:     3393
                   DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. An arithmetic operation (* / + -) is the operand of a different operator with the same precedence.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* PRQA S 3393 1*/
                *p_pred_feature_input_noise_f32    = p_input_state_s->covariance_vf32[SYM_IDX(INDEX_INPUT_SPEED, INDEX_INPUT_SPEED, INPUT_DIMENSION)] * squaref( state_vector_vf32[INDEX_X_REL] ) / r_rel_squared_f32;
            }
            else
            {
                *p_pred_feature_input_noise_f32    = 0.0F;
            }
            break;
        }
        case MEAS_DPHI_REL:
        {
            *p_pred_feature_input_noise_f32    = p_input_state_s->covariance_vf32[SYM_IDX(INDEX_INPUT_YAWRATE, INDEX_INPUT_YAWRATE, INPUT_DIMENSION)];
            /* to do: add term proportional to speed covariance */
            break;
        }
        default:
        {
            *p_pred_feature_input_noise_f32    = 0.0F;     /* error value?*/
            ASSERT(FALSE)
            break;
        }
    }
}

PRIVATE void get_CAA_translated_state_vector(F32* const translated_state_vector_vf32, const F32* const state_vector_vf32, const F32 measurement_origin_x_relative_f32, const F32 measurement_origin_y_relative_f32)
{
    /* translate x and y position, this is dependent upon the state vector and hence upon the model */
    translated_state_vector_vf32[INDEX_X_REL]   = state_vector_vf32[INDEX_X_REL] - measurement_origin_x_relative_f32;
    translated_state_vector_vf32[INDEX_Y_REL]   = state_vector_vf32[INDEX_Y_REL] - measurement_origin_y_relative_f32;
    translated_state_vector_vf32[INDEX_PSI_REL] = state_vector_vf32[INDEX_PSI_REL];
    translated_state_vector_vf32[INDEX_V_ABS]   = state_vector_vf32[INDEX_V_ABS];
    translated_state_vector_vf32[INDEX_A_ABS]   = state_vector_vf32[INDEX_A_ABS];
}

PRIVATE void init_CAA_state_vector(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32* const meas_feature_vf32)
{
    /* local variables*/
    F32 x_rel_f32       = meas_feature_vf32[MEAS_INIT_X_REL];
    F32 vx_rel_f32      = meas_feature_vf32[MEAS_INIT_VX_REL];
    F32 phi_rel_f32     = meas_feature_vf32[MEAS_INIT_PHI_REL];
    F32 cos_phi_rel_f32 = cosf(phi_rel_f32);
    F32 dphi_rel_f32    = meas_feature_vf32[MEAS_INIT_PHI_DOT_REL];
    F32 v_ego_f32       = input_vector_vf32[INDEX_INPUT_SPEED];
    F32 dpsi_ego_f32    = input_vector_vf32[INDEX_INPUT_YAWRATE];
    F32 vx_abs_f32;
    F32 vy_abs_times_cos_squared_f32;
    F32 vx_abs_times_cos_squared_f32;


    /* assign individual vector entries */
    /* first the easy ones */
    state_vector_vf32[INDEX_X_REL] = x_rel_f32;
    state_vector_vf32[INDEX_Y_REL] = x_rel_f32 * tanf( phi_rel_f32 );
    state_vector_vf32[INDEX_A_ABS] = 0.0F;

    /* velocity over ground */
    /* vx_abs_f32 and vy_abs_times_cos_squared_f32 are computed in \Simulator\matlab\scripts\Init_velocity_for_CAA_model.m */

    /* DEVIATION:     3395
       DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. A * or / operation is the operand of a + or - operator.
       JUSTIFICATION: Auto-Generated Code from Matlab. */
    /* DEVIATION:     3393
       DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. An arithmetic operation (* / + -) is the operand of a different operator with the same precedence.
       JUSTIFICATION: Auto-Generated Code from Matlab. */
    /* PRQA S 3395 1*/ /* PRQA S 3393 1*/
    vx_abs_f32                      = vx_rel_f32+v_ego_f32-x_rel_f32*tanf(phi_rel_f32)*dpsi_ego_f32;

    /* DEVIATION:     3395
       DESCRIPTION:   QAC Deviation Request: Extra parentheses recommended. A * or / operation is the operand of a + or - operator.
       JUSTIFICATION: Auto-Generated Code from Matlab. */
    /* PRQA S 3395 1*/
    vy_abs_times_cos_squared_f32    = (dphi_rel_f32+dpsi_ego_f32*squaref(cos_phi_rel_f32))*x_rel_f32+sinf(phi_rel_f32)*vx_rel_f32*cos_phi_rel_f32;
    vx_abs_times_cos_squared_f32    = squaref(cos_phi_rel_f32) * vx_abs_f32;

    /* angle of speed vector */
    if (   (fabsf(vy_abs_times_cos_squared_f32) > SMALL_NUMBER)
        || (fabsf(vx_abs_times_cos_squared_f32) > SMALL_NUMBER)
        )
    {
        state_vector_vf32[INDEX_PSI_REL] = atan2f( vy_abs_times_cos_squared_f32,
                                                   vx_abs_times_cos_squared_f32 );
    }
    else
    {
        state_vector_vf32[INDEX_PSI_REL] = 0.0F;
    }

    /* magnitude of speed vector */
    if ( fabsf(cos_phi_rel_f32) > SMALL_NUMBER )
    {
        state_vector_vf32[INDEX_V_ABS] = sqrtf( squaref( vx_abs_f32 ) + squaref( vy_abs_times_cos_squared_f32 / squaref( cos_phi_rel_f32 ) ) );
    }
    else
    {
        state_vector_vf32[INDEX_V_ABS] = 0.0F;
    }
}

void create_CAA_Kalman_model(KALMAN_MODEL_STRUCT* const p_CAA_Kalman_model_s)
{
    if(NULL != p_CAA_Kalman_model_s)
    {
        /* reset the whole kalman model struct */
        (void)memset(p_CAA_Kalman_model_s, 0, sizeof(KALMAN_MODEL_STRUCT));

        /* assign existing functions     */
        p_CAA_Kalman_model_s->set_predicted_state_vector            = &set_CAA_predicted_state_vector;
        p_CAA_Kalman_model_s->get_A_matrix                          = &get_CAA_A_matrix;
        p_CAA_Kalman_model_s->get_process_noise_matrix              = &get_CAA_process_noise_matrix;
        p_CAA_Kalman_model_s->get_C_row                             = &get_CAA_C_row;
        p_CAA_Kalman_model_s->get_predicted_feature                 = &get_CAA_predicted_feature;
        p_CAA_Kalman_model_s->get_predicted_feature_input_noise     = &get_CAA_predicted_feature_input_noise;
        p_CAA_Kalman_model_s->get_translated_state_vector           = &get_CAA_translated_state_vector;
        p_CAA_Kalman_model_s->init_state_vector                     = &init_CAA_state_vector;
        p_CAA_Kalman_model_s->model_type_u8                         = MODEL_CAA;
        p_CAA_Kalman_model_s->model_state_dim_u8                    = STATE_DIMENSION_CAA;
        p_CAA_Kalman_model_s->model_input_dim_u8                    = INPUT_DIMENSION;
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != p_CAA_Kalman_model_s)
    }
}
