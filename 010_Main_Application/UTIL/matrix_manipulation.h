/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: matrix_manipulation.h $
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
 * @file matrix_manipulation.h
 * @brief This file summarizes functions to perform matrix manipulations.
 *        Note, that there are two different realizations for matrices: one for symmetric matrices
 *        (e.g., covariance matrices) and one for "normal" matrices.
 *        We use INVALID_NUMBER instead of NAN, because the cross-compiler has problems with NAN.
 *        Note, that then NAN is mentioned in comments below, INVALID_NUMBER is meant.
**/

#ifndef MATRIX_MANIPULATION_H
#define MATRIX_MANIPULATION_H

#include "../UTIL/matrix_indices.h"

    /*************************************************************/
    /*   FUNCTION DECLARATIONS                                   */
    /*************************************************************/

    /**
     *  @details fills a 3x3 Matrix
     *  @param[in] 'matrix_vf32' matrix to fill, array of length 9
     *  @param[in] 'sij_f32' value for: matrix_vf32[i,j]
     */
    void fill3x3Matrix(F32 matrix_vf32[], F32 s11_f32, F32 s12_f32, F32 s13_f32,
                                          F32 s21_f32, F32 s22_f32, F32 s23_f32,
                                          F32 s31_f32, F32 s32_f32, F32 s33_f32);

    /**
     * @details
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'scalar1_vf32'
     * @param[in] 'matrix1_vf32'
     * @param[in] 'scalar2_f32'
     * @param[in] 'matrix2_vf32'
     * @param[in] 'dim1_u8'
     * @param[in] 'dim2_u8'
     */
    void MatrixAddition( F32* const outputMatrix_vf32, F32 scalar1_f32, const F32* const matrix1_vf32, F32 scalar2_f32, const F32* const matrix2_vf32,
                          const U8 dim1_u8, const U8 dim2_u8 );

    /**
     * @details
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'scalar1_vf32'
     * @param[in] 'matrix1_vf32'
     * @param[in] 'scalar2_f32'
     * @param[in] 'matrix2_vf32'
     * @param[in] 'dim_u8'
     */
    void SymMatrixAddition( F32* const outputMatrix_vf32, F32 scalar1_f32, const F32* const matrix1_vf32, F32 scalar2_f32, const F32* const matrix2_vf32,
                             const U8 dim_u8 );
    /**
     * @details
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'scalar_f32'
     * @param[in] 'matrix_vf32'
     * @param[in] 'dim1_u8'
     * @param[in] 'dim2_u8'
     */
    void scalarMatrixMult( F32* const outputMatrix_vf32, F32 scalar_f32, const F32* const matrix_vf32,
                            const U8 dim1_u8, const U8 dim2_u8 );

    /**
     * @details
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'scalar_f32'
     * @param[in] 'matrix1_vf32'
     * @param[in] 'dim_square_u8'
     */
    void scalarSymMatrixMult( F32* const outputMatrix_vf32, F32 scalar_f32, const F32* const matrix1_vf32,
                               const U8 dim_square_u8 );

    /**
     * @details
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'matrix1_vf32'
     * @param[in] 'matrix2_vf32'
     * @param[in] 'dim11_u8'
     * @param[in] 'dim12_u8'
     * @param[in] 'dim22_u8'
     */
    void matrixMatrixMult( F32* const outputMatrix_vf32, const F32* const matrix1_vf32, const F32* const matrix2_vf32,
                            const U8 dim11_u8, const U8 dim12_u8, const U8 dim22_u8 );

    /**
     * @details
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'matrix1_vf32'
     * @param[in] 'matrix2_vf32'
     * @param[in] 'dim11_u8'
     * @param[in] 'dim22_u8'
     */
    void symMatrixMatrixMult( F32* const outputMatrix_vf32, const F32* const matrix1Sym_vf32, const F32* const matrix2_vf32,
                              const U8 dim11_u8, const U8 dim22_u8 );

    /**
     * @details
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'matrix1_vf32'
     * @param[in] 'matrix2_vf32'
     * @param[in] 'dim11_u8'
     * @param[in] 'dim12_u8'
     * @param[in] 'dim22_u8'
     */
    void matrixSymMatrixMult( F32* const outputMatrix_vf32, const F32* const matrix1_vf32, const F32* const matrix2_vf32,
                               const U8 dim11_u8, const U8 dim12_u8, const U8 dim22_u8 );


    /**
     * @details This function computes the covariance Matrix out of a given set of samples.
     * For an example for the used algorithm, see
     * http://de.wikipedia.org/wiki/Mehrdimensionale_Normalverteilung#Stichproben_bei_multivariaten_Verteilungen
     * INVALID_NUMBER values are ignored in computation
     *
     * @param[out] 'outputMatrix_vf32' The covariance matrix, which is computed using the samples
     *                             outputMatrix_vf32 is a symmetric matrix of dimension (dim_data x dim_data)
     *                             Attention: be sure to create outputMatrix_vf32 as a symmetric matrix using
     *                             NR_ELEMENTS_SYM and use the "sym-functions" for further matrix manipulation
     * @param[in] 'samples_vf32'   Matrix containing the samples for the covariance matrix computation.
     *                             Every column stands for a different dimension of the covariance matrix.
     *                             In a row, we find all the samples for the according dimension.
     *                             samples is a non-symmetric matrix of dimension (dim_data x nr_samples)
     *                             Attention: alters the samples by centering them around their mean.
     * @param[in] 'dim_data_u8'
     * @param[in] 'nr_samples_u16'
     */
    void cov_matrix(F32* const outputMatrix_vf32, F32* const samples_vf32, U8 dim_data_u8, U16 nr_samples_u16);

    /**
     * @details: This function checks the validity of calculated covariance matrix.
     *           In this function only the diagonal elements of the matrix are checked to be a positive value.
     *           Note: The matrix must have a positive semi-definite property, which is not being checked because of high computational costs.
     * @param[in]       covariance_matrix_vf32 is either process/state covariance matrix with dimension
     *                  (upper triangular matrix with a size of NR_ELEMENTS_SYM(STATE_DIMENSION_MAX))
     * @param[in]       dim_u8 is the dimension'n' of a n*n matrix
     * @param[in]       cov_valid_thres_f32 is the threshold value for variance to be a valid
     * @return          is_covariance_valid_b is the boolean to show whether Covariance matrix is valid or not
     */
    BOOLEAN isCovarianceMatrixValid( const F32* const covariance_matrix_vf32, const U8 dim_u8, const F32 cov_valid_thres_f32 );

    /**
     * @details This function computes the matrix1_vf32 * matrix2_vf32 * matrix1_vf32.
     * @param[out] 'outputMatrix_vf32Sym' is pointer to first element of symmetric matrix.
     * @param[in] 'matrix1_vf32' is pointer to first element of general matrix.
     * @param[in] 'matrix2Sym_vf32' is pointer to first element of symmetric matrix.
     * @param[in] 'dim11_u8'
     * @param[in] 'dim12_u8'
     */
    void matrixQuadraticForm( F32* const outputMatrixSym_vf32, const F32* const matrix1_vf32, const F32* const matrix2Sym_vf32,
                               const U8 dim11_u8, const U8 dim12_u8 );

    /**
     * @details This function computes the 'column vector * row vector'.
     * @param[out] 'outputMatrix_vf32Sym_vf32' is pointer to first element of symmetric matrix.
     * @param[in] 'vector1_vf32' is pointer to first element of vector.
     * @param[in] 'dim1_u8' vector1 has dimension dim1_u8.
     *
     */
    void VectorTensorProduct( F32* const outputMatrixSym_vf32, const F32* const vector1_vf32,
                                const U8 dim1_u8 );

    /**
     * @details This function computes the inverse for the given matrix.
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'matrix_vf32'
     * @param[in] 'dim_u8'
     */
    void Inverse( F32* const outputMatrix_vf32, const F32* const matrix_vf32,
                   const U8 dim_u8 );

    /**
     * @details
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'matrix_vf32'
     * @param[in] 'dim_u8'
     */
    void SymInverse( F32* const outputMatrix_vf32, const F32* const matrix_vf32,
                      const U8 dim_u8 );

    /**
     * @details This function returns the determinant for matrices up to dimension 3.
     * @param[in] 'array_vf32'
     * @param[in] 'dim_u8'
     * @return determinant for the given matrix.
     */
    F32 Determinant( const F32* const array_vf32, const U8 dim_u8 );

    /**
     * @details This function returns the determinant for symmetric matrices up to dimension 3.
     * @param[in] 'array_vf32'
     * @param[in] 'dim_u8'
     * @return determinant for the given matrix.
     */
    /*  determinant for symmetric matrices up to dimension 3 */
    F32 SymDeterminant( const F32* const array_vf32, const U8 dim_u8 );

    /**
     * @details
     * @param[out] 'outputMatrix_vf32'
     * @param[in] 'matrix_vf32'
     * @param[in] 'dim1_u8'
     * @param[in] 'dim2_u8'
     */
    void Transposition( F32* const outputMatrix_vf32, const F32* const matrix_vf32,
                          const U8 dim1_u8, const U8 dim2_u8 );

    /**
     * @details
     * @param[out] 'identityMatrix_vf32'
     * @param[in] 'dim_square_u8'
     */
    void SetSquareIdentityMatrix( F32* const identityMatrix_vf32, const U8 dim_square_u8 );

#endif /* MATRIX_MANIPULATION_H */
