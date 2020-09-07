/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: least_squares.h $
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
#ifndef LEAST_SQUARES_H
#define LEAST_SQUARES_H

struct POLYNOMIAL_TRAJECTORY_TAG;

/**
 * @details Function definitions for least squares.
 * @param[out] 'linear_parameters_f32'
 * @param[in] 'x_values_f32'
 * @param[in] 'y_values_f32'
 * @param[in] 'weights_f32'
 * @param[in] 'value_length_u8'
 */
void linear_weighted_least_squares(       F32 linear_parameters_f32[],
                                    const F32 x_values_f32[],
                                    const F32 y_values_f32[],
                                    const F32 weights_f32[],
                                    const U8 value_length_u8);


/**
 * @detials Function definitions for least squares for parabola fitting.
 * @param[out] 'parabola_parameters_vf32'
 * @param[out] 'p_fitting_error_f32'
 * @param[in] 'x_vf32'
 * @param[in] 'y_vf32'
 * @param[in] 'data_length_u8'
 * @return
 */
BOOLEAN parabola_fitting_least_squares(       F32* const parabola_parameters_vf32,
                                              F32* const p_fitting_error_f32,
                                        const F32 x_vf32[],
                                        const F32 y_vf32[],
                                        const U8 data_length_u8 );

/**
 * @detials Function definitions for least squares for polynomial fitting.
 *          polynomial: y = c1*x  +  c2*x^2  +  c3*x^3
 * @param[out] 'p_polynomial_trajectory_s' resulting polynomial trajectory
 * @param[in] 'x_vf32'  vector of x values
 * @param[in] 'y_vf32'  vector of y values
 * @param[in] 'n_f32'   number of elements in x_vf32 and y_vf32
 * @return
 */
BOOLEAN polynomial_fitting_least_squares_c1c2c3( struct POLYNOMIAL_TRAJECTORY_TAG* const p_polynomial_trajectory_s,
                                                 const F32 x_vf32[],
                                                 const F32 y_vf32[],
                                                 const U8 n_f32 );
#endif
