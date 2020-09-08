
/* PRQA S 0292 17 */
/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: KalFilGen.c $
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

/*************************************************************/
/*      FILE DEFINITIONS                                     */
/*************************************************************/


/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/

#include <mathf.h>
#include <string.h>

#include "../FOUNDATION/FOUNDATION.h"

#include "../UTIL/ModTypes.h"
#include "../UTIL/ModCAA.h"
#include "../UTIL/ModCACV.h"
#include "../UTIL/matrix_manipulation.h"
#include "../UTIL/KalFilGen.h"

/*************************************************************/
/*      LOCAL VARIABLES                                      */
/*************************************************************/


/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/
/**
 * @details:
 * brief description: single measurement update, for details please see Kalman_filter_update
 * @param[in] 'p_Kalman_model_s': pointer to Kalman model
 * @param[in] 'p_input_s': pointer to input struct
 * @param[in] 'p_meas_s': pointer to scalar measurement struct
 * @param[out] 'p_meas_update_analysis_s': pointer to scalar measurement update analysis struct (optional, this pointer can be NULL)
 */
PRIVATE void Kalman_filter_scalar_update( KALMAN_MODEL_STRUCT* const p_Kalman_model_s,
                                          const INPUT_STRUCT* const p_input_s,
                                          const SCALAR_MEAS_STRUCT* const p_meas_s,
                                          SCALAR_MEAS_UPDATE_ANALYSIS* const p_meas_update_analysis_s );

/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/


