
/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: matrix_manipulation.c $
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
 * @file matrix_manipulation.c
 * @brief This file summarizes functions to perform matrix manipulations.
 *        Note, that there are two different realizations for matrices: one for symmetric matrices
 *        (e.g., covariance matrices) and one for "normal" matrices.
 *        We use INVALID_NUMBER instead of NAN, because the cross-compiler has problems with NAN.
 *        Note, that then NAN is mentioned in comments below, INVALID_NUMBER is meant.
**/

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

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <assert.h>

#include <string.h>
#include <stdlib.h>
#include <mathf.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../UTIL/matrix_manipulation.h"
#include "../UTIL/vector_manipulation.h"


/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

/*
  DEVIATION:     4700
  DESCRIPTION:   Metric value out of threshold range: STPAR = 10.
  JUSTIFICATION: Amount of parameters is necessary here. Parameters are clearly-arranged, so the amount is uncritical.
*/
/* PRQA S 4700  8 */
void fill3x3Matrix(F32 matrix_vf32[], F32 s11_f32, F32 s12_f32, F32 s13_f32,
                                      F32 s21_f32, F32 s22_f32, F32 s23_f32,
                                      F32 s31_f32, F32 s32_f32, F32 s33_f32)
{
    matrix_vf32[0] = s11_f32;  matrix_vf32[1] = s12_f32;  matrix_vf32[2] = s13_f32;
    matrix_vf32[3] = s21_f32;  matrix_vf32[4] = s22_f32;  matrix_vf32[5] = s23_f32;
    matrix_vf32[6] = s31_f32;  matrix_vf32[7] = s32_f32;  matrix_vf32[8] = s33_f32;
}

