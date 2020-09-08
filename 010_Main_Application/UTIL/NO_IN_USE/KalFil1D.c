
/* PRQA S 0292 17 */
/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: KalFil1D.c $
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
#include "../UTIL/KalFil1D.h"

/*************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                        */
/*************************************************************/

/**
 * @details         this function reinitializes the Kalman model in case of error encountered
 * @param[in, out]  'p_Kalman_model_s': pointer to the model used for 1D Kalman filter
 * @param[in]       'p_meas_feature_s': pointer to measurement structure
 */
PRIVATE void Kalman_filter_error_handle_OneDim( KALMAN_MODEL_ONEDIM_STRUCT* const p_Kalman_model_s,
                                                const SCALAR_MEAS_STRUCT* const p_meas_feature_s );

/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

void Kalman_filter_init_OneDim( KALMAN_MODEL_ONEDIM_STRUCT* const p_Kalman_model_s,
                                const SCALAR_STATE_STRUCT* const p_initial_state_s )
{
    if ( NULL != p_Kalman_model_s )
    {
        /* reset the whole Kalman model struct */
        (void)memset(p_Kalman_model_s, 0, sizeof(KALMAN_MODEL_ONEDIM_STRUCT));

        if ( NULL != p_initial_state_s )
        {
            /* initialize state mean and variance*/
            p_Kalman_model_s->state_s.mean_f32          = p_initial_state_s->mean_f32;
            p_Kalman_model_s->state_s.variance_f32      = p_initial_state_s->variance_f32;

            /* set has_been_initialised_b to true*/
            p_Kalman_model_s->has_been_initialised_b    = TRUE;
        }
        else
        {
            /* set has_been_initialised_b to false*/
            p_Kalman_model_s->has_been_initialised_b    = FALSE;
        }
    }
    else
    {
        /* Do nothing*/
        ASSERT(NULL != p_Kalman_model_s)
    }
}

void Kalman_filter_prediction_OneDim( KALMAN_MODEL_ONEDIM_STRUCT* const p_Kalman_model_s,
                                      const SCALAR_STATE_STRUCT* const p_input_s,
                                      const F32 process_variance_f32 )
{
    if ( NULL != p_Kalman_model_s )
    {
    #ifdef simulator
        /* save state before prediction*/
        p_Kalman_model_s->state_previous_s.mean_f32         = p_Kalman_model_s->state_s.mean_f32;
        p_Kalman_model_s->state_previous_s.variance_f32     = p_Kalman_model_s->state_s.variance_f32;
    #endif

        /* the calculation are based on following
         * A_f32 = 1.0F;
         * Ppred_f32 = A_f32 * p_state_s->variance_vf32 * A_f32;
         * Q_f32 = dt_f32 * dt_f32 * process_variance_f32;
         * p_state_s->variance_vf32 = Ppred_f32 + Q_f32;
         */

        /* predict mean */
        /* p_Kalman_model_s->state_s.mean_vf32 = p_Kalman_model_s->state_s.mean_vf32;  -> do nothing! */

        /* Predict state variance  only if process_variance_f32 is a positive value */
        if ( process_variance_f32 > 0.0F )
        {
            /* predict variance */
            p_Kalman_model_s->state_s.variance_f32      += process_variance_f32;
        }
        else
        {
            /* Do not add negative or zero process variance and set state is_in_error_state_b to be TRUE */
            p_Kalman_model_s->is_in_error_state_b = TRUE;
            ASSERT( process_variance_f32 > 0.0F )
        }

        /* if input state is not null and negative, then do more prediction */
        if ( NULL != p_input_s )
        {
            p_Kalman_model_s->state_s.mean_f32      += p_input_s->mean_f32;

            if ( p_input_s->variance_f32 > 0.0F )
            {
                p_Kalman_model_s->state_s.variance_f32  += p_input_s->variance_f32;
            }
            else
            {
                /* Do not add negative input variance */
            }
            /* Set ASSERT message */
            ASSERT( p_input_s->variance_f32 >= 0.0F )
        }
        else
        {
            /* Do nothing */
        }

    #ifdef simulator
        /* save input*/
        if ( NULL != p_input_s )
        {
            p_Kalman_model_s->input_for_prediction_s.mean_f32       = p_input_s->mean_f32;
            p_Kalman_model_s->input_for_prediction_s.variance_f32   = p_input_s->variance_f32;
        }
        else
        {
            p_Kalman_model_s->input_for_prediction_s.mean_f32       = 0.0F;
            p_Kalman_model_s->input_for_prediction_s.variance_f32   = 0.0F;
        }

        /* save predicted state*/
        p_Kalman_model_s->state_predicted_s.mean_f32                = p_Kalman_model_s->state_s.mean_f32;
        p_Kalman_model_s->state_predicted_s.variance_f32            = p_Kalman_model_s->state_s.variance_f32;

        /* save process variance*/
        p_Kalman_model_s->process_variance_f32                      = process_variance_f32;
    #endif
    }
    else
    {
        /* Do nothing*/
        ASSERT(NULL != p_Kalman_model_s)
    }
}