void Kalman_filter_init( KALMAN_MODEL_STRUCT* const p_Kalman_model_s,
                         const enum KALMAN_MODEL_TYPES model_type_e,
                         const F32* const input_vector_vf32,
                         const F32* const initial_meas_feature_vf32,
                         const F32* const initial_state_covariance_vf32 )
{
    if (NULL != p_Kalman_model_s)
    {
        if (    (NULL != input_vector_vf32)
            &&  (NULL != initial_meas_feature_vf32)
            &&  (NULL != initial_state_covariance_vf32) )
        {
            switch(model_type_e)
            {
            case MODEL_CAA:
                create_CAA_Kalman_model(p_Kalman_model_s);
                break;
            case MODEL_CACV:
                create_CACV_Kalman_model(p_Kalman_model_s);
                break;
            default:
                ASSERT( (model_type_e == MODEL_CAA)||(model_type_e == MODEL_CACV) )
                break;
            }

            /* initialize state based on the measurement vector */
            p_Kalman_model_s->init_state_vector(p_Kalman_model_s->state_s.mean_vf32, input_vector_vf32, initial_meas_feature_vf32);

            /* initialize state covariance */
            (void)memcpy(&p_Kalman_model_s->state_s.covariance_vf32, initial_state_covariance_vf32, NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)*sizeof(F32));

            /* set state has_been_initialised_b flag to true */
            p_Kalman_model_s->has_been_initialised_b    = TRUE;
            /* set is_in_error_state boolean to be false */
            p_Kalman_model_s->is_in_error_state_b       = FALSE;
        }
        else
        {
            /* set state has_been_initialised_b flag to False */
            p_Kalman_model_s->has_been_initialised_b    = FALSE;
            ASSERT(FALSE)
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

}


void Kalman_filter_prediction( KALMAN_MODEL_STRUCT* const p_Kalman_model_s,
                               const INPUT_STRUCT* const p_input_s,
                               const F32* const process_variance_vf32,
                               const F32* const stabilizing_noise_covariance_vf32,
                               const F32 dt_f32 )
{
    /* local constant value initialization */
    const F32   cov_valid_thres_f32             = 0.0F;

    /* local variables */
    F32         A_vf32[STATE_DIMENSION_MAX*STATE_DIMENSION_MAX];
    F32         Q_vf32[NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)];
    F32         Ppred_vf32[NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)];

    if(    (NULL != p_Kalman_model_s)
        && (NULL != p_input_s)
        && (NULL != process_variance_vf32)
        && (NULL != stabilizing_noise_covariance_vf32) )
    {
#ifdef simulator
        /* save state before kalman prediction */
        (void)memcpy(&p_Kalman_model_s->state_previous_s, &p_Kalman_model_s->state_s, sizeof(STATE_STRUCT));

        /* save input struct for kalman prediction */
        (void)memcpy(&p_Kalman_model_s->input_for_prediction_s, p_input_s, sizeof(INPUT_STRUCT));

        /* save more process noise data for kalman prediction */
        (void)memcpy(&p_Kalman_model_s->process_variance_vf32, process_variance_vf32, INPUT_DIMENSION * sizeof(F32));
        (void)memcpy(&p_Kalman_model_s->stabilizing_noise_covariance_matrix_vf32, stabilizing_noise_covariance_vf32,  NR_ELEMENTS_SYM(STATE_DIMENSION_MAX) * sizeof(F32));
#endif

        /** === begin of kalman prediction === */

        /* get the A matrix*/
        p_Kalman_model_s->get_A_matrix(A_vf32, p_Kalman_model_s->state_s.mean_vf32, p_input_s->mean_vf32, dt_f32);

        /* covariance matrix propagation*/
        matrixQuadraticForm( Ppred_vf32, A_vf32, p_Kalman_model_s->state_s.covariance_vf32, p_Kalman_model_s->model_state_dim_u8, p_Kalman_model_s->model_state_dim_u8 );

        /* get the process noise matrix*/
        p_Kalman_model_s->get_process_noise_matrix(Q_vf32, p_Kalman_model_s->state_s.mean_vf32, p_input_s, process_variance_vf32, stabilizing_noise_covariance_vf32, dt_f32);

        /* check the calculated process noise covariance matrix validity */
        if ( FALSE == isCovarianceMatrixValid(Q_vf32, p_Kalman_model_s->model_state_dim_u8, cov_valid_thres_f32) )
        {
            p_Kalman_model_s->is_in_error_state_b = TRUE;
        }
        else
        {
            /* is_in_error_state_b flag already set to FALSE */
        }

        /* addition of process noise*/
        SymMatrixAddition( p_Kalman_model_s->state_s.covariance_vf32, 1.0F, Ppred_vf32, 1.0F, Q_vf32, p_Kalman_model_s->model_state_dim_u8 );

        /* check the predicted state covariance matrix validity */
        if ( FALSE == isCovarianceMatrixValid(p_Kalman_model_s->state_s.covariance_vf32, p_Kalman_model_s->model_state_dim_u8, cov_valid_thres_f32) )
        {
            p_Kalman_model_s->is_in_error_state_b = TRUE;
        }
        else
        {
            /* is_in_error_state_b flag is already set to FALSE */
        }

        /* state prediction*/
        p_Kalman_model_s->set_predicted_state_vector(p_Kalman_model_s->state_s.mean_vf32, p_input_s->mean_vf32, dt_f32);

        /** === end of kalman prediction === */

#ifdef simulator
        /* increase pure prediction time after Kalman prediction */
        p_Kalman_model_s->pure_prediction_time_f32 += dt_f32;

        /* save state after kalman prediction */
        (void)memcpy(&p_Kalman_model_s->state_predicted_s, &p_Kalman_model_s->state_s, sizeof(STATE_STRUCT));

        /* save A matrix */
        (void)memcpy(&p_Kalman_model_s->A_matrix_vf32, &A_vf32, STATE_DIMENSION_MAX*STATE_DIMENSION_MAX * sizeof(F32));

        /* save Q matrix */
        (void)memcpy(&p_Kalman_model_s->Q_matrix_vf32, &Q_vf32, NR_ELEMENTS_SYM(STATE_DIMENSION_MAX) * sizeof(F32));

#endif
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}

