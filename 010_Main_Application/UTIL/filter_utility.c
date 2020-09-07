/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: filter_utility.c $
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

*********************************************************************/


/**
 * @file    filter_utility.c
 * @brief   utility functions to support filter algorithm
 */


/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <stdlib.h>
#include <mathf.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../UTIL/filter_utility.h"
#include "../UTIL/vector_manipulation.h"
#include "../UTIL/matrix_manipulation.h"

/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/

#if(defined(simulator) && !defined(ACSim))
    F32 erfcf(const F32 x_f32);
#endif


/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

BOOLEAN discrete_Bayes_update( F32 posterior_prob_vector_vf32[], const F32 predicted_prob_vector_vf32[],
                               const F32 measurement_likelihood_matrix_vf32[], const U8 meas_index_u8, const U8 dim_u8 )
{
    /*  locals */
    U8          iIndex_u8;
    F32         normalization_f32 = 0.0F;
    BOOLEAN     is_valid_b;

    if ( (NULL != posterior_prob_vector_vf32)
          && (NULL != predicted_prob_vector_vf32)
          && (NULL != measurement_likelihood_matrix_vf32)
          && (meas_index_u8 < dim_u8) )
    {
        /*  compute normalization */
        for ( iIndex_u8 = 0; iIndex_u8 < dim_u8; iIndex_u8++ )
        {
            normalization_f32 += measurement_likelihood_matrix_vf32[ GEN_IDX(meas_index_u8,iIndex_u8,dim_u8) ]
                                    * predicted_prob_vector_vf32[iIndex_u8];
        }

        if ( fabsf( normalization_f32 ) > DIV_THRESHOLD )
        {
            /*  compute update */
            for ( iIndex_u8 = 0; iIndex_u8 < dim_u8; iIndex_u8++ )
            {
                posterior_prob_vector_vf32[iIndex_u8] = ( measurement_likelihood_matrix_vf32[ GEN_IDX(meas_index_u8,iIndex_u8,dim_u8) ]
                                                    * predicted_prob_vector_vf32[iIndex_u8] ) / normalization_f32;
            }

            is_valid_b = TRUE;
        }
        else
        {
            is_valid_b = FALSE;
        }
    }
    else
    {
        is_valid_b = FALSE;

        ASSERT(FALSE)
    }

    return is_valid_b;
}


