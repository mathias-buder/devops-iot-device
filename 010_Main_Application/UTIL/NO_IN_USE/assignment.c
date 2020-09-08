
/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: assignment.c $
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


/*************************Copyright information from internet source

Copyright (c) 2004, Markus Buehren
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the distribution

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*********************************************************************/

/*************************************************************/
/*      FILE DEFINITIONS                                     */
/*************************************************************/

#define ASSIGNMENT            /*  defines this file */
#define UTIL_SUBSYSTEM        /*  defines the subsystem that this file resides in */

/*************************************************************/
/*      INCLUDE FILES                                        */
/*************************************************************/
#include <mathf.h>
#include <string.h>
#include <stdlib.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../TRACK/TRACK.h"
#include "../VIDEO/VIDEO.h"

#ifdef simulator
    #include <stdio.h>
    #include "switches.h"
    #include "compatib.h"
    #include <assert.h>
#endif

#include "../UTIL/assignment.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
#ifdef MUNKRES
/*  check for infinite values (e.g. for unreachable associations) */
#define CHECK_FOR_INF

typedef enum {
    MUNKRES_STEP2A,
    MUNKRES_STEP2B,
    MUNKRES_STEP3,
    MUNKRES_STEP4,
    MUNKRES_STEP5,
    MUNKRES_FINISHED,
    NR_MUNKRES_STEPS
} MUNKRES_STEPS;
#endif /* MUNKRES */

typedef struct DIST_MATRIX_MAPPING_STRUCT_TAG
{
    U16 dms_col_u16;
    U16 dms_row_u16;
    F32 dms_dist_f32;
} DIST_MATRIX_MAPPING_STRUCT;

/*  DEVIATION:     3218
    DESCRIPTION:   File scope static, 'g_dist_matrix_mapping_vs', is only accessed in one function.
    JUSTIFICATION: The local array of a function is moved to here due to stack size limit (4k).
*/
/* PRQA S 3218 1 */
static DIST_MATRIX_MAPPING_STRUCT g_dist_matrix_mapping_vs[MAX_ROWS_ASSIGNMENT * MAX_COLUMNS_ASSIGNMENT];

/*************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                        */
/*************************************************************/

#ifdef SUBOPTIMAL1
/****
Function definitions for suboptimal1 algorithm
****/

void assignmentsuboptimal1(S16* assignment, F32* cost, const F32* const distMatrixIn, U16 nOfRows, U16 nOfColumns);
#endif /* SUBOPTIMAL1 */

/***
 * @details Suboptimal algorithm for the assignment problem.
 *
 * @param[out]  assignment_result_vs16 The result vector.
 *              Length: number_of_rows_u16.
 *              assignment_result_vs16[i]=j means that the i-th row is assigned to the j-th column.
 *              assignment_result_vs16[i]=-1 means that the i-th row is not assigned.
 * @param[out] p_cost_f32 The accumulated cost for the found suboptimal solution. Unassigned elements will not contribute to this cost.
 * @param[in] dist_matrix_vf32 The distance matrix used for assignment.
 *            Dimension: number_of_rows_u16 x number_of_cols_u16.
 *            The value ASSIGNMENT_INVALID_DISTANCE means, that the given pair can not be assigned.
 * @param[in] number_of_rows_u16 The number of columns of dist_matrix_vf32.
 * @param[in] number_of_cols_u16 The number of rows of dist_matrix_vf32.
 */
PRIVATE void assignmentsuboptimal2( S16 assignment_result_vs16[],
        F32* const p_cost_f32,
        const F32 dist_matrix_vf32[],
        U16 number_of_rows_u16,
        U16 number_of_cols_u16 );

/**
 * @details
 * @param[in] 'p_a_s'
 * @param[in] 'p_b_s'
 * @return
 */
PRIVATE int dist_matrix_compare(const void* p_a_s, const void* p_b_s);

#ifdef MUNKRES
/**
 * @details This function defines the Munkres algorithm.
 * Note: The matrix order had to be switch from column to row order, because the internet source was
 * originally intended for MATLAB.
 *
 * @param[out] 'assignment'
 * @param[out] 'cost'
 * @param[in] 'distMatrixIn'
 * @param[in] 'nOfRows'
 * @param[in] 'nOfColumns'
 */
void assignmentoptimal(S16* assignment, F32* cost, const F32* const distMatrixIn, U16 nOfRows, U16 nOfColumns);

void buildassignmentvector(S16* assignment, BOOLEAN* starMatrix, U16 nOfRows, U16 nOfColumns);