void Kalman_filter_update( KALMAN_MODEL_STRUCT* const p_Kalman_model_s,
                           const INPUT_STRUCT* const p_input_s,
                           MEAS_SEQUENCE* const p_meas_sequence_s )
{
    /* local variables*/
    U8                              meas_index_u8;
    const SCALAR_MEAS_STRUCT*       p_current_meas_s;
    SCALAR_MEAS_UPDATE_ANALYSIS*    p_current_meas_update_analysis_s;

#ifdef simulator
    /* save input struct for kalman prediction */
    (void)memcpy(&p_Kalman_model_s->input_for_update_s, p_input_s, sizeof(INPUT_STRUCT));
#endif

    /* Perform filter update if all input parameters are valid */
    if (   (NULL != p_input_s)
        && (NULL != p_meas_sequence_s)
        && (NULL != p_Kalman_model_s)
        )
    {
        /* sequential updating, loop through measurement indices*/
        for ( meas_index_u8 = 0; meas_index_u8 < p_meas_sequence_s->number_measurements_u8; meas_index_u8++ )
        {
            /* get current scalar measurement */
            p_current_meas_s = &p_meas_sequence_s->meas_vs[meas_index_u8];

            /* get current analysis struct */
            if (NULL != p_meas_sequence_s->meas_update_analysis_vs)
            {
                p_current_meas_update_analysis_s = &p_meas_sequence_s->meas_update_analysis_vs[meas_index_u8];
            }
            else
            {
                p_current_meas_update_analysis_s = NULL;
            }

            /* check if this scalar measurement should be used*/
            if (TRUE == p_current_meas_s->measurement_valid_flag_b)
            {
                /* then perform Kalman filter scalar update */
                Kalman_filter_scalar_update( p_Kalman_model_s,
                                             p_input_s,
                                             p_current_meas_s,
                                             p_current_meas_update_analysis_s );
            }
            else
            {
                /* no Kalman update when measurement is not valid */
            }
        }
    }
    else
    {
        ASSERT(FALSE)
    }

}


