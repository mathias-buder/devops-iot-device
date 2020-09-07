
/* PRQA S 0292 17 */
/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: ModCACV.c $
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
    array, it should be declared as an array."  */
    /* PRQA S 0492 EOF*/

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
#include "../UTIL/ModCACV.h"
#include "../UTIL/matrix_manipulation.h"

/*************************************************************/
/*      LOCAL FUNCTION DECLARATIONS                          */
/*************************************************************/

/**
 * @details
 * @param[out] 'state_vector_vf32'
 * @param[in] 'input_vector_vf32' is not used.
 * @param[in] 'dt_f32'
 */
PRIVATE void set_CACV_predicted_state_vector(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32);

/**
 * @details
 * @param[out] 'A_matrix_vf32'
 * @param[in] 'state_vector_vf32' is not used.
 * @param[in] 'input_vector_vf32' is not used.
 * @param[in] 'dt_f32'
 */
PRIVATE void get_CACV_A_matrix(F32* const A_matrix_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32);

/**
 * @details
 * @param[out] 'Q_matrix_vf32'
 * @param[in] 'state_vector_vf32' is not used.
 * @param[in] 'p_input_state_s' is not used.
 * @param[in] 'process_variance_vf32'
 * @param[in] 'stabilizing_noise_covariance_vf32'
 * @param[in] 'dt_f32'
 */
PRIVATE void get_CACV_process_noise_matrix( F32* const Q_matrix_vf32,
                                   const F32* const state_vector_vf32,
                                   const INPUT_STRUCT* const p_input_state_s,
                                   const F32* const process_variance_vf32,
                                   const F32* const stabilizing_noise_covariance_vf32,
                                   const F32 dt_f32 );

/**
 * @details
 * @param[out] 'C_row_vf32'
 * @param[in] 'state_vector_vf32'
 * @param[in] 'input_vector_vf32' is not used.
 * @param[in] 'meas_type_e'
 */
PRIVATE void get_CACV_C_row(F32* const C_row_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e);

/**
 * @details
 * @param[out] 'p_pred_feature_f32'
 * @param[in] 'state_vector_vf32'
 * @param[in] 'input_vector_vf32' is not used.
 * @param[in] 'meas_type_e'
 */
PRIVATE void get_CACV_predicted_feature(F32* const p_pred_feature_f32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e);

/**
 * @details
 * @param[out] 'p_pred_feature_input_noise_f32'
 * @param[in] 'state_vector_vf32' is un-used.
 * @param[in] 'p_input_state_s' is un-used.
 * @param[in] 'meas_type_e' is un-used.
 */
PRIVATE void get_CACV_predicted_feature_input_noise(F32* const p_pred_feature_input_noise_f32, const F32* const state_vector_vf32, const INPUT_STRUCT* const p_input_state_s, const MEASUREMENT_TYPES meas_type_e);

/**
 * @details This function translates the state vector in given coordinate origin
 */
PRIVATE void get_CACV_translated_state_vector(F32* const translated_state_vector_vf32, const F32* const state_vector_vf32, const F32 measurement_origin_x_relative_f32, const F32 measurement_origin_y_relative_f32);

/**
 * @details
 * @param[out] 'state_vector_vf32'
 * @param[in] 'input_vector_vf32' is not used.
 * @param[in] 'meas_feature_vf32'
 */
PRIVATE void init_CACV_state_vector(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32* const meas_feature_vf32);


/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

