
/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: polynomial_line.c $
        $Revision: 1.1 $
        $Date: 2018/05/28 16:34:26CEST $

        TRW Ltd owns the copyright in this document and associated
        documents and all rights are reserved.  These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of TRW
        Ltd. Any copy of this drawing or document made by any method
        must also include a copy of this legend.

    $CopyrightDate: (c) ZF 2019 $

*********************************************************************/

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
 * @file polynomial_line
 * @details This module provides the operations to represent a trajectory as polynomial line.
 * The trajectory is given by
 * y = TRAJECTOY_LATERAL_OFFSET + x * tan(TRAJECTOY_TANGENTIAL_ANGLE)
 * + x^2 * 0.5 * TRAJECTOY_CURVATURE + x^3 * 1/6 * TRAJECTOY_CURVATURE_RATE
 */

#define UTIL_SUBSYSTEM
#define polynomial

/* TODO: Inserting extra parentheses to resolve remaining QA-C issues later as it is an error-prone process */

#include <string.h>
#include <mathf.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../UTIL/polynomial_line.h"

F32 get_curvature( const F32 x_f32, const POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s)
{
    F32 result_f32 = BIG_NUMBER;

    if(NULL != p_poly_coeff_s)
    {
        result_f32 = (p_poly_coeff_s->curvature_f32 + (x_f32 * p_poly_coeff_s->curvature_rate_f32));
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != p_poly_coeff_s)
    }
    return result_f32;
}

F32  polynomial_get_y_coordinate( const POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s, const F32 x_f32 )
{
    F32 result_f32 = BIG_NUMBER;

    if(NULL != p_poly_coeff_s)
    {
        /* y  = C0_f32 + C1_f32 * x + C2_f32 * x^2 + C3_f32 * x^3
         *    = offset + tan_angle * x + curvature/2 * x^2 + curvature_rate /6 * x^3  */
        result_f32 = (  p_poly_coeff_s->lateral_offset_f32
                       + (  (  p_poly_coeff_s->tangential_angle_f32
                            + ( ( (  p_poly_coeff_s->curvature_f32 * 0.5F )
                                  + ( p_poly_coeff_s->curvature_rate_f32 * 0.166666667F * x_f32 ) ) * x_f32 ) ) * x_f32 ) );
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != p_poly_coeff_s)
    }

    return result_f32;
}

F32  polynomial_get_derivative( const POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s, const F32 x_f32 )
{
    F32 y_derivative_f32;

    if(NULL != p_poly_coeff_s)
    {
        /* y  = C0_f32 + C1_f32 * x + C2_f32 * x^2 + C3_f32 * x^3        = offset + tan_angle * x + curvature/2 * x^2 + curvature_rate /6 * x^3  */
        /* y'  =         C1_f32     + C2_f32 * 2 * x + C3_f32 * 3 * x^2  =          tan_angle     + curvature   * x   + curvature_rate /2 * x^2  */
        y_derivative_f32 = p_poly_coeff_s->tangential_angle_f32
                         + ( ( p_poly_coeff_s->curvature_f32
                           + ( 0.5F * p_poly_coeff_s->curvature_rate_f32 * x_f32 ) ) * x_f32 );
    }
    else
    {
        y_derivative_f32 = BIG_NUMBER;
    }

    return y_derivative_f32;
}


void translate_polynomial( POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s, const F32 tx_f32, const F32 ty_f32 )
{
    POLYNOMIAL_TRAJECTORY temp_polynomial_s;

    /* do transformations*/

    if(NULL != p_poly_coeff_s)
    {
        temp_polynomial_s.lateral_offset_f32   =    ty_f32
                                                  + p_poly_coeff_s->lateral_offset_f32
                                                  + ( (   -p_poly_coeff_s->tangential_angle_f32
                                                       + (  (  ( p_poly_coeff_s->curvature_f32 * 0.5F )
                                                             - ( p_poly_coeff_s->curvature_rate_f32 * 0.166666667F * tx_f32 )
                                                            ) * tx_f32
                                                         )
                                                      ) * tx_f32 );

        temp_polynomial_s.tangential_angle_f32 =    p_poly_coeff_s->tangential_angle_f32
                                                 + -( tx_f32 * p_poly_coeff_s->curvature_f32 )
                                                 +  ( SQUARE(tx_f32) * p_poly_coeff_s->curvature_rate_f32 * 0.5F );

        temp_polynomial_s.curvature_f32        = p_poly_coeff_s->curvature_f32 - (p_poly_coeff_s->curvature_rate_f32 * tx_f32);

        temp_polynomial_s.curvature_rate_f32   = p_poly_coeff_s->curvature_rate_f32;

        (void)memcpy(p_poly_coeff_s, &temp_polynomial_s, sizeof(POLYNOMIAL_TRAJECTORY));
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != p_poly_coeff_s)
    }

}


F32 get_tangential_angle(const POLYNOMIAL_TRAJECTORY* const p_trajectory_s, const F32 x_f32)
{
    F32 angle_f32 = BIG_NUMBER;
    F32 tan_f32;

    if (NULL != p_trajectory_s)
    {
        tan_f32 = polynomial_get_derivative( p_trajectory_s, x_f32 );

        angle_f32 = atanf(tan_f32);
    }
    else
    {
        ASSERT(NULL != p_trajectory_s)
    }

    return angle_f32;
}


void set_polynomial_trajectory(POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s,
                                       const F32 in_lateral_offset_f32,
                                       const F32 in_tangential_angle_f32,
                                       const F32 in_curvature_f32,
                                       const F32 in_curvature_rate_f32)
{

    if(NULL != p_poly_coeff_s)
    {
        p_poly_coeff_s->lateral_offset_f32   = in_lateral_offset_f32;
        p_poly_coeff_s->tangential_angle_f32 = in_tangential_angle_f32;
        p_poly_coeff_s->curvature_f32        = in_curvature_f32;
        p_poly_coeff_s->curvature_rate_f32   = in_curvature_rate_f32;
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != p_poly_coeff_s)
    }
}

F32 get_polynomial_distance_to_origin(const POLYNOMIAL_TRAJECTORY* p_polynomial_s)
{
    F32 distance_to_origin_f32;

    if(NULL != p_polynomial_s)
    {
        distance_to_origin_f32 = p_polynomial_s->lateral_offset_f32 / (sqrtf(1.0F + SQUARE(p_polynomial_s->tangential_angle_f32)));
    }
    else
    {
        ASSERT(FALSE)
        distance_to_origin_f32 = BIG_NUMBER;
    }

    return distance_to_origin_f32;
}