void Kalman_filter_update_OneDim( KALMAN_MODEL_ONEDIM_STRUCT* const p_Kalman_model_s,
                                  const SCALAR_STATE_STRUCT* const p_input_s,
                                  const SCALAR_MEAS_STRUCT* const p_meas_feature_s )
{
    /* local variables*/
    SCALAR_STATE_STRUCT pred_state_s;
    /*  DEVIATION:     781
        DESCRIPTION:   'innovation_variance_f32' and 'innovation_value_f32' are being used as a structure/union member as well as being a label, tag or ordinary identifier.
        JUSTIFICATION: For readability and maintainability, identical signals should have identical names. */
    /* PRQA S 781 01 */
    F32                 innovation_variance_f32;
    F32                 Kalman_gain_f32;

    if (   ( NULL != p_Kalman_model_s )
        && ( NULL != p_meas_feature_s )  )
    {
        if ( TRUE == p_meas_feature_s->measurement_valid_flag_b )
        {
            if (    ( TRUE  == p_Kalman_model_s->has_been_initialised_b )
                &&  ( FALSE == p_Kalman_model_s->is_in_error_state_b )   )
            {
                /* the calculation are based on following
                 * C_row_f32 = 1.0F;
                 * innovation_variance_f32 = C_row_f32 * p_state_s->variance_vf32 * C_row_f32;
                 * pred_feature_input_noise_f32 = 0.0F;
                 * innovation_variance_f32 += p_meas_feature_s->variance_vf32 + pred_feature_input_noise_f32;
                 */

                /* save the predicted state */
                pred_state_s.mean_f32       = p_Kalman_model_s->state_s.mean_f32;
                pred_state_s.variance_f32   = p_Kalman_model_s->state_s.variance_f32;

                /* calculate innovation variance */
                innovation_variance_f32 = pred_state_s.variance_f32 + p_meas_feature_s->variance_f32;

                /* calculate Kalman gain */
                if (    ( innovation_variance_f32           > SMALL_NUMBER_SQUARED )
                    &&  ( pred_state_s.variance_f32         > 0.0F )
                    &&  ( p_meas_feature_s->variance_f32    > 0.0F )    )
                {
                    Kalman_gain_f32 = pred_state_s.variance_f32 / innovation_variance_f32;
                }
                else /* if measurement or predicted state variance is not positive,
                        set Kalman gain to zero */
                {
                    Kalman_gain_f32         = 0.0F;
                    ASSERT( innovation_variance_f32 > SMALL_NUMBER_SQUARED )
                }
                ASSERT( pred_state_s.variance_f32       > SMALL_NUMBER_SQUARED )
                ASSERT( p_meas_feature_s->variance_f32  > SMALL_NUMBER_SQUARED )


                /* state variance update */
                p_Kalman_model_s->state_s.variance_f32  = pred_state_s.variance_f32 * ( 1.0F - Kalman_gain_f32);

                /* state mean update */
                p_Kalman_model_s->state_s.mean_f32      += ( Kalman_gain_f32 * (p_meas_feature_s->sample_f32 - pred_state_s.mean_f32) );
            }
            else
            {
                /* call error handling routine */
                Kalman_filter_error_handle_OneDim( p_Kalman_model_s, p_meas_feature_s);
            }
        }
        else /* no update if measurement is invalid */
        {
            /* do nothing */
        }

    #ifdef simulator
        /* save input for update*/
        if ( NULL != p_input_s )
        {
            p_Kalman_model_s->input_for_update_s.mean_f32           = p_input_s->mean_f32;
            p_Kalman_model_s->input_for_update_s.variance_f32       = p_input_s->variance_f32;
        }
        else
        {
            p_Kalman_model_s->input_for_update_s.mean_f32           = 0.0F;
            p_Kalman_model_s->input_for_update_s.variance_f32       = 0.0F;
        }

        /* save measurement*/
        p_Kalman_model_s->meas_feature_s.measurement_valid_flag_b   = p_meas_feature_s->measurement_valid_flag_b;
        p_Kalman_model_s->meas_feature_s.sample_f32                 = p_meas_feature_s->sample_f32;
        p_Kalman_model_s->meas_feature_s.variance_f32               = p_meas_feature_s->variance_f32;

        /* save Kalman gain*/
        p_Kalman_model_s->meas_update_analysis_s.Kalman_gain_column_vf32[0]    = Kalman_gain_f32;

        /* save innovation values*/
        if ( TRUE == p_meas_feature_s->measurement_valid_flag_b )
        {
            p_Kalman_model_s->meas_update_analysis_s.innovation_value_f32      = p_meas_feature_s->sample_f32 - pred_state_s.mean_f32;
            p_Kalman_model_s->meas_update_analysis_s.innovation_variance_f32   = innovation_variance_f32;
        }
        else
        {
            p_Kalman_model_s->meas_update_analysis_s.innovation_value_f32      = 0.0F;
            p_Kalman_model_s->meas_update_analysis_s.innovation_variance_f32   = 0.0F;
        }
    #endif
    }
    else
    {
        /* Do nothing*/
        ASSERT(FALSE)
    }
}