/**
* ATTENTION: Here change from rows to columns because of distMatrixIn as argument!
**/
void computeassignmentcost(S16* assignment, F32* cost, const F32* const distMatrixIn, U16 nOfRows, U16 nOfColumns);

U8 step2a(BOOLEAN* starMatrix, BOOLEAN* coveredColumns, U16 nOfRows, U16 nOfColumns);

U8 step2b(BOOLEAN* coveredColumns, U16 nOfColumns, U16 minDim);

U8 step3(F32* distMatrix, BOOLEAN* starMatrix, BOOLEAN* primeMatrix, BOOLEAN* coveredColumns, BOOLEAN* coveredRows, U16 nOfRows, U16 nOfColumns, U16* const row_p, U16* const col_p);

U8 step4(BOOLEAN* starMatrix, BOOLEAN* newStarMatrix, BOOLEAN* primeMatrix, BOOLEAN* coveredRows, U16 nOfRows, U16 nOfColumns, U16 row, U16 col);

U8 step5(F32* distMatrix, BOOLEAN* coveredColumns, BOOLEAN* coveredRows, U16 nOfRows, U16 nOfColumns);
#endif /* MUNKRES */

/*************************************************************/
/*      CODE                                                 */
/*************************************************************/

void Compute_assignments( S16* const assignment_result_vs16,
                          F32* const p_cost_f32,
                          const F32* const dist_matrix_vf32,
                          const U16 number_of_rows_u16,
                          const U16 number_of_columns_u16,
                          ASSIGNMENT_TYPE association_method_e )
{
    if(   (NULL != assignment_result_vs16)
       && (NULL != p_cost_f32)
       && (NULL != dist_matrix_vf32) )
    {
        switch( association_method_e )
        {
    #ifdef MUNKRES
            case MUNKRES:
            {
                assignmentoptimal(assignment_result_vs16, p_cost_f32, dist_matrix_vf32, number_of_rows_u16, number_of_columns_u16);
                break;
            }
    #endif /* MUNKRES */
    #ifdef SUBOPTIMAL1
            case SUBOPTIMAL1:
            {
                assignmentsuboptimal1(assignment_result_vs16, p_cost_f32, dist_matrix_vf32, number_of_rows_u16, number_of_columns_u16);
                break;
            }
    #endif /* SUBOPTIMAL1 */
            case SUBOPTIMAL2:
            {
                assignmentsuboptimal2(assignment_result_vs16, p_cost_f32, dist_matrix_vf32, number_of_rows_u16, number_of_columns_u16);
                break;
            }
            default:
            {
                ASSERT(FALSE)
    #ifdef simulator
                printf("Error in file %s, line %d, association method not known", __FILE__, __LINE__);
    #endif
                break;
            }
        /* DEVIATION:     3352
           DESCRIPTION:   This 'switch' statement contains only two execution paths.
           JUSTIFICATION: Variant-safe implementation */
        /* PRQA S 3352 1*/
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}


#ifdef MUNKRES
/**********
Following: the implementation of munkres algorithm.
Note: The matrix order had to be switch from column to row order, because the internet source was
originally intended for matlab.
**********/

void assignmentoptimal(S16* assignment, F32* cost, const F32* const distMatrixIn, U16 nOfRows, U16 nOfColumns)
{
    F32 value, minValue;
    /* memory allocation */
    F32 distMatrix[ MAX_ROWS_ASSIGNMENT * MAX_COLUMNS_ASSIGNMENT ]; /*  working copy of distMatrixIn is set lateron */
    BOOLEAN coveredColumns[ MAX_COLUMNS_ASSIGNMENT ];
    BOOLEAN coveredRows[ MAX_ROWS_ASSIGNMENT ];
    BOOLEAN starMatrix[ MAX_ROWS_ASSIGNMENT * MAX_COLUMNS_ASSIGNMENT ];
    BOOLEAN primeMatrix[ MAX_ROWS_ASSIGNMENT * MAX_COLUMNS_ASSIGNMENT ];
    BOOLEAN newStarMatrix[ MAX_ROWS_ASSIGNMENT * MAX_COLUMNS_ASSIGNMENT ]; /* used in step4 */
    U16 nOfElements_16, minDim_u16, row_u16, index_u16;
    U16 col_u16 = 0U;
    U8 function_to_call;


#ifdef CHECK_FOR_INF
    BOOLEAN infiniteValueFound;
    F32 maxFiniteValue, infValue;
#endif


    /*  matrix initialization */
    (void)memset(distMatrix, 0U, sizeof distMatrix);
    (void)memset(coveredColumns, FALSE, sizeof coveredColumns);
    (void)memset(coveredRows, FALSE, sizeof coveredRows);
    (void)memset(starMatrix, FALSE, sizeof starMatrix);
    (void)memset(primeMatrix, FALSE, sizeof primeMatrix);
    (void)memset(newStarMatrix, FALSE, sizeof newStarMatrix);

    /* initialization */
    *cost = 0.0F;
    for (row_u16=0; row_u16<nOfRows; row_u16++)
    {
        assignment[row_u16] = NOT_ASSIGNED;
    }

    /* generate working copy of distance Matrix */
    /* check if all matrix elements are positive */
    nOfElements_16   = nOfRows * nOfColumns;

    for (row_u16=0; row_u16<nOfRows; row_u16++)
    {
        for (col_u16=0; col_u16<nOfColumns; col_u16++)
        {
            value = distMatrixIn[(row_u16*nOfColumns) + col_u16];
            /*  isfinite(x) and standard ASSIGNMENT_INVALID_DISTANCE don't work on this compiler, possible error source */
            /* if ((value != ASSIGNMENT_INVALID_DISTANCE) && (value < 0)) */
            /*     printf("Error in file %s, line %d, negative value in distance matrix", __FILE__, __LINE__); */
            distMatrix[row_u16 + (col_u16*nOfRows)] = value;
        }
    }


#ifdef CHECK_FOR_INF
    /* check for infinite values */
    maxFiniteValue     = -1.0F;
    infiniteValueFound = FALSE;

    for (index_u16 = 0U; index_u16<nOfElements_16; index_u16++)
    {
        value = distMatrix[index_u16];
        if (ASSIGNMENT_INVALID_DISTANCE > value)
        {
            if (value > maxFiniteValue)
            {
                maxFiniteValue = value;
            }
        }
        else
        {
            infiniteValueFound = TRUE;
        }
    }
    if (TRUE == infiniteValueFound)
    {
        if ( fabs(maxFiniteValue - -1.0F) < SMALL_NUMBER)
        {
            /* all elements are infinite */
            return;
        }


        /* set all infinite elements to big finite value */
        if (maxFiniteValue > 0.0F)
        {
            infValue = 10.0F * maxFiniteValue * (F32)nOfElements_16;
        }
        else
        {
            infValue = 10.0F;
        }

        for (index_u16 = 0U; index_u16<nOfElements_16; index_u16++)
        {
            if ( ASSIGNMENT_INVALID_DISTANCE > distMatrix[index_u16] )
            {
                /* do nothing */
            }
            else
            {
                distMatrix[index_u16] = infValue;
            }
        }
    }
#endif


    /* preliminary steps */
    if (nOfRows <= nOfColumns)
    {
        minDim_u16 = nOfRows;

        for (row_u16=0; row_u16<nOfRows; row_u16++)
        {
            /* find the smallest element in the row_u16 */
            minValue = distMatrix[row_u16];

            for (col_u16=1U; col_u16<nOfColumns; col_u16++)
            {
                index_u16 = (col_u16*nOfRows)+row_u16;
                value = distMatrix[index_u16];
                if (value < minValue)
                {
                    minValue = value;
                }
            }

            /* subtract the smallest element from each element of the row_u16 */
            for (col_u16=0U; col_u16<nOfColumns; col_u16++)
            {
                index_u16 = (col_u16*nOfRows)+row_u16;
                distMatrix[index_u16] -= minValue;
            }
        }

        /* Steps 1 and 2a */
        for (row_u16=0; row_u16<nOfRows; row_u16++)
        {
            for (col_u16=0; col_u16<nOfColumns; col_u16++)
            {
                if (fabs(distMatrix[row_u16 + (nOfRows*col_u16)]) < SMALL_NUMBER)
                {
                    if (!coveredColumns[col_u16])
                    {
                        starMatrix[row_u16 + (nOfRows*col_u16)] = TRUE;
                        coveredColumns[col_u16]           = TRUE;
                        break;
                    }
                }
            }
        }
    }
    else /* if (nOfRows > nOfColumns) */
    {
        minDim_u16 = nOfColumns;

        for (col_u16=0; col_u16<nOfColumns; col_u16++)
        {
            /* find the smallest element in the column */
            minValue = distMatrix[nOfRows*col_u16];
            for (row_u16 = 1; row_u16<nOfRows; row_u16++)
            {
                value = distMatrix[(nOfRows*col_u16)+row_u16];
                if (value < minValue)
                {
                    minValue = value;
                }
            }

            /* subtract the smallest element from each element of the column */
            for (row_u16 = 0; row_u16<nOfRows; row_u16++)
            {
                distMatrix[(nOfRows*col_u16) + row_u16] -= minValue;
            }
        }

        /* Steps 1 and 2a */
        for (col_u16=0; col_u16<nOfColumns; col_u16++)
        {
            for (row_u16=0; row_u16<nOfRows; row_u16++)
            {
                if (fabs(distMatrix[row_u16 + (nOfRows*col_u16)]) < SMALL_NUMBER)
                {
                    if (FALSE == coveredRows[row_u16])
                    {
                        starMatrix[row_u16 + (nOfRows*col_u16)] = TRUE;
                        coveredColumns[col_u16]           = TRUE;
                        coveredRows[row_u16]              = TRUE;
                        break;
                    }
                }
            }
        }
        for (row_u16=0; row_u16<nOfRows; row_u16++)
        {
            coveredRows[row_u16] = FALSE;
        }

    }

    /* move to step 2b */
    function_to_call = MUNKRES_STEP2B;
    while ( MUNKRES_FINISHED != function_to_call )
    {
        switch ( function_to_call )
        {
            case MUNKRES_STEP2A:
            {
                function_to_call = step2a(starMatrix, coveredColumns, nOfRows, nOfColumns);
                break;
            }
            case MUNKRES_STEP2B:
            {
                function_to_call = step2b(coveredColumns, nOfColumns, minDim_u16);
                break;
            }
            case MUNKRES_STEP3:
            {
                function_to_call = step3(distMatrix, starMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, &row_u16, &col_u16);
                break;
            }
            case MUNKRES_STEP4:
            {
                function_to_call = step4(starMatrix, newStarMatrix, primeMatrix, coveredRows, nOfRows, nOfColumns, row_u16, col_u16);
                break;
            }
            case MUNKRES_STEP5:
            {
                function_to_call = step5(distMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns);
                break;
            }
            default:
            {
                /* this should not happen */
                ASSERT(FALSE)
                break;
            }
        }
    }

    buildassignmentvector(assignment, starMatrix, nOfRows, nOfColumns);

    /* compute cost and remove invalid assignments */
    computeassignmentcost(assignment, cost, distMatrixIn, nOfRows, nOfColumns);

    return;
}

void buildassignmentvector(S16* assignment, BOOLEAN* starMatrix, U16 nOfRows, U16 nOfColumns)
{
    U16 row, col;

    for (row=0; row<nOfRows; row++)
    {
        for (col=0; col<nOfColumns; col++)
        {
            if (TRUE == starMatrix[row + (nOfRows*col)])
            {
                assignment[row] = (S16)col;
                break;
            }
        }
    }

}

void computeassignmentcost(S16* assignment, F32* cost, const F32* const distMatrixIn, U16 nOfRows, U16 nOfColumns)
{

    U16 row;
    S16 col;
#ifdef CHECK_FOR_INF
    F32 value;
#endif

    for (row=0; row<nOfRows; row++)
    {
        col = assignment[row];

        if (col >= 0)
        {
#ifdef CHECK_FOR_INF
            value = distMatrixIn[(nOfColumns*row) + col];
            if (ASSIGNMENT_INVALID_DISTANCE > value)
            {
                *cost += value;
            }
            else
            {
                assignment[row] = NOT_ASSIGNED;
            }
#else
            *cost += distMatrixIn[nOfColumns*row + col];
#endif
        }
    }
}

U8 step2a(BOOLEAN* starMatrix, BOOLEAN* coveredColumns, U16 nOfRows, U16 nOfColumns)
{
    U16 col_u16, row_u16;

    /* cover every column containing a starred zero */
    for (col_u16=0; col_u16<nOfColumns; col_u16++)
    {
        for (row_u16 = 0; row_u16<nOfRows; row_u16++)
        {
            if (TRUE == starMatrix[(nOfRows*col_u16) + row_u16])
            {
                coveredColumns[col_u16] = TRUE;
                break;
            }
        }
    }

    /* move to step 2b */
    return MUNKRES_STEP2B;

}

U8 step2b(BOOLEAN* coveredColumns, U16 nOfColumns, U16 minDim)
{

    U16 col, nOfCoveredColumns;

    /* count covered columns */
    nOfCoveredColumns = 0;
    for (col=0; col<nOfColumns; col++)
    {
        if (TRUE == coveredColumns[col])
        {
            nOfCoveredColumns++;
        }
    }

    if (nOfCoveredColumns == minDim)
    {
        /* algorithm finished */
        return MUNKRES_FINISHED;
    }
    else
    {
        return MUNKRES_STEP3;
    }
}

U8 step3(F32* distMatrix, BOOLEAN* starMatrix, BOOLEAN* primeMatrix, BOOLEAN* coveredColumns, BOOLEAN* coveredRows, U16 nOfRows, U16 nOfColumns, U16* const row_p, U16* const col_p)
{


    BOOLEAN zerosFound;
    U16 starCol, row, col;

    zerosFound = TRUE;
    while(TRUE == zerosFound)
    {
        zerosFound = FALSE;
        for (col=0; col<nOfColumns; col++)
        {
            if (FALSE == coveredColumns[col])
            {
                for (row=0; row<nOfRows; row++)
                {
                    if ((FALSE == coveredRows[row]) && (fabs(distMatrix[row + (nOfRows*col)]) < SMALL_NUMBER))
                    {
                        /* prime zero */
                        primeMatrix[row + (nOfRows*col)] = TRUE;

                        /* find starred zero in current row */
                        for (starCol=0; starCol<nOfColumns; starCol++)
                        {
                            if (TRUE == starMatrix[row + (nOfRows*starCol)])
                            {
                                break;
                            }
                        }

                        if (starCol == nOfColumns) /* no starred zero found */
                        {
                            *row_p = row;
                            *col_p = col;
                            /* move to step 4 */
                            return MUNKRES_STEP4;
                        }
                        else
                        {
                            coveredRows[row]        = TRUE;
                            coveredColumns[starCol] = FALSE;
                            zerosFound              = TRUE;
                            break;
                        }
                    }
                }
            }
        }
    }

    /* move to step 5 */
    return MUNKRES_STEP5;
}

U8 step4(BOOLEAN* starMatrix, BOOLEAN* newStarMatrix, BOOLEAN* primeMatrix, BOOLEAN* coveredRows, U16 nOfRows, U16 nOfColumns, U16 row, U16 col)
{

    U16 n, starRow, starCol, primeRow, primeCol;
    U16 nOfElements = nOfRows*nOfColumns;

    /* generate temporary copy of starMatrix */
    for (n=0; n<nOfElements; n++)
    {
        newStarMatrix[n] = starMatrix[n];
    }

    /* star current zero */
    newStarMatrix[row + (nOfRows*col)] = TRUE;

    /* find starred zero in current column */
    starCol = col;
    for (starRow=0; starRow<nOfRows; starRow++)
    {
        if (TRUE == starMatrix[starRow + (nOfRows*starCol)])
        {
            break;
        }
    }

    while(starRow<nOfRows)
    {
        /* unstar the starred zero */
        newStarMatrix[starRow + (nOfRows*starCol)] = FALSE;

        /* find primed zero in current row */
        primeRow = starRow;
        for (primeCol=0; primeCol<nOfColumns; primeCol++)
        {
            if (TRUE == primeMatrix[primeRow + (nOfRows*primeCol)])
            {
                break;
            }
        }

        /* star the primed zero */
        newStarMatrix[primeRow + (nOfRows*primeCol)] = TRUE;

        /* find starred zero in current column */
        starCol = primeCol;
        for (starRow=0; starRow<nOfRows; starRow++)
        {
            if (TRUE == starMatrix[starRow + (nOfRows*starCol)])
            {
                break;
            }
        }
    }

    /* use temporary copy as new starMatrix */
    /* delete all primes, uncover all rows */
    for (n=0; n<nOfElements; n++)
    {
        primeMatrix[n] = FALSE;
        starMatrix[n]  = newStarMatrix[n];
    }
    for (n=0; n<nOfRows; n++)
    {
        coveredRows[n] = FALSE;
    }

    /* move to step 2a */
    return MUNKRES_STEP2A;
}

U8 step5(F32* distMatrix, BOOLEAN* coveredColumns, BOOLEAN* coveredRows, U16 nOfRows, U16 nOfColumns)
{
    F32 h, value;
    U16 row, col;


    /* find smallest uncovered element h */
    h = ASSIGNMENT_INVALID_DISTANCE;

    for (row=0; row<nOfRows; row++)
    {
        if (FALSE == coveredRows[row])
        {
            for (col=0; col<nOfColumns; col++)
            {
                if (FALSE == coveredColumns[col])
                {
                    value = distMatrix[row + (nOfRows*col)];
                    if (value < h)
                    {
                        h = value;
                    }
                }
            }
        }
    }

    /* add h to each covered row */
    for (row=0; row<nOfRows; row++)
    {
        if (TRUE == coveredRows[row])
        {
            for (col=0; col<nOfColumns; col++)
            {
                distMatrix[row + (nOfRows*col)] += h;
            }
        }
    }

    /* subtract h from each uncovered column */
    for (col=0; col<nOfColumns; col++)
    {
        if (!coveredColumns[col])
        {
            for (row=0; row<nOfRows; row++)
            {
                distMatrix[row + (nOfRows*col)] -= h;
            }
        }
    }

    /* move to step 3 */
    return MUNKRES_STEP3;
}
#endif /* MUNKRES */

#ifdef SUBOPTIMAL1
/* //////////////////// */
/*  HERE SUBOPTIMAL1 ALGORITHM */
/* //////////////////// */

void assignmentsuboptimal1(S16* assignment, F32* cost, const F32* const distMatrixIn, U16 nOfRows, U16 nOfColumns)
{
    BOOLEAN infiniteValueFound, finiteValueFound, repeatSteps, allSinglyValidated, singleValidationFound;
    U16 n, row, col;/* , nOfElements; */
    U16 tmpRow = 0U;
    U16 tmpCol = 0U;
    F32 value, minValue, inf;
    F32 distMatrix[MAX_ROWS_ASSIGNMENT * MAX_COLUMNS_ASSIGNMENT];
    S16 nOfValidObservations[MAX_ROWS_ASSIGNMENT]; /*  actually, for us rows are tracks, but doesn't matter */
    S16 nOfValidTracks[MAX_COLUMNS_ASSIGNMENT];

    (void)memset(distMatrix, 0, sizeof distMatrix);
    (void)memset(nOfValidObservations, 0, sizeof nOfValidObservations);
    (void)memset(nOfValidTracks, 0, sizeof nOfValidTracks);


    inf = ASSIGNMENT_INVALID_DISTANCE;

    /* make working copy of distance Matrix */
    /* nOfElements   = nOfRows * nOfColumns; */
    for (row=0; row<nOfRows; row++)
    {
        for (col=0; col<nOfColumns; col++)
        {
            value = distMatrixIn[(row*nOfColumns) + col];
            /*  isfinite(x) and standard ASSIGNMENT_INVALID_DISTANCE don't work on this compiler, possible error source */
            #ifdef simulator
                if ((ASSIGNMENT_INVALID_DISTANCE > value) && (value < 0))
                    printf("Error in file %s, line %d, negative value in distance matrix", __FILE__, __LINE__);
            #endif
            distMatrix[row + (col*nOfRows)] = value;
        }
    }


    /* initialization */
    *cost = 0;

    for (row=0; row<nOfRows; row++)
    {
        assignment[row] = NOT_ASSIGNED;
    }

    /* compute number of validations */
    infiniteValueFound = FALSE;
    finiteValueFound  = FALSE;
    for (row=0; row<nOfRows; row++)
    {
        for (col=0; col<nOfColumns; col++)
        {
            if (ASSIGNMENT_INVALID_DISTANCE > (distMatrix[row + (nOfRows*col)]))
            {
                nOfValidTracks[col]       += 1;
                nOfValidObservations[row] += 1;
                finiteValueFound = TRUE;
            }
            else
            {
                infiniteValueFound = TRUE;
            }
        }
    }

    if (TRUE == infiniteValueFound)
    {
        if (FALSE == finiteValueFound)
        {
            return;
        }

        repeatSteps = TRUE;

        while(TRUE == repeatSteps)
        {
            repeatSteps = FALSE;

            /* step 1: reject assignments of multiply validated tracks to singly validated observations   */
            for (col=0; col<nOfColumns; col++)
            {
                singleValidationFound = FALSE;
                for (row=0; row<nOfRows; row++)
                {
                    if (    ( ASSIGNMENT_INVALID_DISTANCE > distMatrix[row + (nOfRows*col)] )
                        && ( nOfValidObservations[row] == 1 ) )
                    {
                        singleValidationFound = TRUE;
                        break;
                    }
                }

                if (TRUE == singleValidationFound)
                {
                    for (row=0; row<nOfRows; row++)
                    {
                        if ((nOfValidObservations[row] > 1) && (ASSIGNMENT_INVALID_DISTANCE > (distMatrix[row + (nOfRows*col)])))
                        {
                            distMatrix[row + (nOfRows*col)] = inf;
                            nOfValidObservations[row] -= 1;
                            nOfValidTracks[col]       -= 1;
                            repeatSteps = TRUE;
                        }
                    }
                }
            }

            /* step 2: reject assignments of multiply validated observations to singly validated tracks */
            if (nOfColumns > 1)
            {
                for (row=0; row<nOfRows; row++)
                {
                    singleValidationFound = FALSE;
                    for (col=0; col<nOfColumns; col++)
                    {
                        if ((ASSIGNMENT_INVALID_DISTANCE > (distMatrix[row + (nOfRows*col)])) && (nOfValidTracks[col] == 1))
                        {
                            singleValidationFound = TRUE;
                            break;
                        }
                    }

                    if (TRUE == singleValidationFound)
                    {
                        for (col=0; col<nOfColumns; col++)
                        {
                            if ((nOfValidTracks[col] > 1) && (ASSIGNMENT_INVALID_DISTANCE > (distMatrix[row + (nOfRows*col)])))
                            {
                                distMatrix[row + (nOfRows*col)] = inf;
                                nOfValidObservations[row] -= 1;
                                nOfValidTracks[col]       -= 1;
                                repeatSteps = TRUE;
                            }
                        }
                    }
                }
            }
        } /* while(repeatSteps) */

        /* for each multiply validated track that validates only with singly validated  */
        /* observations, choose the observation with minimum distance */
        for (row=0; row<nOfRows; row++)
        {
            if (nOfValidObservations[row] > 1)
            {
                allSinglyValidated = TRUE;
                minValue = inf;
                for (col=0; col<nOfColumns; col++)
                {
                    value = distMatrix[row + (nOfRows*col)];
                    if (ASSIGNMENT_INVALID_DISTANCE > value)
                    {
                        if (nOfValidTracks[col] > 1)
                        {
                            allSinglyValidated = FALSE;
                            break;
                        }
                        else
                        {
                            if ((nOfValidTracks[col] == 1) && (value < minValue))
                            {
                                tmpCol   = col;
                                minValue = value;
                            }
                            else
                            {
                                /* do nothing */
                            }
                        }
                    }
                }

                if (TRUE == allSinglyValidated)
                {
                    assignment[row] = (S16)tmpCol;

                    *cost += minValue;
                    for (n=0; n<nOfRows; n++)
                    {
                        distMatrix[n + (nOfRows*tmpCol)] = inf;
                    }
                    for (n=0; n<nOfColumns; n++)
                    {
                        distMatrix[row + (nOfRows*n)] = inf;
                    }
                }
            }
        }

        /* for each multiply validated observation that validates only with singly validated  */
        /* track, choose the track with minimum distance */
        for (col=0; col<nOfColumns; col++)
        {
            if (nOfValidTracks[col] > 1)
            {
                allSinglyValidated = TRUE;
                minValue = inf;
                for (row=0; row<nOfRows; row++)
                {
                    value = distMatrix[row + (nOfRows*col)];
                    if (ASSIGNMENT_INVALID_DISTANCE > value)
                    {
                        if (nOfValidObservations[row] > 1)
                        {
                            allSinglyValidated = FALSE;
                            break;
                        }
                        else
                        {
                            if ((nOfValidObservations[row] == 1) && (value < minValue))
                            {
                                tmpRow   = row;
                                minValue = value;
                            }
                            else
                            {
                                /* do nothing */
                            }
                        }
                    }
                }

                if (TRUE == allSinglyValidated)
                {
                    assignment[tmpRow] = (S16)col;

                    *cost += minValue;

                    for (n=0; n<nOfRows; n++)
                    {
                        distMatrix[n + (nOfRows*col)] = inf;
                    }
                    for (n=0; n<nOfColumns; n++)
                    {
                        distMatrix[tmpRow + (nOfRows*n)] = inf;
                    }
                }
            }
        }
    } /* if (infiniteValueFound) */

    /* now, recursively search for the minimum element and do the assignment */
    while(TRUE)
    {
        /* find minimum distance observation-to-track pair */
        minValue = inf;
        for (row=0; row<nOfRows; row++)
        {
            for (col=0; col<nOfColumns; col++)
            {
                value = distMatrix[row + (nOfRows*col)];
                if ((ASSIGNMENT_INVALID_DISTANCE > value) && (value < minValue))
                {
                    minValue = value;
                    tmpRow   = row;
                    tmpCol   = col;
                }
            }
        }

        if (ASSIGNMENT_INVALID_DISTANCE > minValue)
        {
            assignment[tmpRow] = (S16)tmpCol;

            *cost += minValue;

            for (n=0; n<nOfRows; n++)
            {
                distMatrix[n + (nOfRows*tmpCol)] = inf;
            }
            for (n=0; n<nOfColumns; n++)
            {
                distMatrix[tmpRow + (nOfRows*n)] = inf;
            }
        }
        else
        {
            break;
        }

    } /* while(TRUE) */

}
#endif /* SUBOPTIMAL1 */

PRIVATE void assignmentsuboptimal2( S16 assignment_result_vs16[],
                            F32* const p_cost_f32,
                            const F32 dist_matrix_vf32[],
                            U16 number_of_rows_u16,
                            U16 number_of_cols_u16 )
{
    /* local variables */
    U16 number_of_finite_elements_u16;
    U16 current_row_u16;
    U16 current_col_u16;
    U16 current_index_u16;
    U16 mapping_index_u16;
    F32 total_cost_f32;
    BOOLEAN assigned_row_vb[MAX_ROWS_ASSIGNMENT];
    BOOLEAN assigned_col_vb[MAX_ROWS_ASSIGNMENT];

    /* reset local arrays */
    (void)memset( assigned_row_vb, 0, sizeof(BOOLEAN) * MAX_ROWS_ASSIGNMENT );
    (void)memset( assigned_col_vb, 0, sizeof(BOOLEAN) * MAX_ROWS_ASSIGNMENT );
    total_cost_f32 = 0.0F;

    /* check and clamp max number to prevent overflow */
    number_of_rows_u16      = min(number_of_rows_u16, MAX_ROWS_ASSIGNMENT);
    number_of_cols_u16      = min(number_of_cols_u16, MAX_ROWS_ASSIGNMENT);

    /* initialize all assignments with -1 (not assigned)*/
    for (current_row_u16 = 0; current_row_u16 < number_of_rows_u16; current_row_u16++)
    {
        assignment_result_vs16[current_row_u16] = NOT_ASSIGNED;
    }

    mapping_index_u16 = 0U;
    current_index_u16 = 0U;

    /* save finite values of dist_matrix to g_dist_matrix_mapping_vs */
    for (current_row_u16 = 0; current_row_u16 < number_of_rows_u16; current_row_u16++)
    {
        for (current_col_u16 = 0; current_col_u16 < number_of_cols_u16; current_col_u16++)
        {
            if ( ASSIGNMENT_INVALID_DISTANCE > dist_matrix_vf32[current_index_u16] )
            {
                g_dist_matrix_mapping_vs[mapping_index_u16].dms_dist_f32 = dist_matrix_vf32[current_index_u16];
                g_dist_matrix_mapping_vs[mapping_index_u16].dms_col_u16 = current_col_u16;
                g_dist_matrix_mapping_vs[mapping_index_u16].dms_row_u16 = current_row_u16;
                mapping_index_u16++;
            }
            else
            {
                /* do nothing */
            }
            current_index_u16++;
        }
    }

    number_of_finite_elements_u16 = mapping_index_u16;

    /* sort g_dist_matrix_mapping_vs from min to max (key: dms_dist_f32)*/
    qsort(&g_dist_matrix_mapping_vs[0], number_of_finite_elements_u16, sizeof(DIST_MATRIX_MAPPING_STRUCT), &dist_matrix_compare);

    /* assignment */
    for (current_index_u16 = 0; current_index_u16 < number_of_finite_elements_u16; current_index_u16++)
    {
        /* mapping in row and column */
        current_row_u16 = g_dist_matrix_mapping_vs[current_index_u16].dms_row_u16;
        current_col_u16 = g_dist_matrix_mapping_vs[current_index_u16].dms_col_u16;

        /* do assignment only when both the row and the column are free */
        if (   (FALSE == assigned_row_vb[current_row_u16])
            && (FALSE == assigned_col_vb[current_col_u16])
            )
        {
            /* set the assignment */
            assignment_result_vs16[current_row_u16] = (S16)current_col_u16;

            /* sum the total cost */
            total_cost_f32 += g_dist_matrix_mapping_vs[current_index_u16].dms_dist_f32;

            /* set the column and row as assigned */
            assigned_row_vb[current_row_u16] = TRUE;
            assigned_col_vb[current_col_u16] = TRUE;
        }
        else
        {
            /* either the row or the column are already assigned, no backtracking in suboptimal solution,
             * so reject this combination. */
        }
    }

    if ( NULL != p_cost_f32 )
    {
        *p_cost_f32 = total_cost_f32;
    }

}

PRIVATE int dist_matrix_compare(const void* p_a_s, const void* p_b_s)
{
    F32 a_f32;
    F32 b_f32;
    int result = 0;
    if (   ( NULL != p_a_s )
        && ( NULL != p_b_s )
        )
    {
        a_f32 = ((const DIST_MATRIX_MAPPING_STRUCT*)p_a_s)->dms_dist_f32;
        b_f32 = ((const DIST_MATRIX_MAPPING_STRUCT*)p_b_s)->dms_dist_f32;
        result = ((S16)(a_f32 > b_f32)) - ((S16)(a_f32 < b_f32));
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
    return result;
}
