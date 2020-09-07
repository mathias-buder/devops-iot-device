/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: filter_utility.h $
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
 * @file    filter_utility.h
 * @details utility functions to support filter algorithm.
 */
#ifndef FILTER_UTILITY_H
#define FILTER_UTILITY_H

/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/

/**
 * @details This function implements the Bayes update for a finite dimensional state
 * with dimension dim_u8 and a square measurement likelihood matrix.
 * @param[out] 'posterior_prob_vector_vf32'
 * @param[in] 'predicted_prob_vector_vf32'
 * @param[in] 'measurement_likelihood_matrix_vf32'
 * @param[in] 'meas_index_u8'
 * @param[in] 'dim_u8'
 * @return
 */
BOOLEAN discrete_Bayes_update( F32 posterior_prob_vector_vf32[], const F32 predicted_prob_vector_vf32[],
                               const F32 measurement_likelihood_matrix_vf32[], const U8 meas_index_u8, const U8 dim_u8 );

/**
 * @details This function calculates the probability of a position inside an interval.
 * @param[in] 'left_border_f32' is the left border of the given interval.
 * @param[in] 'right_border_f32' is the right border of the given interval.
 * @param[in] 'mean_f32' is the mean of the position.
 * @param[in] 'variance_f32' is the variance of the position.
 * @return the probability of the position inside the given interval.
 */
F32 calculate_probability_inside_interval( const F32 left_border_f32, const F32 right_border_f32, const F32 mean_f32, const F32 variance_f32 );

#endif /* FILTER_UTILITY_H */