PRIVATE void Kalman_filter_error_handle_OneDim( KALMAN_MODEL_ONEDIM_STRUCT* const p_Kalman_model_s,
                                                const SCALAR_MEAS_STRUCT* const p_meas_feature_s )
{
    /* local constant */
    /* Use factor to avoid low value of measurement variance for initialization */
    const F32 variance_init_factor_f32   = 3.0F;

    /* local variables */
    SCALAR_STATE_STRUCT initial_state_s;

    if (   ( NULL != p_Kalman_model_s )
        && ( NULL != p_meas_feature_s )  )
    {
        if ( p_meas_feature_s->variance_f32 > 0.0F )
        {
            initial_state_s.mean_f32        = p_meas_feature_s->sample_f32;
            initial_state_s.variance_f32    = variance_init_factor_f32 * p_meas_feature_s->variance_f32;

            /* reinitialize the Kalman model with current measurement */
            Kalman_filter_init_OneDim( p_Kalman_model_s, &initial_state_s );
        }
        else
        {
            /* do nothing as measurement is not valid, but call assert */
            ASSERT( p_meas_feature_s->variance_f32 > 0.0F )
        }
    }
    else
    {
        /* do nothing, but call assert */
        ASSERT( NULL != p_Kalman_model_s )
        ASSERT( NULL != p_meas_feature_s )
    }
}

SCALAR_MEAS_STRUCT Kalman_filter_fuse_scalar_measurement(const SCALAR_MEAS_STRUCT* p_meas_1_s, const SCALAR_MEAS_STRUCT* p_meas_2_s)
{
    SCALAR_MEAS_STRUCT meas_r_s;

    if(FALSE == p_meas_2_s->measurement_valid_flag_b)
    {
        /* mease_2 invalid  or */
        /* both    invalid     */
        meas_r_s = *p_meas_1_s;
    }
    else if(FALSE == p_meas_1_s->measurement_valid_flag_b)
    {
        /* mease_1 invalid */
        meas_r_s = *p_meas_2_s;
    }
    else
    {
        /* both valid */
        meas_r_s.measurement_valid_flag_b = TRUE;

        meas_r_s.sample_f32 = ((p_meas_2_s->variance_f32*p_meas_1_s->sample_f32) + (p_meas_1_s->variance_f32*p_meas_2_s->sample_f32))
                                                /      (p_meas_1_s->variance_f32 + p_meas_2_s->variance_f32);

        meas_r_s.variance_f32 = (p_meas_1_s->variance_f32 * p_meas_2_s->variance_f32) / (p_meas_1_s->variance_f32 + p_meas_2_s->variance_f32);
    }

    return meas_r_s;
}