PRIVATE void Kalman_filter_scalar_update( KALMAN_MODEL_STRUCT* const p_Kalman_model_s,
                                          const INPUT_STRUCT* const p_input_s,
                                          const SCALAR_MEAS_STRUCT* const p_meas_s,
                                          SCALAR_MEAS_UPDATE_ANALYSIS* const p_meas_update_analysis_s )
{
    /* local variables*/
    F32                             pred_feature_input_noise_f32;
    F32                             pred_feature_f32;
    F32                             pred_feature_variance_vf32[1];
    /*  DEVIATION:     781
        DESCRIPTION:   'innovation_variance_f32' and 'innovation_value_f32' are being used as a structure/union member as well as being a label, tag or ordinary identifier.
        JUSTIFICATION: For readability and maintainability, identical signals should have identical names. */
    /* PRQA S 781 02 */
    F32                             innovation_variance_f32;
    F32                             innovation_value_f32;
    F32                             C_row_vf32[STATE_DIMENSION_MAX];
    F32                             Kalman_gain_pseudo_vf32[STATE_DIMENSION_MAX];
    F32                             auxiliary_covariance_vf32[NR_ELEMENTS_SYM(STATE_DIMENSION_MAX)];
    F32                             translated_state_vector_vf32[STATE_DIMENSION_MAX];                   /* state vector transformed to origin of measurement */
    SCALAR_MEAS_UPDATE_ANALYSIS     analysis_s;


    /* reset analysis struct */
    (void)memset(&analysis_s, 0, sizeof(SCALAR_MEAS_UPDATE_ANALYSIS));

    /* transform state vector to coordinate origin of measurement (sensor) */
    p_Kalman_model_s->get_translated_state_vector(translated_state_vector_vf32, p_Kalman_model_s->state_s.mean_vf32, p_meas_s->measurement_origin_s.x_f32, p_meas_s->measurement_origin_s.y_f32);

    /* get C matrix with translated state vector */
    p_Kalman_model_s->get_C_row(C_row_vf32, translated_state_vector_vf32, p_input_s->mean_vf32, p_meas_s->meas_type_e);

    /* get predicted feature with translated state vector */
    p_Kalman_model_s->get_predicted_feature(&pred_feature_f32, translated_state_vector_vf32, p_input_s->mean_vf32, p_meas_s->meas_type_e);

    /* get predicted feature input noise with translated state vector */
    p_Kalman_model_s->get_predicted_feature_input_noise(&pred_feature_input_noise_f32, translated_state_vector_vf32, p_input_s, p_meas_s->meas_type_e);

    /* get predicted feature variance */
    matrixQuadraticForm( pred_feature_variance_vf32, C_row_vf32, p_Kalman_model_s->state_s.covariance_vf32, 1, p_Kalman_model_s->model_state_dim_u8 );

    /* calculate innovation variance */
    if (   ( pred_feature_variance_vf32[0]  > 0.0F )
        && ( p_meas_s->variance_f32         > 0.0F )   )
    {
        if (pred_feature_input_noise_f32 > 0.0F)
        {
            innovation_variance_f32 = pred_feature_variance_vf32[0] + p_meas_s->variance_f32 + pred_feature_input_noise_f32;
        }
        else
        {
            /* do not add negative or zero input noise */
            innovation_variance_f32 = pred_feature_variance_vf32[0] + p_meas_s->variance_f32;
            ASSERT(pred_feature_input_noise_f32 >= 0.0F)
        }
    }
    else
    {
        innovation_variance_f32 = 0.0F;
    }
    ASSERT( pred_feature_variance_vf32[0]    > SMALL_NUMBER_SQUARED )
    ASSERT( p_meas_s->variance_f32           > SMALL_NUMBER_SQUARED )

    /* calculate innovation value */
    innovation_value_f32 = p_meas_s->sample_f32 - pred_feature_f32;

    /* save measurement struct to analysis struct */
    analysis_s.innovation_value_f32          = innovation_value_f32;
    analysis_s.innovation_variance_f32       = innovation_variance_f32;

#ifdef simulator
    analysis_s.pred_feature_vector_f32       = pred_feature_f32;
    analysis_s.pred_feature_variance_f32     = pred_feature_variance_vf32[0];
    analysis_s.pred_feature_input_noise_f32  = pred_feature_input_noise_f32;
    (void)memcpy( analysis_s.pre_update_covariance_vf32, p_Kalman_model_s->state_s.covariance_vf32, NR_ELEMENTS_SYM(STATE_DIMENSION_MAX) * sizeof (F32) );
#endif

    /* innovation variance must always be positive and larger than epsilon to prevent ZDV */
    if ( innovation_variance_f32 > SMALL_NUMBER_SQUARED )
    {
        /* Kalman pseudo gain (no division by innovation covariance)*/
        symMatrixMatrixMult(Kalman_gain_pseudo_vf32, p_Kalman_model_s->state_s.covariance_vf32, C_row_vf32, p_Kalman_model_s->model_state_dim_u8, 1);

        /* update covariance, first aux matrix*/
        VectorTensorProduct(auxiliary_covariance_vf32, Kalman_gain_pseudo_vf32, p_Kalman_model_s->model_state_dim_u8);

        /* then do the addition */
        SymMatrixAddition( p_Kalman_model_s->state_s.covariance_vf32,
                           1.0F, p_Kalman_model_s->state_s.covariance_vf32,
                           (-1.0F / innovation_variance_f32), auxiliary_covariance_vf32,
                           p_Kalman_model_s->model_state_dim_u8 );

        /* state update */
        MatrixAddition( p_Kalman_model_s->state_s.mean_vf32,
                        1.0F, p_Kalman_model_s->state_s.mean_vf32,
                        (innovation_value_f32 / innovation_variance_f32), Kalman_gain_pseudo_vf32,
                        p_Kalman_model_s->model_state_dim_u8, 1 );

#ifdef simulator
        /* reset pure prediction time after Kalman update */
        p_Kalman_model_s->pure_prediction_time_f32 = 0.0F;

        /* calculate and save Kalman gain = (Kalman_gain_pseudo_vf32 / innovation_variance_f32) */
        scalarMatrixMult(analysis_s.Kalman_gain_column_vf32, (1.0F / innovation_variance_f32), Kalman_gain_pseudo_vf32, p_Kalman_model_s->model_state_dim_u8, 1);

        scalarMatrixMult(analysis_s.delta_vf32, innovation_value_f32, analysis_s.Kalman_gain_column_vf32, p_Kalman_model_s->model_state_dim_u8, 1);

        /* copy C row array into matrix */
        (void)memcpy(analysis_s.C_row_vf32, C_row_vf32, sizeof(F32) * p_Kalman_model_s->model_state_dim_u8);

        (void)memcpy( analysis_s.post_update_covariance_vf32, p_Kalman_model_s->state_s.covariance_vf32, NR_ELEMENTS_SYM(STATE_DIMENSION_MAX) * sizeof (F32) );
#endif

    }
    else
    {
        /* Skip Kalman update if innovation variance is negative or below epsilon! */
        ASSERT( innovation_variance_f32 > SMALL_NUMBER_SQUARED )
    }

    /* output analysis info from meas update */
    if (NULL != p_meas_update_analysis_s)
    {
        (void)memcpy(p_meas_update_analysis_s, &analysis_s, sizeof(SCALAR_MEAS_UPDATE_ANALYSIS));
    }
    else
    {
        /* do nothing */
    }

}
