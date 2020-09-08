/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: vector_manipulation.h $
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
 * @file vector_manipulation.h
 * @brief This file summarizes functions to perform vector manipulations.
 *        We use INVALID_NUMBER instead of NAN, because the cross-compiler has problems with NAN.
 *        Note, that then NAN is mentioned in comments below, INVALID_NUMBER is meant.
 */
#ifndef VECTOR_MANIPULATION_H
#define VECTOR_MANIPULATION_H


    /*************************************************************/
    /*      DEFINES                                              */
    /*************************************************************/

    #define DIV_THRESHOLD 1e-6


    /*************************************************************/
    /*      MACROS                                               */
    /*************************************************************/

    #define EQUAL_FLOAT( X, Y, EPSILON ) ((EPSILON) > fabsf((X) - (Y)))

    #define SGN_f32(value_f32) (((value_f32) < 0.0F) ? -1.0F : 1.0F)

    /*************************************************************/
    /*   FUNCTION DECLARATIONS                                   */
    /*************************************************************/

    /**
     * @details This function ensures that [ reference-allowered_delta <= value <= reference+allowered_delta ].
     *
     * ***FUNCTION USED IN SIMULATOR*****
     */
    void assertFloat(F32 value_f32, F32 reference_f32, F32 allowered_delta_f32);

    /**
     * @details This function makes sure that exactly one bit is set in the bitfield.
     * useful to check whether or not e.g. exactly one movement state or exactly one lane assigned.
     *
     * ***FUNCTION USED IN SIMULATOR*****
     */
    void assertOneBitSet(U16 bitfield_u16);

    /**
     * @details This function computes the inner product of two vectors.
     * INVALID_NUMBER values are ignored in computation and p_nr_NaNs_u16 stores their quantity.
     * @param[in] 'a_vf32'
     * @param[in] 'b_vf32'
     * @param[in] 'vector_length_u16'
     * @param[out] 'p_nr_NaNs_u16'
     * @return the vector product.
     */
    F32 sprod(const F32 a_vf32[], const F32 b_vf32[], U16 vector_length_u16, U16* const p_nr_NaNs_u16);

    /**
     * @details This function removes the invalid values of the vector, and updates the length
     * to the length without them.
     * @param[in, out] 'a_vf32'
     * @param[in, out] 'p_vector_length_u16'
     */
    void remove_NaN(F32 a_vf32[], U16* const p_vector_length_u16);

    /**
     * @details This function linearly interpolates between the two points (x1, y1) and (x2, y1)
     * to get the y value for x.
     * x1 < x2 and values mustn't be NaN.
     * @param[in] 'x_f32'
     * @param[in] 'x1_f32'
     * @param[in] 'y1_f32'
     * @param[in] 'x2_f32'
     * @param[in] 'y2_f32'
     * @return
     */
    F32 lin_interpol(const F32 x_f32, const F32 x1_f32, const F32 y1_f32, const F32 x2_f32, const F32 y2_f32);

    /**
     * @details
     * linearly interpolates as 'lin_interpol()', however x2 does not need to be bigger than x1
     * @param[in] 'x_f32'
     * @param[in] 'x1_f32'
     * @param[in] 'y1_f32'
     * @param[in] 'x2_f32'
     * @param[in] 'y2_f32'
     * @param[in] 'is_constant_extrapolation_b'
     * @return
     */
    F32 linear_interpolation( const F32 x_f32, const F32 x1_f32, const F32 y1_f32, const F32 x2_f32, const F32 y2_f32, const BOOLEAN is_constant_extrapolation_b );

    /**
     * @details This function linearly interpolates the points x using x1 and y1 as input.
     * results are stored in y.
     * @param[out] 'y_vf32'
     * @param[in] 'x_vf32'
     * @param[in] 'length_x_u8'
     * @param[in] 'x1_vf32'
     * @param[in] 'y1_vf32'
     * @param[in] 'length_input_u8'
     */
    void lin_interpol_v(F32 y_vf32[], const F32 x_vf32[], const U8 length_x_u8, const F32 x1_vf32[], const F32 y1_vf32[], const U8 length_input_u8);

    /**
     * @details This function returns the median of the vector 'a_f32', exactly how MATLAB does it.
     * Attention: a mustn't contain NaNs and has to be sorted!
     * @param[in] 'a_vf32'
     * @param[in] 'vector_length_u16'
     */
    F32 median(const F32* const a_vf32, const U16 vector_length_u16);

    /**
     * @details This function retrieves the IQR in MATLAB style.
     * Attention: a mustn't contain NaNs and has to be sorted!
     * @param[in] 'a_vf32'
     * @param[in] 'vector_length_u16'
     */
    F32 iqr(const F32* const a_vf32, const U16 vector_length_u16);

    /**
     * @details This function calculates the prctile of element "index_u16" of a vector.
     * @param[in] 'vector_length_u16'
     * @param[in] 'index_u16'
     */
    F32 calc_prctile(const U16 vector_length_u16, const U16 index_u16);

    /**
     * @details This function determines the percentile "percentile" of 'a_vf32'. (example: median is percentile 50).
     * Attention: 'a_vf32' mustn't contain NaNs and has to be sorted! Use of remove_NaN and qsort is advised.
     * @param[in] 'a_vf32'
     * @param[in] 'vector_length_u16'
     * @param[in] 'percentile_f32'
     */
    F32 prctile(const F32 a_vf32[], const U16 vector_length_u16, F32 percentile_f32);

    /**
     * @details
     * @param[in] 'interval_a_start_f32'
     * @param[in] 'interval_a_end_f32'
     * @param[in] 'interval_b_start_f32'
     * @param[in] 'interval_b_end_f32'
     */
    F32 Overlap_Relative_To_Second_Interval( const F32 interval_a_start_f32, const F32 interval_a_end_f32,
                                             const F32 interval_b_start_f32, const F32 interval_b_end_f32 );

    /**
     * @details This function returns the min value in a vector.
     * @param[in] 'a_vf32'
     * @param[in] 'vector_length_u16'
     */
    F32 min_v(const F32 a_vf32[], U16 vector_length_u16);

    /**
     * @details This function returns the max value in a vector.
     * @param[in] 'a_vf32'
     * @param[in] 'vector_length_u16'
     */
    F32 max_v(const F32 a_vf32[], U16 vector_length_u16);

    /**
     * @details This function compute the mean of a given vector.
     * INVALID_NUMBER values are ignored in mean computation.
     * @param[out] 'a_vf32'
     * @param[in] 'vector_length_u16'
     */
     F32 mean(const F32 a_vf32[], U16 vector_length_u16);

    /**
     * @details This function returns the number of invalid elements in a vector.
     * @param[in] 'a_vf32'
     * @param[in] 'vector_length_u16'
     */
    U16 nr_invalid(const F32 a_vf32[], U16 vector_length_u16);

    /**
     * @details This function centres the vector 'a_vf32', meaning the mean of 'a_vf32'
     * is computed and subtracted of every element.
     * @param[in, out] 'a_vf32'
     * @param[in] 'vector_length_u16'
     */
    void center_vector(F32 a_vf32[], U16 vector_length_u16);

    /**
     * @details This function computes the covariance of two vectors a and b.
     * Attention: requires centers vectors as input. Use center_vector(..).
     * INVALID_NUMBER values are ignored in computation
     * @param[in] 'a_vf32'
     * @param[in] 'b_vf32'
     * @param[in] 'vector_length_u16'
     */
    F32 cov_vec(const F32* const a_vf32, const F32* const b_vf32, U16 vector_length_u16);

#endif /* VECTOR_MANIPULATION_H */