#if(defined(simulator) && !defined(ACSim))
/* Complementary error function, only used in old simulator */
F32 erfcf(const F32 x_f32)
{
    /* taken from matlab's erfcore(x,1) implementation
       Borland's math.h does not contain erfcf.c, but Windriver's does
       this is adapted from the erf function in 4151_object_confidence.c in Fusion_AC3B_Video_PowerPC/Trunk/Main_Apps/FUSION/ from 1.9.2009 */

    /* defines */
    #define ONE_OVER_SQRT_PI   0.564189583547756F

    /* constant values */
    const F32 xbreak_f32   = 0.46875F;
    const F32 a_f32[]      = {3.16112374387056560e00, 1.13864154151050156e02,
                              3.77485237685302021e02, 3.20937758913846947e03,
                              1.85777706184603153e-1};
    const F32 b_f32[]      = {2.36012909523441209e01, 2.44024637934444173e02,
                              1.28261652607737228e03, 2.84423683343917062e03};
    const F32 c_f32[]      = {5.64188496988670089e-1, 8.88314979438837594e00,
                              6.61191906371416295e01, 2.98635138197400131e02,
                              8.81952221241769090e02, 1.71204761263407058e03,
                              2.05107837782607147e03, 1.23033935479799725e03,
                              2.15311535474403846e-8};
    const F32 d_f32[]      = {1.57449261107098347e01, 1.17693950891312499e02,
                              5.37181101862009858e02, 1.62138957456669019e03,
                              3.29079923573345963e03, 4.36261909014324716e03,
                              3.43936767414372164e03, 1.23033935480374942e03};
    const F32 p_f32[]      = {3.05326634961232344e-1, 3.60344899949804439e-1,
                              1.25781726111229246e-1, 1.60837851487422766e-2,
                              6.58749161529837803e-4, 1.63153871373020978e-2};
    const F32 q_f32[]      = {2.56852019228982242e00, 1.87295284992346047e00,
                              5.27905102951428412e-1, 6.05183413124413191e-2,
                              2.33520497626869185e-3};

    /* local variables */
    F32 y_f32;
    F32 z_f32;
    F32 del_f32;
    F32 xnum_f32;
    F32 xden_f32;
    F32 result_f32;
    U8  index_u8;

    if ( fabsf(x_f32) <= xbreak_f32 )
    {
        z_f32       = x_f32 * x_f32;
        xnum_f32    = a_f32[4] * z_f32;
        xden_f32    = z_f32;

        for (index_u8 = 0; index_u8 < 3; index_u8++)
        {
            xnum_f32 = (xnum_f32 + a_f32[index_u8]) * z_f32;
            xden_f32 = (xden_f32 + b_f32[index_u8]) * z_f32;
        }

        result_f32 = x_f32 * (xnum_f32 + a_f32[3]) / (xden_f32 + b_f32[3]);
        /* for erfc */
        result_f32 = 1.0F - result_f32;
    }
    else if ( fabsf(x_f32) <= 4.0F )
    {
        y_f32       = fabsf(x_f32);
        xnum_f32    = c_f32[8]*y_f32;
        xden_f32    = y_f32;

        for (index_u8 = 0; index_u8 < 7; index_u8++)
        {
            xnum_f32 = (xnum_f32 + c_f32[index_u8]) * y_f32;
            xden_f32 = (xden_f32 + d_f32[index_u8]) * y_f32;
        }

        result_f32  = (xnum_f32 + c_f32[7]) / (xden_f32 + d_f32[7]);
        z_f32       = floorf(y_f32*16.0F)*0.0625F;
        del_f32     = (y_f32-z_f32)*(y_f32+z_f32);
        result_f32  = expf(-z_f32*z_f32) * expf(-del_f32) * result_f32;
    }
    else
    {
        y_f32 = fabsf(x_f32);
        z_f32 = 1.0F / (y_f32 * y_f32);
        xnum_f32 = p_f32[5]*z_f32;
        xden_f32 = z_f32;

        for (index_u8 = 0; index_u8 < 4; index_u8++)
        {
            xnum_f32 = (xnum_f32 + p_f32[index_u8]) * z_f32;
            xden_f32 = (xden_f32 + q_f32[index_u8]) * z_f32;
        }

        result_f32 = z_f32 * (xnum_f32 + p_f32[4]) / (xden_f32 + q_f32[4]);
        result_f32 = (ONE_OVER_SQRT_PI - result_f32) / y_f32;
        z_f32       = floorf(y_f32*16.0F)*0.0625F;
        del_f32     = (y_f32-z_f32)*(y_f32+z_f32);
        result_f32  = expf(-z_f32*z_f32) * expf(-del_f32) * result_f32;
        result_f32  = CLAMP(result_f32,-1.0F,1.0F);
    }

    if (x_f32 < -xbreak_f32)
    {
        result_f32 = 2.0F - result_f32;
    }
    else
    {
        /* no change */
        /* result_f32 = result_f32; */
    }

    return result_f32;
}
#endif


F32 calculate_probability_inside_interval( const F32 left_border_f32, const F32 right_border_f32, const F32 mean_f32, const F32 variance_f32 )
{
    /* constant variables */
    const F32 MINUS_INV_SQRT_TWO = (-0.707106781186547F);    /* -1/sqrtf(2) */

    /* local variables */
    F32 prob_left_of_left_f32;
    F32 prob_left_of_right_f32;
    F32 prob_inside_interval_f32;
    F32 standard_deviation_f32;

    /* input parameter left_border_f32 should always smaller than right_border_f32 */
    if (left_border_f32 < right_border_f32)
    {
        /* check singularity: ensure positive variance and avoid division by zero */
        if( variance_f32 > (SMALL_NUMBER*SMALL_NUMBER) )
        {
            standard_deviation_f32 = sqrtf( variance_f32 );
            /* compute individual probabilities according to the two boundaries */
            prob_left_of_left_f32  = 0.5F * erfcf( (MINUS_INV_SQRT_TWO * ( left_border_f32 - mean_f32 )) / standard_deviation_f32 );
            prob_left_of_right_f32 = 0.5F * erfcf( (MINUS_INV_SQRT_TWO * ( right_border_f32 - mean_f32 )) / standard_deviation_f32 );

            /* then compute occupancy probability */
            prob_inside_interval_f32 = prob_left_of_right_f32 - prob_left_of_left_f32;
        }
        else
        {
            /* check if the position is within boundary (left_border_f32 <= position_f32 <= right_border_f32) */
            if (   (mean_f32 >= left_border_f32)
                && (mean_f32 <= right_border_f32)
                )
            {
                /* set probability to max */
                prob_inside_interval_f32 = 1.0F;
            }
            else
            {
                /* set probability to min */
                prob_inside_interval_f32 = 0.0F;
            }
        }
    }
    else
    {
        prob_inside_interval_f32 = 0.0F;
        ASSERT(left_border_f32 < right_border_f32)
    }

    /* clamp the probability to (0, 1), defensive coding */
    prob_inside_interval_f32 = CLAMP(prob_inside_interval_f32, 0.0F, 1.0F);

    return prob_inside_interval_f32;
}