PRIVATE void set_CACV_predicted_state_vector(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32)
{
    /* local variables*/
    F32                x_f32                        = state_vector_vf32[INDEX_X_REL];
    F32                y_f32                        = state_vector_vf32[INDEX_Y_REL];

    /* vector field*/
    state_vector_vf32[INDEX_X_REL]   = x_f32 + (state_vector_vf32[INDEX_VX_REL] * dt_f32) + (state_vector_vf32[INDEX_AX_REL] * dt_f32 * dt_f32 * 0.5F);
    state_vector_vf32[INDEX_Y_REL]   = y_f32 + (state_vector_vf32[INDEX_VY_REL] * dt_f32);
    state_vector_vf32[INDEX_VX_REL]  = (state_vector_vf32[INDEX_AX_REL] * dt_f32) + state_vector_vf32[INDEX_VX_REL];
    /* state_vector_vf32[INDEX_VY_REL]  = state_vector_vf32[INDEX_VY_REL];   stays the same */
    /* state_vector_vf32[INDEX_AX_REL]  = state_vector_vf32[INDEX_AX_REL];   stays the same */
}

PRIVATE void get_CACV_A_matrix(F32* const A_matrix_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32 dt_f32)
{
    /* set to identity matrix*/
    SetSquareIdentityMatrix( A_matrix_vf32, STATE_DIMENSION_CACV );

    /* A matrix*/
    A_matrix_vf32[GEN_IDX(0, 2, STATE_DIMENSION_CACV)] = dt_f32;
    A_matrix_vf32[GEN_IDX(0, 4, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*0.5F;
    A_matrix_vf32[GEN_IDX(1, 3, STATE_DIMENSION_CACV)] = dt_f32;
    A_matrix_vf32[GEN_IDX(2, 4, STATE_DIMENSION_CACV)] = dt_f32;
}

PRIVATE void get_CACV_process_noise_matrix( F32* const Q_matrix_vf32,
                                    const F32* const state_vector_vf32,
                                    const INPUT_STRUCT* const p_input_state_s,
                                    const F32* const process_variance_vf32,
                                    const F32* const stabilizing_noise_covariance_vf32,
                                    const F32 dt_f32 )
{
    /* local variables*/
    F32                V_ax_f32         = process_variance_vf32[INDEX_CACV_PROCESS_NOISE_AX];
    F32                V_vy_f32         = process_variance_vf32[INDEX_CACV_PROCESS_NOISE_VY];

    /* set all elements to zero*/
    (void)memset(Q_matrix_vf32, 0, NR_ELEMENTS_SYM(STATE_DIMENSION_CACV) * sizeof(F32));

    /* Q matrix*/
    Q_matrix_vf32[SYM_IDX(0, 0, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*dt_f32*dt_f32*dt_f32*dt_f32*V_ax_f32*0.027777777778F;
    Q_matrix_vf32[SYM_IDX(0, 2, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*dt_f32*dt_f32*dt_f32*V_ax_f32*0.083333333333F;
    Q_matrix_vf32[SYM_IDX(0, 4, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*dt_f32*dt_f32*V_ax_f32*0.166666666667F;
    Q_matrix_vf32[SYM_IDX(1, 1, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*dt_f32*dt_f32*V_vy_f32*0.25F;
    Q_matrix_vf32[SYM_IDX(1, 3, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*dt_f32*V_vy_f32*0.5F;
    Q_matrix_vf32[SYM_IDX(2, 2, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*dt_f32*dt_f32*V_ax_f32*0.25F;
    Q_matrix_vf32[SYM_IDX(2, 4, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*dt_f32*V_ax_f32*0.5F;
    Q_matrix_vf32[SYM_IDX(3, 3, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*V_vy_f32;
    Q_matrix_vf32[SYM_IDX(4, 4, STATE_DIMENSION_CACV)] = dt_f32*dt_f32*V_ax_f32;

    /* add stabilizing noise to system */
    SymMatrixAddition( Q_matrix_vf32, 1.0F, Q_matrix_vf32, 1.0F, stabilizing_noise_covariance_vf32, STATE_DIMENSION_CACV );

}


PRIVATE void get_CACV_C_row(F32* const C_row_vf32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e)
{
    /* local variables*/
    F32 r_rel_f32;
    F32 r_rel_pow3_f32;

    /* assign C matrix*/
    /* first set everything to zero*/
    (void)memset( C_row_vf32, 0, STATE_DIMENSION_CACV * sizeof(F32) );

    switch (meas_type_e)
    {
        case MEAS_X_REL:
        {
            C_row_vf32[INDEX_X_REL]     = 1.0F;
            break;
        }
        case MEAS_TAN_PHI_REL:
        {
            if ( fabsf(state_vector_vf32[INDEX_X_REL]) > MINIMAL_DISTANCE_MEAS_MODEL )
            {
                C_row_vf32[INDEX_X_REL]     = -state_vector_vf32[INDEX_Y_REL] / squaref( state_vector_vf32[INDEX_X_REL] );
                C_row_vf32[INDEX_Y_REL]     = 1.0F / state_vector_vf32[INDEX_X_REL];
            }
            else
            {
                /* leave as zero*/
            }
            break;
        }
        case MEAS_VX_REL:
        {
            C_row_vf32[INDEX_VX_REL]     = 1.0F;
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
            r_rel_f32                       = sqrtf( squaref( state_vector_vf32[INDEX_X_REL] ) + squaref( state_vector_vf32[INDEX_Y_REL] ) );
            if ( r_rel_f32 > MINIMAL_DISTANCE_MEAS_MODEL )
            {
                /* raise to the third power for denominator*/
                r_rel_pow3_f32               = r_rel_f32 * r_rel_f32 * r_rel_f32;

                C_row_vf32[INDEX_X_REL]      =  ( state_vector_vf32[INDEX_Y_REL] * ( (state_vector_vf32[INDEX_VX_REL] * state_vector_vf32[INDEX_Y_REL])
                                                                                   - (state_vector_vf32[INDEX_VY_REL] * state_vector_vf32[INDEX_X_REL]) ) ) / r_rel_pow3_f32;
                C_row_vf32[INDEX_Y_REL]      = ( -state_vector_vf32[INDEX_X_REL] * ( (state_vector_vf32[INDEX_VX_REL] * state_vector_vf32[INDEX_Y_REL])
                                                                                   - (state_vector_vf32[INDEX_VY_REL] * state_vector_vf32[INDEX_X_REL]) ) ) / r_rel_pow3_f32;
                C_row_vf32[INDEX_VX_REL]     = state_vector_vf32[INDEX_X_REL] / r_rel_f32;
                C_row_vf32[INDEX_VY_REL]     = state_vector_vf32[INDEX_Y_REL] / r_rel_f32;
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

PRIVATE void get_CACV_predicted_feature(F32* const p_pred_feature_f32, const F32* const state_vector_vf32, const F32* const input_vector_vf32, const MEASUREMENT_TYPES meas_type_e)
{
    /* local variables*/
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
            *p_pred_feature_f32    = state_vector_vf32[INDEX_VX_REL];
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
            r_rel_f32                       = sqrtf( squaref( state_vector_vf32[INDEX_X_REL] ) + squaref( state_vector_vf32[INDEX_Y_REL] ) );
            if ( r_rel_f32 > MINIMAL_DISTANCE_MEAS_MODEL )
            {
                *p_pred_feature_f32    = ( (state_vector_vf32[INDEX_VX_REL] * state_vector_vf32[INDEX_X_REL])
                                          + (state_vector_vf32[INDEX_VY_REL] * state_vector_vf32[INDEX_Y_REL]) ) / r_rel_f32;
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

PRIVATE void get_CACV_predicted_feature_input_noise(F32* const p_pred_feature_input_noise_f32, const F32* const state_vector_vf32, const INPUT_STRUCT* const p_input_state_s, const MEASUREMENT_TYPES meas_type_e)
{
    /* assign predicted feature input noise, this is always zero for CACV model*/
    *p_pred_feature_input_noise_f32    = 0.0F;
}

PRIVATE void init_CACV_state_vector(F32* const state_vector_vf32, const F32* const input_vector_vf32, const F32* const meas_feature_vf32)
{
    /* local variables*/
    F32 x_rel_f32       = meas_feature_vf32[MEAS_INIT_X_REL];
    F32 vx_rel_f32      = meas_feature_vf32[MEAS_INIT_VX_REL];
    F32 phi_rel_f32     = meas_feature_vf32[MEAS_INIT_PHI_REL];
    F32 cos_phi_rel_f32 = cosf(phi_rel_f32);
    F32 dphi_rel_f32    = meas_feature_vf32[MEAS_INIT_PHI_DOT_REL];


    /* assign individual vector entries */
    /* first the easy ones */
    state_vector_vf32[INDEX_X_REL]  = x_rel_f32;
    state_vector_vf32[INDEX_Y_REL]  = x_rel_f32 * tanf( phi_rel_f32 );
    state_vector_vf32[INDEX_VX_REL] = vx_rel_f32;
    state_vector_vf32[INDEX_AX_REL] = 0.0F;

    /* magnitude of speed vector */
    if ( fabsf(cos_phi_rel_f32) > SMALL_NUMBER )
    {
        state_vector_vf32[INDEX_VY_REL] = (tanf(phi_rel_f32) * vx_rel_f32) + ((x_rel_f32 * dphi_rel_f32) / squaref(cos_phi_rel_f32));
    }
    else
    {
        state_vector_vf32[INDEX_VY_REL] = 0.0F;
    }
}

PRIVATE void get_CACV_translated_state_vector(F32* const translated_state_vector_vf32, const F32* const state_vector_vf32, const F32 measurement_origin_x_relative_f32, const F32 measurement_origin_y_relative_f32)
{
    /* translate x and y position, this is dependent upon the state vector and hence upon the model */
    translated_state_vector_vf32[INDEX_X_REL]   = state_vector_vf32[INDEX_X_REL] - measurement_origin_x_relative_f32;
    translated_state_vector_vf32[INDEX_Y_REL]   = state_vector_vf32[INDEX_Y_REL] - measurement_origin_y_relative_f32;
    translated_state_vector_vf32[INDEX_VX_REL]  = state_vector_vf32[INDEX_VX_REL];
    translated_state_vector_vf32[INDEX_VY_REL]  = state_vector_vf32[INDEX_VY_REL];
    translated_state_vector_vf32[INDEX_AX_REL]  = state_vector_vf32[INDEX_AX_REL];
}

void create_CACV_Kalman_model(KALMAN_MODEL_STRUCT* const p_CACV_Kalman_model_s)
{
    if(NULL != p_CACV_Kalman_model_s)
    {
        /* reset the whole kalman model struct */
        (void)memset(p_CACV_Kalman_model_s, 0, sizeof(KALMAN_MODEL_STRUCT));

        /* assign existing functions     */
        p_CACV_Kalman_model_s->set_predicted_state_vector           = &set_CACV_predicted_state_vector;
        p_CACV_Kalman_model_s->get_A_matrix                         = &get_CACV_A_matrix;
        p_CACV_Kalman_model_s->get_process_noise_matrix             = &get_CACV_process_noise_matrix;
        p_CACV_Kalman_model_s->get_C_row                            = &get_CACV_C_row;
        p_CACV_Kalman_model_s->get_predicted_feature                = &get_CACV_predicted_feature;
        p_CACV_Kalman_model_s->get_predicted_feature_input_noise    = &get_CACV_predicted_feature_input_noise;
        p_CACV_Kalman_model_s->get_translated_state_vector          = &get_CACV_translated_state_vector;
        p_CACV_Kalman_model_s->init_state_vector                    = &init_CACV_state_vector;
        p_CACV_Kalman_model_s->model_type_u8                        = MODEL_CACV;
        p_CACV_Kalman_model_s->model_state_dim_u8                   = STATE_DIMENSION_CACV;
        p_CACV_Kalman_model_s->model_input_dim_u8                   = INPUT_DIMENSION;
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != p_CACV_Kalman_model_s)
    }
}