void MatrixAddition( F32* const outputMatrix_vf32, F32 scalar1_f32, const F32* const matrix1_vf32, F32 scalar2_f32, const F32* const matrix2_vf32,
                     const U8 dim1_u8, const U8 dim2_u8 )
{
    U8 iIndex_u8;
    U8 jIndex_u8;

    if( (NULL != outputMatrix_vf32)
        && (NULL != matrix1_vf32)
        && (NULL != matrix2_vf32) )
    {
        for ( iIndex_u8 = 0; iIndex_u8 < dim1_u8; iIndex_u8++ )
        {
            for ( jIndex_u8 = 0; jIndex_u8 < dim2_u8; jIndex_u8++ )
            {
                outputMatrix_vf32[ GEN_IDX( iIndex_u8, jIndex_u8, dim2_u8 ) ] = (scalar1_f32 * matrix1_vf32[ GEN_IDX( iIndex_u8, jIndex_u8, dim2_u8 ) ]) + (scalar2_f32 * matrix2_vf32[ GEN_IDX( iIndex_u8, jIndex_u8, dim2_u8 ) ]);
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}


void SymMatrixAddition( F32* const outputMatrix_vf32, F32 scalar1_f32, const F32* const matrix1_vf32, F32 scalar2_f32, const F32* const matrix2_vf32,
                        const U8 dim_u8 )
{
    U8 iIndex_u8;

    if( (NULL != outputMatrix_vf32)
        && (NULL != matrix1_vf32)
        && (NULL != matrix2_vf32) )
    {
        for ( iIndex_u8 = 0; iIndex_u8 < NR_ELEMENTS_SYM(dim_u8); iIndex_u8++ )
        {
            outputMatrix_vf32[ iIndex_u8 ] = ( scalar1_f32 * matrix1_vf32[ iIndex_u8 ] ) + ( scalar2_f32 * matrix2_vf32[ iIndex_u8 ] );
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}


void scalarMatrixMult( F32* const outputMatrix_vf32, F32 scalar_f32, const F32* const matrix_vf32,
                       const U8 dim1_u8, const U8 dim2_u8 )
{
    U8 iIndex_u8;
    U8 jIndex_u8;

    if( (NULL != outputMatrix_vf32)
        && (NULL != matrix_vf32) )
    {
        for ( iIndex_u8 = 0; iIndex_u8 < dim1_u8; iIndex_u8++ )
        {
            for ( jIndex_u8 = 0; jIndex_u8 < dim2_u8; jIndex_u8++ )
            {
                outputMatrix_vf32[ GEN_IDX( iIndex_u8, jIndex_u8, dim2_u8 ) ] = scalar_f32 * matrix_vf32[ GEN_IDX( iIndex_u8, jIndex_u8, dim2_u8 ) ];
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}


void scalarSymMatrixMult( F32* const outputMatrix_vf32, F32 scalar_f32, const F32* const matrix1_vf32,
                          const U8 dim_square_u8 )
{
    U8 iIndex_u8;

    if( (NULL != outputMatrix_vf32)
        && (NULL != matrix1_vf32) )
    {
        for ( iIndex_u8 = 0; iIndex_u8 < NR_ELEMENTS_SYM(dim_square_u8); iIndex_u8++ )
        {
            outputMatrix_vf32[ iIndex_u8 ] = scalar_f32 * matrix1_vf32[ iIndex_u8 ];
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}


void matrixMatrixMult( F32* const outputMatrix_vf32, const F32* const matrix1_vf32, const F32* const matrix2_vf32,
                       const U8 dim11_u8, const U8 dim12_u8, const U8 dim22_u8 )
{
    U8 iIndex_u8;
    U8 jIndex_u8;
    U8 kIndex_u8;

    if( (NULL != outputMatrix_vf32)
        && (NULL != matrix1_vf32)
        && (NULL != matrix2_vf32) )
    {
        (void)memset(outputMatrix_vf32, 0U, dim11_u8 * dim22_u8 * sizeof(F32) );

        /*  matrix matrix multiplication */
        for ( kIndex_u8 = 0; kIndex_u8 < dim22_u8; kIndex_u8++ )
        {
            for ( iIndex_u8 = 0; iIndex_u8 < dim11_u8; iIndex_u8++ )
            {
                for ( jIndex_u8 = 0; jIndex_u8 < dim12_u8; jIndex_u8++ )
                {
                    outputMatrix_vf32[ GEN_IDX(iIndex_u8,kIndex_u8,dim22_u8) ] = outputMatrix_vf32[ GEN_IDX(iIndex_u8,kIndex_u8,dim22_u8) ] +
                                                        ( matrix1_vf32[ GEN_IDX(iIndex_u8,jIndex_u8,dim12_u8) ] * matrix2_vf32[ GEN_IDX(jIndex_u8,kIndex_u8,dim22_u8) ] );

                }
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

    ASSERT(outputMatrix_vf32 != matrix1_vf32)
    ASSERT(outputMatrix_vf32 != matrix2_vf32)
}


void symMatrixMatrixMult( F32* const outputMatrix_vf32, const F32* const matrix1Sym_vf32, const F32* const matrix2_vf32,
                       const U8 dim11_u8, const U8 dim22_u8 )
{
    U8 iIndex_u8;
    U8 jIndex_u8;
    U8 kIndex_u8;

    if( (NULL != outputMatrix_vf32)
        && (NULL != matrix1Sym_vf32)
        && (NULL != matrix2_vf32) )
    {
        (void)memset(outputMatrix_vf32, 0U, dim11_u8 * dim22_u8 * sizeof(F32) );

        /*  matrix matrix multiplication */
        for ( kIndex_u8 = 0; kIndex_u8 < dim22_u8; kIndex_u8++ )
        {
            for ( iIndex_u8 = 0; iIndex_u8 < dim11_u8; iIndex_u8++ )
            {
                for ( jIndex_u8 = 0; jIndex_u8 < dim11_u8; jIndex_u8++ )
                {
                    outputMatrix_vf32[ GEN_IDX(iIndex_u8,kIndex_u8,dim22_u8) ] = outputMatrix_vf32[ GEN_IDX(iIndex_u8,kIndex_u8,dim22_u8) ] +
                                                                  (matrix1Sym_vf32[ SYM_IDX(iIndex_u8,jIndex_u8,dim11_u8) ] * matrix2_vf32[ GEN_IDX(jIndex_u8,kIndex_u8,dim22_u8) ]);

                }
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

    ASSERT(outputMatrix_vf32 != matrix1Sym_vf32)
    ASSERT(outputMatrix_vf32 != matrix2_vf32)
}


void matrixSymMatrixMult( F32* const outputMatrix_vf32, const F32* const matrix1_vf32, const F32* const matrix2_vf32,
                       const U8 dim11_u8, const U8 dim12_u8, const U8 dim22_u8 )
{
    U8 iIndex_u8;
    U8 jIndex_u8;
    U8 kIndex_u8;

    if( (NULL != outputMatrix_vf32)
        && (NULL != matrix1_vf32)
        && (NULL != matrix2_vf32) )
    {
        (void)memset(outputMatrix_vf32, 0U, dim11_u8 * dim22_u8 * sizeof(F32) );

        /*  matrix matrix multiplication */
        for ( kIndex_u8 = 0; kIndex_u8 < dim22_u8; kIndex_u8++ )
        {
            for ( iIndex_u8 = 0; iIndex_u8 < dim11_u8; iIndex_u8++ )
            {
                for ( jIndex_u8 = 0; jIndex_u8 < dim12_u8; jIndex_u8++ )
                {
                    outputMatrix_vf32[ GEN_IDX(iIndex_u8,kIndex_u8,dim22_u8) ] =   outputMatrix_vf32[ GEN_IDX(iIndex_u8,kIndex_u8,dim22_u8) ]
                                                                   + ( matrix1_vf32[ GEN_IDX(iIndex_u8,jIndex_u8,dim12_u8) ] * matrix2_vf32[ SYM_IDX(jIndex_u8,kIndex_u8,dim22_u8) ] );

                }
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

    ASSERT(outputMatrix_vf32 != matrix1_vf32)
    ASSERT(outputMatrix_vf32 != matrix2_vf32)
}


void cov_matrix(F32* const outputMatrix_vf32, F32* const samples_vf32, U8 dim_data_u8, U16 nr_samples_u16)
{

    F32 *row_i_vf32;
    F32 *row_j_vf32;
    U16 i_u16;
    U16 j_u16;

    if( (NULL != outputMatrix_vf32)
        && (NULL != samples_vf32) )
    {
        /*  now get centered samples matrix; */
        for ( i_u16 = 0; i_u16 < dim_data_u8; i_u16++ )
        {
            /*  pointer to the i-th row in the samples matrix (meaning, the samples belonging to dimension i_u16) */
            /*  note, that we flipped the matrix from the wikipedia example in order to be able to do so. */
            /*  attention: this makes use of the internal representation of a matrix (row order) */
            /*  if this canges, the algorithm has to be changed */
            row_i_vf32 = &samples_vf32[GEN_IDX(i_u16, 0, nr_samples_u16)];
            center_vector(row_i_vf32, nr_samples_u16);
        }

        /*  now determine the covariance values for every combination and store it in outputMatrix */
        /*  (upper triangle, as in matlab script) */
        for ( i_u16 = 0; i_u16 < dim_data_u8; i_u16++ )
        {
            for ( j_u16 = i_u16; j_u16 < dim_data_u8; j_u16++)
            {
                row_i_vf32 = &samples_vf32[GEN_IDX(i_u16, 0, nr_samples_u16)];
                row_j_vf32 = &samples_vf32[GEN_IDX(j_u16, 0, nr_samples_u16)];
                outputMatrix_vf32[SYM_IDX_ORDERED(i_u16, j_u16, dim_data_u8)] = cov_vec(row_i_vf32, row_j_vf32, nr_samples_u16);
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}

BOOLEAN isCovarianceMatrixValid( const F32* const covariance_matrix_vf32, const U8 dim_u8, const F32 cov_valid_thres_f32 )
{
    BOOLEAN is_covariance_valid_b           = TRUE;
    U8      index_u8;

    /* check for the threshold value for covariance validity to be correct */
    ASSERT( cov_valid_thres_f32 >= 0.0F )

    for( index_u8 = 0; index_u8 < dim_u8; index_u8++ )
    {
        /* check for all the diagonal elements of the covariance matrix to be positive */
        if ( covariance_matrix_vf32[SYM_IDX(index_u8, index_u8, dim_u8)] > cov_valid_thres_f32 )
        {
            /* do nothing as is_covariance_valid_b is already set to TRUE */
        }
        else
        {
            is_covariance_valid_b = FALSE;
            /* create ASSERT message if any of the diagonal element of the matrix is calculated negative or zero.*/
            ASSERT( covariance_matrix_vf32[SYM_IDX(index_u8, index_u8, dim_u8)] > cov_valid_thres_f32 )
            break;
        }
    }
    return is_covariance_valid_b;
}

void matrixQuadraticForm( F32* const outputMatrixSym_vf32, const F32* const matrix1_vf32, const F32* const matrix2Sym_vf32,
                           const U8 dim11_u8, const U8 dim12_u8 )
{
    U8 iIndex_u8;
    U8 jIndex_u8;
    U8 kIndex_u8;
    U8 lIndex_u8;
    U8 symIndex_u8;
    F32 aux_f32;
    F32 matrix1_ij_f32;
    F32 matrix1_kj_f32;

    if((NULL != outputMatrixSym_vf32)
        && (NULL != matrix1_vf32)
        && (NULL != matrix2Sym_vf32) )
    {
        /*  m1 * m2 * m1.' quadratic form */
        for ( iIndex_u8 = 0; iIndex_u8 < dim11_u8; iIndex_u8++ )
        {
            for ( kIndex_u8 = iIndex_u8; kIndex_u8 < dim11_u8; kIndex_u8++ )
            {
                /*  initialize auxiliary variable */
                aux_f32 = 0.0F;

                for ( jIndex_u8 = 0; jIndex_u8 < dim12_u8; jIndex_u8++ )
                {
                    /*  define elements to minimize matrix element access */
                    matrix1_ij_f32 = matrix1_vf32[ GEN_IDX( iIndex_u8 , jIndex_u8, dim12_u8 ) ];
                    matrix1_kj_f32 = matrix1_vf32[ GEN_IDX( kIndex_u8 , jIndex_u8, dim12_u8 ) ];

                    for ( lIndex_u8 = jIndex_u8; lIndex_u8 < dim12_u8; lIndex_u8++ )
                    {
                        symIndex_u8 = SYM_IDX_ORDERED(jIndex_u8, lIndex_u8, dim12_u8);

                        if ( lIndex_u8 == jIndex_u8 )
                        {
                            aux_f32 = aux_f32 + ( matrix1_ij_f32 * matrix2Sym_vf32[ symIndex_u8 ] * matrix1_vf32[ GEN_IDX( kIndex_u8 , lIndex_u8, dim12_u8 ) ] );
                        }
                        else
                        {
                            aux_f32 = aux_f32 + ( matrix2Sym_vf32[ symIndex_u8 ] * ( ( matrix1_ij_f32 * matrix1_vf32[ GEN_IDX( kIndex_u8 , lIndex_u8, dim12_u8 ) ] ) + ( matrix1_vf32[ GEN_IDX( iIndex_u8 , lIndex_u8, dim12_u8 ) ] * matrix1_kj_f32 ) ) );
                        }
                    }
                }

                outputMatrixSym_vf32[ SYM_IDX_ORDERED(iIndex_u8, kIndex_u8, dim11_u8) ] = aux_f32;
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

    ASSERT(outputMatrixSym_vf32 != matrix1_vf32)
    ASSERT(outputMatrixSym_vf32 != matrix2Sym_vf32)
}


void VectorTensorProduct( F32* const outputMatrixSym_vf32, const F32* const vector1_vf32,
                           const U8 dim1_u8 )
{
    U8 iIndex_u8;
    U8 kIndex_u8;

    if((NULL != outputMatrixSym_vf32)
        && (NULL != vector1_vf32) )
    {
        /*  vector tensor product */
        for ( iIndex_u8 = 0; iIndex_u8 < dim1_u8; iIndex_u8++ )
        {
            for ( kIndex_u8 = iIndex_u8; kIndex_u8 < dim1_u8; kIndex_u8++ )
            {
                outputMatrixSym_vf32[ SYM_IDX_ORDERED( iIndex_u8, kIndex_u8, dim1_u8 ) ] = vector1_vf32[ iIndex_u8 ] * vector1_vf32[ kIndex_u8 ];
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}


void Inverse( F32* const outputMatrix_vf32, const F32* const matrix_vf32,
        const U8 dim_u8 )
{
    F32 determinant_f32;
    F32 inv_determinant_f32;

    if((NULL != outputMatrix_vf32)
        && (NULL != matrix_vf32) )
    {
        /*  get determinant */
        determinant_f32 = Determinant( matrix_vf32, dim_u8 );

        if ( fabs( determinant_f32 ) < DIV_THRESHOLD )
        {
            /*  need to output error message */
        }
        else
        {
            inv_determinant_f32 = 1.0F / determinant_f32;
            switch ( dim_u8 )
            {
                case 1:
                {
                    outputMatrix_vf32[0] = 1.0F * inv_determinant_f32;
                    break;
                }
                case 2:
                {
                    outputMatrix_vf32[0] =  matrix_vf32[3] * inv_determinant_f32;
                    outputMatrix_vf32[1] = -matrix_vf32[1] * inv_determinant_f32;
                    outputMatrix_vf32[2] = -matrix_vf32[2] * inv_determinant_f32;
                    outputMatrix_vf32[3] =  matrix_vf32[0] * inv_determinant_f32;
                    break;
                }
                case 3:
                {
                    outputMatrix_vf32[0] =  ( ( matrix_vf32[4] * matrix_vf32[8] ) - ( matrix_vf32[5] * matrix_vf32[7] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[1] = -( ( matrix_vf32[1] * matrix_vf32[8] ) - ( matrix_vf32[2] * matrix_vf32[7] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[2] =  ( ( matrix_vf32[1] * matrix_vf32[5] ) - ( matrix_vf32[2] * matrix_vf32[4] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[3] = -( ( matrix_vf32[3] * matrix_vf32[8] ) - ( matrix_vf32[6] * matrix_vf32[5] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[4] =  ( ( matrix_vf32[0] * matrix_vf32[8] ) - ( matrix_vf32[6] * matrix_vf32[2] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[5] = -( ( matrix_vf32[0] * matrix_vf32[5] ) - ( matrix_vf32[3] * matrix_vf32[2] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[6] =  ( ( matrix_vf32[3] * matrix_vf32[7] ) - ( matrix_vf32[6] * matrix_vf32[4] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[7] = -( ( matrix_vf32[0] * matrix_vf32[7] ) - ( matrix_vf32[6] * matrix_vf32[1] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[8] =  ( ( matrix_vf32[0] * matrix_vf32[4] ) - ( matrix_vf32[3] * matrix_vf32[1] ) ) * inv_determinant_f32;
                    break;
                }
                default:
                {
                    /*  need to output error message */
                    ASSERT(FALSE)
                    break;
                }
            }

        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

    ASSERT(outputMatrix_vf32 != matrix_vf32)
}


void SymInverse( F32* const outputMatrix_vf32, const F32* const matrix_vf32,
        const U8 dim_u8 )
{
    F32 determinant_f32;
    F32 inv_determinant_f32;

    if((NULL != outputMatrix_vf32)
        && (NULL != matrix_vf32) )
    {
        /*  get determinant_f32 */
        determinant_f32 = SymDeterminant( matrix_vf32, dim_u8 );

        if ( fabsf( determinant_f32 ) < DIV_THRESHOLD )
        {
            /*  need to output error message */
        }
        else
        {
            inv_determinant_f32 = 1.0F / determinant_f32;
            switch ( dim_u8 )
            {
                case 1:
                {
                    outputMatrix_vf32[0] = 1.0F * inv_determinant_f32;
                    break;
                }
                case 2:
                {
                    outputMatrix_vf32[0] =  matrix_vf32[2] * inv_determinant_f32;
                    outputMatrix_vf32[1] = -matrix_vf32[1] * inv_determinant_f32;
                    outputMatrix_vf32[2] =  matrix_vf32[0] * inv_determinant_f32;
                    break;
                }
                case 3:
                {
                    outputMatrix_vf32[0] =  ( ( matrix_vf32[3] * matrix_vf32[5] ) - ( matrix_vf32[4] * matrix_vf32[4] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[1] = -( ( matrix_vf32[1] * matrix_vf32[5] ) - ( matrix_vf32[2] * matrix_vf32[4] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[2] =  ( ( matrix_vf32[1] * matrix_vf32[4] ) - ( matrix_vf32[2] * matrix_vf32[3] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[3] =  ( ( matrix_vf32[0] * matrix_vf32[5] ) - ( matrix_vf32[2] * matrix_vf32[2] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[4] = -( ( matrix_vf32[0] * matrix_vf32[4] ) - ( matrix_vf32[1] * matrix_vf32[2] ) ) * inv_determinant_f32;
                    outputMatrix_vf32[5] =  ( ( matrix_vf32[0] * matrix_vf32[3] ) - ( matrix_vf32[1] * matrix_vf32[1] ) ) * inv_determinant_f32;
                    break;
                }
                default:
                {
                    /*  need to output error message */
                    ASSERT(FALSE)
                    break;
                }
            }

        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

    ASSERT(outputMatrix_vf32 != matrix_vf32)
}


F32 Determinant( const F32* const array_vf32, const U8 dim_u8 )
{
    F32 determinant_f32;

    if(NULL != array_vf32)
    {
        switch ( dim_u8 )
        {
            case 1:
            {
                determinant_f32 = array_vf32[0];
                break;
            }
            case 2:
            {
                determinant_f32 =   ( array_vf32[0] * array_vf32[3] )
                                              - ( array_vf32[1] * array_vf32[2] );
                break;
            }
            case 3:
            {
                determinant_f32 =    ( array_vf32[0] * ( ( array_vf32[4] * array_vf32[8] ) - ( array_vf32[7] * array_vf32[5] ) ) )
                                              +  ( array_vf32[1] * ( ( array_vf32[5] * array_vf32[6] ) - ( array_vf32[3] * array_vf32[8] ) ) )
                                              +  ( array_vf32[2] * ( ( array_vf32[3] * array_vf32[7] ) - ( array_vf32[4] * array_vf32[6] ) ) );
                break;
            }
            default:
            {
                /*  need to output error message */
                determinant_f32 = 0.0F;
                ASSERT(FALSE)
                break;
            }
        }
    }
    else
    {
        determinant_f32 = 0.0F;

        ASSERT(FALSE)
    }

    return determinant_f32;
}


F32 SymDeterminant( const F32* const array_vf32, const U8 dim_u8 )
{
    F32 determinant_f32;

#ifdef simulator
    F32 t2_f32, t3_f32, t4_f32, t5_f32, t6_f32, t7_f32, t8_f32, t9_f32, t10_f32, t11_f32;
#endif

    if(NULL != array_vf32)
    {
        switch ( dim_u8 )
        {
            case 1:
            {
                determinant_f32 = array_vf32[0];
                break;
            }
            case 2:
            {
                determinant_f32 = ( array_vf32[0] * array_vf32[2] ) - ( array_vf32[1] * array_vf32[1] );
                break;
            }
            case 3:
            {
                determinant_f32 =( array_vf32[0] * ( ( array_vf32[3] * array_vf32[5] ) - ( array_vf32[4] * array_vf32[4] ) ) )
                              +  ( array_vf32[1] * ( ( array_vf32[4] * array_vf32[2] ) - ( array_vf32[5] * array_vf32[1] ) ) )
                              +  ( array_vf32[2] * ( ( array_vf32[1] * array_vf32[4] ) - ( array_vf32[2] * array_vf32[3] ) ) );
                break;
            }
#ifdef simulator
            case 4:
            {
                /*
                 * auto-generated from matlab-code:
                 * syms a_0 a_1 a_2 a_3 a_4 a_5 a_6 a_7 a_8 a_9
                 * A4 = [a_0,a_1,a_2,a_3;a_1,a_4,a_5,a_6;a_2,a_5,a_7,a_8;a_3,a_6,a_8,a_9]
                 * ccode(det(A4),'file','C:\test.c')
                 *
                 * post-processing (to array indexing and to match coding guidelines):
                 * replace 'a_(\d+)' -> 'array_vf32[\1]'
                 * replace '\+' -> '\n\t\t+'
                 * replace '-' -> '\n\t\t-'
                 * replace '2\.0' -> '2.0F'
                 * replace '\+(.*)$' -> '+ (  \1 )'
                 * replace '-(.*)$' -> '+ ( -\1 )'
                 * replace '*' -> ' * '
                 * replace 't0_f32' -> 'determinant_f32'
                 * replace 't\d+' -> '\0_f32'
                 * slight rework (first and last line)
                 */
                t2_f32 = array_vf32[8] * array_vf32[8];
                t3_f32 = array_vf32[6] * array_vf32[6];
                t4_f32 = array_vf32[5] * array_vf32[5];
                t5_f32 = array_vf32[3] * array_vf32[3];
                t6_f32 = array_vf32[2] * array_vf32[2];
                t7_f32 = array_vf32[1] * array_vf32[1];
                determinant_f32 = (t2_f32 * t7_f32)
                      + (  t3_f32 * t6_f32 )
                      + (  t4_f32 * t5_f32 )
                      + ( -array_vf32[0] * array_vf32[4] * t2_f32 )
                      + ( -array_vf32[0] * array_vf32[7] * t3_f32 )
                      + ( -array_vf32[0] * array_vf32[9] * t4_f32 )
                      + ( -array_vf32[4] * array_vf32[7] * t5_f32 )
                      + ( -array_vf32[4] * array_vf32[9] * t6_f32 )
                      + ( -array_vf32[7] * array_vf32[9] * t7_f32 )
                      + ( -array_vf32[2] * array_vf32[3] * array_vf32[5] * array_vf32[6] * 2.0F )
                      + (  array_vf32[1] * array_vf32[2] * array_vf32[5] * array_vf32[9] * 2.0F )
                      + ( -array_vf32[1] * array_vf32[2] * array_vf32[6] * array_vf32[8] * 2.0F )
                      + ( -array_vf32[1] * array_vf32[3] * array_vf32[5] * array_vf32[8] * 2.0F )
                      + (  array_vf32[1] * array_vf32[3] * array_vf32[6] * array_vf32[7] * 2.0F )
                      + (  array_vf32[2] * array_vf32[3] * array_vf32[4] * array_vf32[8] * 2.0F )
                      + (  array_vf32[0] * array_vf32[5] * array_vf32[6] * array_vf32[8] * 2.0F )
                      + (  array_vf32[0] * array_vf32[4] * array_vf32[7] * array_vf32[9] );
                break;
            }
            case 5:
            {
                /*
                 * auto-generated from matlab-code:
                 * syms a_0 a_1 a_2 a_3 a_4 a_5 a_6 a_7 a_8 a_9 a_10 a_11 a_12 a_13 a_14
                 * A5 = [a_0,a_1,a_2,a_3,a_4;a_1,a_5,a_6,a_7,a_8;a_2,a_6,a_9,a_10,a_11;a_3,a_7,a_10,a_12,a_13;a_4,a_8,a_11,a_13,a_14]
                 * ccode(det(A5),'file','C:\test.c')
                 *
                 * post-processing (to array indexing and to match coding guidelines):
                 * replace 'a_(\d+)' -> 'array_vf32[\1]'
                 * replace '\+' -> '\n\t\t+'
                 * replace '-' -> '\n\t\t-'
                 * replace '2\.0' -> '2.0F'
                 * replace '\+(.*)$' -> '+ (  \1 )'
                 * replace '-(.*)$' -> '+ ( -\1 )'
                 * replace '*' -> ' * '
                 * replace 't0_f32' -> 'determinant_f32'
                 * replace 't\d+' -> '\0_f32'
                 * slight rework (first and last line)
                 */
                t2_f32 = array_vf32[11] * array_vf32[11];
                t3_f32 = array_vf32[10] * array_vf32[10];
                t4_f32 = array_vf32[13] * array_vf32[13];
                t5_f32 = array_vf32[3]  * array_vf32[3];
                t6_f32 = array_vf32[8]  * array_vf32[8];
                t7_f32 = array_vf32[4]  * array_vf32[4];
                t8_f32 = array_vf32[7]  * array_vf32[7];
                t9_f32 = array_vf32[2]  * array_vf32[2];
                t10_f32 = array_vf32[6] * array_vf32[6];
                t11_f32 = array_vf32[1] * array_vf32[1];
                determinant_f32 =   (  array_vf32[0]  * t3_f32 * t6_f32 )
                                  + (  array_vf32[0]  * t2_f32 * t8_f32 )
                                  + (  array_vf32[5]  * t2_f32 * t5_f32 )
                                  + (  array_vf32[0]  * t4_f32 * t10_f32 )
                                  + (  array_vf32[5]  * t3_f32 * t7_f32 )
                                  + (  array_vf32[5]  * t4_f32 * t9_f32 )
                                  + (  array_vf32[9]  * t5_f32 * t6_f32 )
                                  + (  array_vf32[9]  * t4_f32 * t11_f32 )
                                  + (  array_vf32[9]  * t7_f32 * t8_f32 )
                                  + (  array_vf32[12] * t2_f32 * t11_f32 )
                                  + (  array_vf32[12] * t6_f32 * t9_f32 )
                                  + (  array_vf32[14] * t3_f32 * t11_f32 )
                                  + (  array_vf32[12] * t7_f32 * t10_f32 )
                                  + (  array_vf32[14] * t5_f32 * t10_f32 )
                                  + (  array_vf32[14] * t8_f32 * t9_f32 )
                                  + ( -array_vf32[1]  * array_vf32[2]  * array_vf32[6]  * t4_f32  * 2.0F )
                                  + ( -array_vf32[1]  * array_vf32[3]  * array_vf32[7]  * t2_f32  * 2.0F )
                                  + ( -array_vf32[1]  * array_vf32[4]  * array_vf32[8]  * t3_f32  * 2.0F )
                                  + ( -array_vf32[0]  * array_vf32[5]  * array_vf32[9]  * t4_f32 )
                                  + ( -array_vf32[0]  * array_vf32[5]  * array_vf32[12] * t2_f32 )
                                  + ( -array_vf32[2]  * array_vf32[3]  * array_vf32[10] * t6_f32  * 2.0F )
                                  + ( -array_vf32[0]  * array_vf32[5]  * array_vf32[14] * t3_f32 )
                                  + ( -array_vf32[2]  * array_vf32[4]  * array_vf32[11] * t8_f32  * 2.0F )
                                  + ( -array_vf32[0]  * array_vf32[9]  * array_vf32[12] * t6_f32 )
                                  + ( -array_vf32[3]  * array_vf32[4]  * array_vf32[13] * t10_f32 * 2.0F )
                                  + ( -array_vf32[6]  * array_vf32[7]  * array_vf32[10] * t7_f32  * 2.0F )
                                  + ( -array_vf32[6]  * array_vf32[8]  * array_vf32[11] * t5_f32  * 2.0F )
                                  + ( -array_vf32[0]  * array_vf32[9]  * array_vf32[14] * t8_f32 )
                                  + ( -array_vf32[5]  * array_vf32[9]  * array_vf32[12] * t7_f32 )
                                  + ( -array_vf32[5]  * array_vf32[9]  * array_vf32[14] * t5_f32 )
                                  + ( -array_vf32[0]  * array_vf32[12] * array_vf32[14] * t10_f32 )
                                  + ( -array_vf32[7]  * array_vf32[8]  * array_vf32[13] * t9_f32 * 2.0F )
                                  + ( -array_vf32[5]  * array_vf32[12] * array_vf32[14] * t9_f32 )
                                  + ( -array_vf32[10] * array_vf32[11] * array_vf32[13] * t11_f32 * 2.0F )
                                  + ( -array_vf32[9]  * array_vf32[12] * array_vf32[14] * t11_f32 )
                                  + (  array_vf32[2]  * array_vf32[3]  * array_vf32[7]  * array_vf32[8]  * array_vf32[11] * 2.0F )
                                  + (  array_vf32[2]  * array_vf32[4]  * array_vf32[7]  * array_vf32[8]  * array_vf32[10] * 2.0F )
                                  + (  array_vf32[3]  * array_vf32[4]  * array_vf32[6]  * array_vf32[7]  * array_vf32[11] * 2.0F )
                                  + (  array_vf32[3]  * array_vf32[4]  * array_vf32[6]  * array_vf32[8]  * array_vf32[10] * 2.0F )
                                  + ( -array_vf32[3]  * array_vf32[4]  * array_vf32[7]  * array_vf32[8]  * array_vf32[9]  * 2.0F )
                                  + ( -array_vf32[2]  * array_vf32[3]  * array_vf32[6]  * array_vf32[7]  * array_vf32[14] * 2.0F )
                                  + (  array_vf32[2]  * array_vf32[3]  * array_vf32[6]  * array_vf32[8]  * array_vf32[13] * 2.0F )
                                  + (  array_vf32[2]  * array_vf32[4]  * array_vf32[6]  * array_vf32[7]  * array_vf32[13] * 2.0F )
                                  + ( -array_vf32[2]  * array_vf32[4]  * array_vf32[6]  * array_vf32[8]  * array_vf32[12] * 2.0F )
                                  + (  array_vf32[1]  * array_vf32[3]  * array_vf32[8]  * array_vf32[10] * array_vf32[11] * 2.0F )
                                  + (  array_vf32[1]  * array_vf32[4]  * array_vf32[7]  * array_vf32[10] * array_vf32[11] * 2.0F )
                                  + ( -array_vf32[3]  * array_vf32[4]  * array_vf32[5]  * array_vf32[10] * array_vf32[11] * 2.0F )
                                  + ( -array_vf32[1]  * array_vf32[2]  * array_vf32[7]  * array_vf32[10] * array_vf32[14] * 2.0F )
                                  + (  array_vf32[1]  * array_vf32[2]  * array_vf32[7]  * array_vf32[11] * array_vf32[13] * 2.0F )
                                  + (  array_vf32[1]  * array_vf32[2]  * array_vf32[8]  * array_vf32[10] * array_vf32[13] * 2.0F )
                                  + ( -array_vf32[1]  * array_vf32[2]  * array_vf32[8]  * array_vf32[11] * array_vf32[12] * 2.0F )
                                  + ( -array_vf32[1]  * array_vf32[3]  * array_vf32[6]  * array_vf32[10] * array_vf32[14] * 2.0F )
                                  + (  array_vf32[1]  * array_vf32[3]  * array_vf32[6]  * array_vf32[11] * array_vf32[13] * 2.0F )
                                  + (  array_vf32[1]  * array_vf32[3]  * array_vf32[7]  * array_vf32[9]  * array_vf32[14] * 2.0F )
                                  + ( -array_vf32[1]  * array_vf32[3]  * array_vf32[8]  * array_vf32[9]  * array_vf32[13] * 2.0F )
                                  + (  array_vf32[1]  * array_vf32[4]  * array_vf32[6]  * array_vf32[10] * array_vf32[13] * 2.0F )
                                  + ( -array_vf32[1]  * array_vf32[4]  * array_vf32[6]  * array_vf32[11] * array_vf32[12] * 2.0F )
                                  + ( -array_vf32[1]  * array_vf32[4]  * array_vf32[7]  * array_vf32[9]  * array_vf32[13] * 2.0F )
                                  + (  array_vf32[1]  * array_vf32[4]  * array_vf32[8]  * array_vf32[9]  * array_vf32[12] * 2.0F )
                                  + (  array_vf32[2]  * array_vf32[3]  * array_vf32[5]  * array_vf32[10] * array_vf32[14] * 2.0F )
                                  + ( -array_vf32[2]  * array_vf32[3]  * array_vf32[5]  * array_vf32[11] * array_vf32[13] * 2.0F )
                                  + ( -array_vf32[2]  * array_vf32[4]  * array_vf32[5]  * array_vf32[10] * array_vf32[13] * 2.0F )
                                  + (  array_vf32[2]  * array_vf32[4]  * array_vf32[5]  * array_vf32[11] * array_vf32[12] * 2.0F )
                                  + (  array_vf32[3]  * array_vf32[4]  * array_vf32[5]  * array_vf32[9]  * array_vf32[13] * 2.0F )
                                  + (  array_vf32[1]  * array_vf32[2]  * array_vf32[6]  * array_vf32[12] * array_vf32[14] * 2.0F )
                                  + ( -array_vf32[0]  * array_vf32[7]  * array_vf32[8]  * array_vf32[10] * array_vf32[11] * 2.0F )
                                  + (  array_vf32[0]  * array_vf32[6]  * array_vf32[7]  * array_vf32[10] * array_vf32[14] * 2.0F )
                                  + ( -array_vf32[0]  * array_vf32[6]  * array_vf32[7]  * array_vf32[11] * array_vf32[13] * 2.0F )
                                  + ( -array_vf32[0]  * array_vf32[6]  * array_vf32[8]  * array_vf32[10] * array_vf32[13] * 2.0F )
                                  + (  array_vf32[0]  * array_vf32[6]  * array_vf32[8]  * array_vf32[11] * array_vf32[12] * 2.0F )
                                  + (  array_vf32[0]  * array_vf32[7]  * array_vf32[8]  * array_vf32[9]  * array_vf32[13] * 2.0F )
                                  + (  array_vf32[0]  * array_vf32[5]  * array_vf32[10] * array_vf32[11] * array_vf32[13] * 2.0F )
                                  + (  array_vf32[0]  * array_vf32[5]  * array_vf32[9]  * array_vf32[12] * array_vf32[14] );
                break;
            }
#endif
            default:
            {
                /*  need to output error message */
                determinant_f32 = 0.0F;
                ASSERT(FALSE)
                break;
            }
        }
    }
    else
    {
        determinant_f32 = 0.0F;

        ASSERT(NULL != array_vf32)
    }

    return determinant_f32;
}


void Transposition( F32* const outputMatrix_vf32, const F32* const matrix_vf32,
                    const U8 dim1_u8, const U8 dim2_u8 )
{
    U8 iIndex_u8;
    U8 jIndex_u8;

    if((NULL != outputMatrix_vf32)
        && (NULL != matrix_vf32) )
    {
        for ( iIndex_u8 = 0; iIndex_u8 < dim2_u8; iIndex_u8++ )
        {
            for ( jIndex_u8 = 0; jIndex_u8 < dim1_u8; jIndex_u8++ )
            {
                outputMatrix_vf32[ GEN_IDX( iIndex_u8, jIndex_u8, dim1_u8 ) ] = matrix_vf32[ GEN_IDX( jIndex_u8, iIndex_u8, dim2_u8 ) ];
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

    ASSERT(outputMatrix_vf32 != matrix_vf32)
}


void SetSquareIdentityMatrix( F32* const identityMatrix_vf32, const U8 dim_square_u8 )
{
    /* local variables */
    U8 index_diagonal_u8;

    if(NULL != identityMatrix_vf32)
    {
        /* preset to zero */
        (void) memset(identityMatrix_vf32, 0, dim_square_u8 * dim_square_u8 * sizeof( F32 ) );

        /* set the diagonal elements */
        for ( index_diagonal_u8 = 0; index_diagonal_u8 < dim_square_u8; index_diagonal_u8++ )
        {
            identityMatrix_vf32[ GEN_IDX( index_diagonal_u8, index_diagonal_u8, dim_square_u8 ) ] = 1.0F;
        }
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != identityMatrix_vf32)
    }
}


