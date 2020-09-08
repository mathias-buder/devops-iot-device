
/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: newton_raphson.c $
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

#define NEWTON_RAPHSON          /*  defines this file */
#define UTIL_SUBSYSTEM          /*  defines the subsystem that this file resides in */

#include <mathf.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../TRACK/TRACK.h"

#include "../UTIL/polynomial_line.h"
#include "../UTIL/newton_raphson.h"


/*  computations from matlab: symbolic_newton_raphson.m */
F32 Closest_point_along_3rd_order_polynomial( const POLYNOMIAL_TRAJECTORY* const poly_coefficients_ps, const F32 x0_f32, const F32 y0_f32, const F32 start_x_f32, const U8 nr_iterations_u8 )
{
    U8  it_u8       = 0;           /*  current iteration */
    F32 x_f32       = start_x_f32; /*  current resulting x */
    F32 old_x_f32   = start_x_f32; /*  x from last iteration. if old vs new has a delta small enough, iteration is terminated */
    F32 x_poly_f32;
    F32 t0_f32;
    F32 t4_f32;
    F32 t5_f32;
    F32 t6_f32;
    F32 t7_f32;
    F32 t8_f32;
    F32 t9_f32;
    F32 t10_f32;
    F32 t11_f32;
    F32 t12_f32;
    F32 p0_f32;
    F32 p1_f32;
    F32 p2_f32;
    F32 p3_f32;


    if(NULL != poly_coefficients_ps)
    {
        p0_f32      = poly_coefficients_ps->lateral_offset_f32;                 /* assign polynomial coefficients */
        p1_f32      = poly_coefficients_ps->tangential_angle_f32;
        p2_f32      = 0.5F * poly_coefficients_ps->curvature_f32;
        p3_f32      = 0.166666667F * poly_coefficients_ps->curvature_rate_f32;

        /* Newton(-Raphson) iteration: x_{n+1} = x_n - f(x_n) / f'(x_n) */
        for ( it_u8 = 0; it_u8 < nr_iterations_u8; it_u8++ )
        {
            /* record old value for error threshold */
            old_x_f32 = x_f32;

            /* expressions for function and its derivative */
            /* from Poly_Path_Newton_Raphson_symbolic.m */
            x_poly_f32  = x_f32;
            t4_f32      = x_poly_f32*x_poly_f32;
            t5_f32      = p1_f32*x_poly_f32;
            t6_f32      = p2_f32*t4_f32;
            t7_f32      = p3_f32*t4_f32*x_poly_f32;

            /* DEVIATION:     3393
               DESCRIPTION:   Extra parentheses recommended. An arithmetic operation (* / + -) is the operand of a different operator with the same precedence.
               JUSTIFICATION: Auto-Generated Code from Matlab. */
            /* PRQA S 3393 1*/
            t8_f32      = p0_f32+t5_f32+t6_f32+t7_f32-y0_f32;
            t9_f32      = p2_f32*x_poly_f32*2.0F;
            t10_f32     = p3_f32*t4_f32*3.0F;
            t11_f32     = p1_f32+t10_f32+t9_f32;

            /* DEVIATION:     3395
               DESCRIPTION:   Extra parentheses recommended. A * or / operation is the operand of a + or - operator.
               JUSTIFICATION: Auto-Generated Code from Matlab. */
            /* PRQA S 3395 1*/
            t12_f32     = t8_f32*(p2_f32*2.0F+p3_f32*x_poly_f32*6.0F)+t11_f32*t11_f32+1.0F;  /* compute f'(x_n) */

            /* compute f(x_n) / f'(x_n) */
            if ( fabsf(t12_f32) > SMALL_NUMBER )
            {
                /* allow the division f(x_n) / f'(x_n) */

                /* DEVIATION:     3395
                   DESCRIPTION:   Extra parentheses recommended. A * or / operation is the operand of a + or - operator.
                   JUSTIFICATION: Auto-Generated Code from Matlab. */
                /* PRQA S 3395 1*/
                t0_f32 = (-x0_f32+x_poly_f32+t11_f32*t8_f32)/t12_f32;
            }
            else
            {
                /* set to default value */
                t0_f32 = 0.0F;
            }

            /* now do the Newton-Raphson iteration */
            x_f32 = x_f32 - t0_f32;

            /* either iterate until end or stop if accuracy is better than MAX_DELTA_X or |f'(x_n)| == 0 */
            if (    ( fabsf( old_x_f32 - x_f32 ) < MAX_DELTA_X )
                 || ( fabsf(t12_f32) <= SMALL_NUMBER ) )
            {
                /* exit from for-loop */
                break;
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != poly_coefficients_ps)
    }

    return x_f32;
}


