
/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: polynomial_line.h $
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
/**
 * @file polynomial_line
 * @details This module provides the operations to represent a trajectory as polynomial line.
 * The trajectory is given by
 * y = TRAJECTOY_LATERAL_OFFSET + x * tan(TRAJECTOY_TANGENTIAL_ANGLE)
 * + x^2 * 0.5 * TRAJECTOY_CURVATURE + x^3 * 1/6 * TRAJECTOY_CURVATURE_RATE
 */

#ifndef POLYNOMIAL_LINE_H
#define POLYNOMIAL_LINE_H


    /**
     * @details Structure describing a trajectory (third order polynomial)
     */
    typedef struct POLYNOMIAL_TRAJECTORY_TAG {
        F32 lateral_offset_f32;    /**< in [m], positive to the right        */
        F32 tangential_angle_f32;  /**< tangent of angle, clockwise positive */
        F32 curvature_f32;         /**< in [1/m], positive to the right      */
        F32 curvature_rate_f32;    /**< in [1/m^2], positive to the right    */
    }POLYNOMIAL_TRAJECTORY;

    /**
     * @details This function evaluates the polynomial at x_f32.
     * @param[in] x_f32
     * @param[in] poly_coeff_vf32
     * @return polynomial at 'x_f32'.
     */
    F32  polynomial_get_y_coordinate( const POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s,  const F32 x_f32 );

    /**
     * @details This function evaluates the derivative of a polynomial at x_f32.
     * @param[in] x_f32             test range [m]
     * @param[in] poly_coeff_vf32   polynomial
     * @return derivative of polynomial at 'x_f32'.
     */
    F32  polynomial_get_derivative( const POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s, const F32 x_f32 );


    /**
     * @details This function translates polynomial by tx_f32 and ty_f32.
     * @param[in/out] poly_coeff_vf32 the polynomial which shall be rotated
     * @param[in] tx_f32 offset in x-direction the line shall be translated
     * @param[in] ty_f32 offset in y-direction the line shall be translated (y positive to right)
     */
    void translate_polynomial( POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s, const F32 tx_f32, const F32 ty_f32 );

    /**
     * @details This function returns the curvature at 'x_f32'.
     * @param[in] 'x_f32' is the range we want to determine the curvature for.
     * @param[in] poly_coeff_vf32
     * @return curvature at 'x_f32'.
     */
    F32  get_curvature( const F32 x_f32, const POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s);

    /**
     * @details This function returns the tangential angle at 'x_f32'.
     * @param[in] 'x_f32' is the range to determine the tangential angle for.
     * @param[in] p_trajectory_s
     * @return tangential angle of 'p_trajectory_s' at 'x_f32'.
     */
    F32  get_tangential_angle(const POLYNOMIAL_TRAJECTORY* const p_trajectory_s, const F32 x_f32);

    /**
     * @details This function sets all parameters of a polynomial trajectory
     * @param[out] p_poly_coeff_s
     * @param[in]  in_lateral_offset_f32
     * @param[in]  in_tangential_angle_f32
     * @param[in]  in_curvature_f32
     * @param[in]  in_curvature_rate_f32
     */
    void set_polynomial_trajectory(POLYNOMIAL_TRAJECTORY* const p_poly_coeff_s,
                                   const F32 in_lateral_offset_f32,
                                   const F32 in_tangential_angle_f32,
                                   const F32 in_curvature_f32,
                                   const F32 in_curvature_rate_f32);

    /**
     * @details This function determines an approximation of the distance between a polynomial and the origin (0,0).
     *          Approximation has a small error for small c2 and c3 coefficients.
     * @param[in]  p_polynomial_s input polynomial
     * @return     distance to origin [m]
     */
    F32 get_polynomial_distance_to_origin(const POLYNOMIAL_TRAJECTORY* p_polynomial_s);

#endif /* POLYNOMIAL_LINE_H */
