
/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: least_squares.c $
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
#define LEAST_SQUARES          /*  defines this file */
#define UTIL_SUBSYSTEM         /*  defines the subsystem that this file resides in */

#include <mathf.h>
#include <assert.h>
#include <string.h>

#include "../FOUNDATION/FOUNDATION.h"

#include "../UTIL/least_squares.h"
#include "../UTIL/polynomial_line.h"
#include "../UTIL/matrix_manipulation.h"

/****
Function definitions for least squares
****/
void linear_weighted_least_squares(F32 linear_parameters_f32[], const F32 x_values_f32[], const F32 y_values_f32[], const F32 weights_f32[], const U8 value_length_u8)
{
    /*  local variables */
    F32     sum_weights_f32                 = 0.0;
    F32     weighted_sum_x_values_f32       = 0.0;
    F32     weighted_sum_y_values_f32       = 0.0;
    F32     weighted_sum_x_x_values_f32     = 0.0;
    F32     weighted_sum_x_y_values_f32     = 0.0;
    F32     denominator_f32;
    U8      index_value_u8;

    if((NULL != linear_parameters_f32)
        && (NULL != x_values_f32)
        && (NULL != y_values_f32)
        && (NULL != weights_f32) )
    {
        /*  compute sums */
        for (index_value_u8 = 0; index_value_u8 < value_length_u8; index_value_u8++)
        {
            /*  increments */
            sum_weights_f32             += weights_f32[index_value_u8];
            weighted_sum_x_values_f32   += x_values_f32[index_value_u8] * weights_f32[index_value_u8];
            weighted_sum_y_values_f32   += y_values_f32[index_value_u8] * weights_f32[index_value_u8];
            weighted_sum_x_x_values_f32 += x_values_f32[index_value_u8] * x_values_f32[index_value_u8] * weights_f32[index_value_u8];
            weighted_sum_x_y_values_f32 += x_values_f32[index_value_u8] * y_values_f32[index_value_u8] * weights_f32[index_value_u8];
        }

        /*  compute denominator */
        denominator_f32 = ( sum_weights_f32 * weighted_sum_x_x_values_f32 ) -( weighted_sum_x_values_f32 * weighted_sum_x_values_f32 );

        /*  check for singularity, magic numbers to be replaced with constants */
        if ( fabsf(denominator_f32) > 0.0001 )
        {
            linear_parameters_f32[0] = ( ( weighted_sum_y_values_f32 * weighted_sum_x_x_values_f32 ) - ( weighted_sum_x_values_f32 * weighted_sum_x_y_values_f32 ) ) / denominator_f32;
            linear_parameters_f32[1] = ( ( sum_weights_f32 * weighted_sum_x_y_values_f32 ) - ( weighted_sum_x_values_f32 * weighted_sum_y_values_f32 ) ) / denominator_f32;
        }
        else
        {
            linear_parameters_f32[0] = 0.0;
            linear_parameters_f32[1] = 9999.9;
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}

/****
Function definitions for least squares for parabola fitting
****/
BOOLEAN parabola_fitting_least_squares( F32* const parabola_parameters_vf32,
                                        F32* const p_fitting_error_f32,
                                        const F32 x_vf32[],
                                        const F32 y_vf32[],
                                        const U8 data_length_u8 )
{
    /*  local variables */
    F32         x_x_f32;
    F32         sum_x_f32               = 0.0F;
    F32         sum_y_f32               = 0.0F;
    F32         sum_x_x_f32             = 0.0F;
    F32         sum_x_x_x_f32           = 0.0F;
    F32         sum_x_x_x_x_f32         = 0.0F;
    F32         sum_x_y_f32             = 0.0F;
    F32         sum_x_x_y_f32           = 0.0F;
    F32         sum_y_y_f32             = 0.0F;
    const F32   data_length_f32         = (F32)data_length_u8;
    F32         sum_x_x_squared_f32;
    /* F32         fitting_error_alternative_f32 = 0.0F; */
    F32         D_f32;
    F32         D_0_f32;
    F32         D_1_f32;
    F32         D_2_f32;
    U8          index_u8;
    BOOLEAN     is_successful_fitting_b;
    F32         fitting_error_f32;
    /* local buffer for resulting parabola_parameters_vf32 */
    F32         result_parameters_vf32[3];


    /* fitting only possible if at least three data points */
    if ( (NULL != x_vf32)
         &&(NULL != y_vf32)
         && (data_length_u8 >= 3U) )
    {

        /*  compute sums */
        for ( index_u8 = 0U; index_u8 < data_length_u8; index_u8++ )
        {
            /* helper variable */
            x_x_f32 = x_vf32[index_u8] * x_vf32[index_u8];

            /*  sums */
            sum_x_f32       += x_vf32[index_u8];
            sum_y_f32       += y_vf32[index_u8];
            sum_x_x_f32     += x_x_f32;
            sum_x_x_x_f32   += x_x_f32 * x_vf32[index_u8];
            sum_x_x_x_x_f32 += x_x_f32 * x_x_f32;
            sum_x_y_f32     += x_vf32[index_u8] * y_vf32[index_u8];
            sum_x_x_y_f32   += x_x_f32 * y_vf32[index_u8];

            /* sum only used for calculation of fitting error */
            sum_y_y_f32     += y_vf32[index_u8]*y_vf32[index_u8];
        }

        /* helper variable */
        sum_x_x_squared_f32 = sum_x_x_f32*sum_x_x_f32;

        /*  compute determinants */
        D_f32 =     -( data_length_f32 * sum_x_x_x_f32 * sum_x_x_x_f32 )
                  + -( sum_x_f32 * sum_x_f32 * sum_x_x_x_x_f32 )
                  + -( sum_x_x_squared_f32 * sum_x_x_f32 )
                  +  ( data_length_f32 * sum_x_x_f32 * sum_x_x_x_x_f32 )
                  +  ( sum_x_f32 * sum_x_x_f32 * sum_x_x_x_f32 * 2.0F );

        D_0_f32 =   -( sum_y_f32*sum_x_x_x_f32 * sum_x_x_x_f32 )
                  + -( sum_x_x_squared_f32 * sum_x_x_y_f32 )
                  + -( sum_x_f32 * ( ( sum_x_y_f32 * sum_x_x_x_x_f32 ) - ( sum_x_x_x_f32 * sum_x_x_y_f32 ) ) )
                  +  ( sum_y_f32 * sum_x_x_f32 * sum_x_x_x_x_f32)
                  +  ( sum_x_x_f32 * sum_x_y_f32 * sum_x_x_x_f32 );

        D_1_f32 =   -( sum_x_x_squared_f32 * sum_x_y_f32 )
                  + -( sum_x_f32 * ( ( sum_y_f32 * sum_x_x_x_x_f32 ) - ( sum_x_x_f32 * sum_x_x_y_f32 ) ) )
                  +  ( data_length_f32 * sum_x_y_f32 * sum_x_x_x_x_f32 )
                  + -( data_length_f32 * sum_x_x_x_f32 * sum_x_x_y_f32 )
                  +  ( sum_y_f32  *sum_x_x_f32 * sum_x_x_x_f32 );

        D_2_f32 =   -( sum_y_f32 * sum_x_x_squared_f32)
                  + -( sum_x_f32 * sum_x_f32*sum_x_x_y_f32)
                  +  ( sum_x_f32 * ( ( sum_y_f32 * sum_x_x_x_f32 ) + ( sum_x_x_f32 * sum_x_y_f32 ) ) )
                  +  ( data_length_f32 * sum_x_x_f32 * sum_x_x_y_f32)
                  + -( data_length_f32 * sum_x_y_f32 * sum_x_x_x_f32);


        /*  check for singularity */
        if ( fabsf( D_f32 ) > SMALL_NUMBER )
        {
            /* use helper variable for 1/D_f32 ? */

            result_parameters_vf32[0] = D_0_f32 / D_f32;
            result_parameters_vf32[1] = D_1_f32 / D_f32;
            result_parameters_vf32[2] = D_2_f32 / D_f32;

            /* compute fitting error normalized by number of data points */
            fitting_error_f32 =     ( result_parameters_vf32[0] * result_parameters_vf32[0] * data_length_f32 )
                                 +  ( 2.0F * result_parameters_vf32[0] * result_parameters_vf32[1] * sum_x_f32 )
                                 +  ( 2.0F * result_parameters_vf32[0] * result_parameters_vf32[2] * sum_x_x_f32 )
                                 + -( 2.0F * result_parameters_vf32[0] * sum_y_f32 )
                                 +  ( result_parameters_vf32[1] * result_parameters_vf32[1] * sum_x_x_f32 )
                                 +  ( 2.0F*result_parameters_vf32[1] * result_parameters_vf32[2] * sum_x_x_x_f32 )
                                 + -( 2.0F*result_parameters_vf32[1] * sum_x_y_f32 )
                                 +  ( result_parameters_vf32[2] * result_parameters_vf32[2] * sum_x_x_x_x_f32 )
                                 + -( 2.0F*result_parameters_vf32[2] * sum_x_x_y_f32 )
                                 + sum_y_y_f32;

            /* alternative way for sanity check, not to be used in final code */
            /* compute sums
            for ( index_u8 = 0U; index_u8 < data_length_u8; index_u8++ )
            {
                fitting_error_alternative_f32 += squaref(y_f32[index_u8] - ( result_parameters_vf32[0] + ( result_parameters_vf32[1]
                                                                            + result_parameters_vf32[2] * x_f32[index_u8] ) * x_f32[index_u8] ) );
            }

            fitting_error_alternative_f32   = sqrtf( fitting_error_alternative_f32 ) / data_length_f32;*/

            if (fitting_error_f32 < 0.0F)
            {
                fitting_error_f32 = 0.0F;
            }
            else
            {
                fitting_error_f32 = sqrtf( fitting_error_f32 ) / data_length_f32;
            }

            /* assert( fabsf(fitting_error_f32 - fitting_error_alternative_f32) < 1.0e-4F); */

            /* fitting_error_f32 = 100000.0F * ( fitting_error_f32 - fitting_error_alternative_f32 ); */

            is_successful_fitting_b = TRUE;
        }
        else
        {
            /* fitting not successful */
            result_parameters_vf32[0] = 0.0F;
            result_parameters_vf32[1] = 0.0F;
            result_parameters_vf32[2] = 0.0F;

            fitting_error_f32           = 0.0F;

            is_successful_fitting_b = FALSE;
        }
    }
    else
    {
        /* fitting not successful */
        result_parameters_vf32[0] = 0.0F;
        result_parameters_vf32[1] = 0.0F;
        result_parameters_vf32[2] = 0.0F;

        fitting_error_f32           = 0.0F;

        is_successful_fitting_b = FALSE;

        ASSERT( (NULL != x_vf32) && (NULL != y_vf32) )
    }

    /* write back fitting_error_f32 */
    if (NULL != p_fitting_error_f32)
    {
        *p_fitting_error_f32 = fitting_error_f32;
    }
    else
    {
        /* do nothing */
    }

    /* write back parabola parameters */
    if (NULL != parabola_parameters_vf32)
    {
        (void)memcpy(parabola_parameters_vf32, result_parameters_vf32, sizeof(F32)*3);
    }
    else
    {
        /* do nothing */
    }

    return ( is_successful_fitting_b );
}


BOOLEAN polynomial_fitting_least_squares_c1c2c3( struct POLYNOMIAL_TRAJECTORY_TAG* const p_polynomial_trajectory_s,
                                                 const F32 x_vf32[],
                                                 const F32 y_vf32[],
                                                 const U8 n_f32 )
{
    /*  local variables */
    F32 x_i_f32;
    F32 y_i_f32;
    F32 matrix_vf32[9];
    U8  index_u8;
    U8  e_u8;

    /* x_exp[N] := x_i^N */
    F32 x_exp_vf32[7U];

    /* sum_xN := Sum[i=1:n](x_i^N) */
    F32 sum_x2_f32 = 0.F;
    F32 sum_x3_f32 = 0.F;
    F32 sum_x4_f32 = 0.F;
    F32 sum_x5_f32 = 0.F;
    F32 sum_x6_f32 = 0.F;

    /* sum_xNy := Sum[i=1:n](x_i^N * y_i) */
    F32 sum_xy_f32  = 0.F;
    F32 sum_x2y_f32 = 0.F;
    F32 sum_x3y_f32 = 0.F;

    /* Determinants */
    F32 D_f32;
    F32 D0_f32;
    F32 D1_f32;
    F32 D2_f32;

    /* resulting parameter */
    BOOLEAN is_successful_fitting_b;
    F32 c1_f32;
    F32 c2_f32;
    F32 c3_f32;


    /* fitting only possible if at least three data points */
    if ( n_f32 >= 3U )
    {
        /*  compute sums */
        for ( index_u8 = 0U; index_u8 < n_f32; index_u8++ )
        {
            /* helper variable */
            x_i_f32 = x_vf32[index_u8];
            y_i_f32 = y_vf32[index_u8];

            x_exp_vf32[0] = 1.0F;
            for( e_u8 = 1U; e_u8 <= 6; e_u8++)
            {
                x_exp_vf32[e_u8] = x_exp_vf32[e_u8-1] * x_i_f32;
            }

            /* sums */
            sum_x2_f32  += x_exp_vf32[2];
            sum_x3_f32  += x_exp_vf32[3];
            sum_x4_f32  += x_exp_vf32[4];
            sum_x5_f32  += x_exp_vf32[5];
            sum_x6_f32  += x_exp_vf32[6];

            sum_xy_f32  += x_exp_vf32[1] * y_i_f32;
            sum_x2y_f32 += x_exp_vf32[2] * y_i_f32;
            sum_x3y_f32 += x_exp_vf32[3] * y_i_f32;

        }

        /*  compute determinants */

        fill3x3Matrix(matrix_vf32, sum_x2_f32, sum_x3_f32, sum_x4_f32,
                                   sum_x3_f32, sum_x4_f32, sum_x5_f32,
                                   sum_x4_f32, sum_x5_f32, sum_x6_f32);
        D_f32 = Determinant(matrix_vf32, 3);

        fill3x3Matrix(matrix_vf32, sum_xy_f32,  sum_x3_f32, sum_x4_f32,
                                   sum_x2y_f32, sum_x4_f32, sum_x5_f32,
                                   sum_x3y_f32, sum_x5_f32, sum_x6_f32);
        D0_f32 = Determinant(matrix_vf32, 3);

        fill3x3Matrix(matrix_vf32, sum_x2_f32, sum_xy_f32,  sum_x4_f32,
                                   sum_x3_f32, sum_x2y_f32, sum_x5_f32,
                                   sum_x4_f32, sum_x3y_f32, sum_x6_f32);
        D1_f32 = Determinant(matrix_vf32, 3);

        fill3x3Matrix(matrix_vf32, sum_x2_f32, sum_x3_f32, sum_xy_f32,
                                   sum_x3_f32, sum_x4_f32, sum_x2y_f32,
                                   sum_x4_f32, sum_x5_f32, sum_x3y_f32);
        D2_f32 = Determinant(matrix_vf32, 3);


        /*  check for singularity */
        if ( fabsf( D_f32 ) > SMALL_NUMBER )
        {
            /* use helper variable for 1/D_f32 ? */

            is_successful_fitting_b = TRUE;
            c1_f32 = D0_f32 / D_f32;
            c2_f32 = D1_f32 / D_f32;
            c3_f32 = D2_f32 / D_f32;
        }
        else
        {
            /* fitting not successful */
            is_successful_fitting_b = FALSE;
            c1_f32 = 0.0F;
            c2_f32 = 0.0F;
            c3_f32 = 0.0F;
        }
    }
    else
    {
        /* fitting not successful */
        is_successful_fitting_b = FALSE;
        c1_f32 = 0.0F;
        c2_f32 = 0.0F;
        c3_f32 = 0.0F;
    }


    /* write back parabola parameters */
    if (NULL != p_polynomial_trajectory_s)
    {
        p_polynomial_trajectory_s->lateral_offset_f32   = 0.0F;
        p_polynomial_trajectory_s->tangential_angle_f32 = c1_f32;
        p_polynomial_trajectory_s->curvature_f32        = c2_f32 * 2.0F;
        p_polynomial_trajectory_s->curvature_rate_f32   = c3_f32 * 6.0F;
    }
    else
    {
        /* do nothing */
    }

    return ( is_successful_fitting_b );
}

