/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: matrix_indices.h $
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

#ifndef MATRIX_INDICES_H_
#define MATRIX_INDICES_H_

/*************************************************************/
/*      MACROS                                               */
/*************************************************************/

/*  macro for accessing the elements of a covariance matrix that is stored
 *  as a vector containing independent entries only (row-wise, upper right triangle)
 *  perhaps change for accessing elements starting at 1 */
#define SYM_IDX(_IDX1,_IDX2,_DIM2)  ( ( ( MIN_IDX(_IDX1,_IDX2) * _DIM2 ) + MAX_IDX(_IDX1,_IDX2) ) - ( ( MIN_IDX(_IDX1,_IDX2) * ( MIN_IDX(_IDX1,_IDX2) + 1U ) ) / 2U ) )

/*
 *  like SYM_IDX(..), but indices must be ordered
 *  !! ATTENTION !! : SML_IDX <= GRT_IDX
 *  much faster, but more dangerous
 */
#define SYM_IDX_ORDERED(SML_IDX,GRT_IDX,_DIM2) ( ( ( (SML_IDX) * (_DIM2) ) + (GRT_IDX) ) - ( ( (SML_IDX)  * ( (SML_IDX) + 1U ) ) / 2U ) )

#define GEN_IDX(_IDX1,_IDX2,_DIM2) ( ( (_IDX1) * (_DIM2) ) + (_IDX2) )

#define MAX_IDX( A, B )                     ( ( (A) >= (B) ) ? (A) : (B) )
#define MIN_IDX( A, B )                     ( ( (A) <= (B) ) ? (A) : (B) )

#define NR_ELEMENTS(_DIM1, _DIM2) ((_DIM1) * (_DIM2))

/* For creating a symmetric matrix, use this macro to determine its size */
#define NR_ELEMENTS_SYM(_DIM1) (((_DIM1) * ((_DIM1) + 1U)) / 2U)


#endif /* MATRIX_INDICES_H_ */
