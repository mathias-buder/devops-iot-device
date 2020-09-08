/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: newton_raphson.h $
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

#ifndef NEWTON_RAPHSON_H
#define NEWTON_RAPHSON_H

    /**
     * @details if the difference between two iterations is smaller than this, it successfully terminates.
     */
    #define MAX_DELTA_X 0.05F /* m  */

    /**
     * @details This function returns the x which minimizes the distances of (px, py) to (x, poly(x)).
     * Uses the numeric newton approach. Values are in radar coordinates.
     * @param[in] 'poly_coefficients_vf32' 4-dimensional array. Stores C, B, A, dA of poly(x) = C + B*x + A*x^2 + dA*x^3.
     * @param[in] 'x0_f32' x-value of point to project onto polynomial.
     * @param[in] 'y0_f32' y-value of point to project onto polynomial.
     * @param[in] 'start_x_f32' value for x to start newton iteration.
     * @param[in] 'nr_iterations_u8' maximum nr. of iterations.
     * @return returns the x which minimizes the point's distance to the polynomial.
     */
    F32 Closest_point_along_3rd_order_polynomial( const POLYNOMIAL_TRAJECTORY* const poly_coefficients_ps, const F32 x0_f32, const F32 y0_f32, const F32 start_x_f32, const U8 nr_iterations_u8 );

#endif /* NEWTON_RAPHSON_H */
