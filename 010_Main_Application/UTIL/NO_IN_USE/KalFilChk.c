
/* PRQA S 0292 17 */
/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: KalFilChk.c $
        $Revision: 1.1 $
        $Date: 2018/05/28 16:34:24CEST $

        TRW Ltd owns the copyright in this document and associated
        documents and all rights are reserved.  These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of TRW
        Ltd. Any copy of this drawing or document made by any method
        must also include a copy of this legend.

    $CopyrightDate: (c) ZF 2019 $

***************************************************************************************************/

/*************************************************************/
/*      FILE DEFINITIONS                                     */
/*************************************************************/
/**
 * @file
 * @brief general details of file
 */

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <mathf.h>
#include <string.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../UTIL/util_types.h"
#include "../UTIL/ModTypes.h"
#include "../UTIL/ring_structure.h"
#include "../UTIL/KalFilGen.h"
#include "../UTIL/KalFilChk.h"


/*************************************************************/
/*      DEFINES                                              */
/*************************************************************/


/*************************************************************/
/*      LOCAL VARIABLES                                      */
/*************************************************************/


/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/
/**
 * @details This function computes normalized innovation squared (NIS) and puts it into NIS sequence.
 * @param[in,out] 'p_NIS_sequence_s' is pointer to NIS sequence
 * @param[in] 'innovation_value_f32' is the innovation
 * @param[in] 'innovation_variance_f32' is the innovation variance
 */
PRIVATE void update_NIS_sequence(
        INNOVATION_SEQUENCE* const p_NIS_sequence_s,
        const F32 innovation_value_f32,
        const F32 innovation_variance_f32);

/**
 * @details This generic function computes chi-squared confidence bound based on sample size and variance.
 * @param[out] 'p_confidence_bound_s' is pointer to confidence bound
 * @param[in] 'variance_f32' is the variance
 * @param[in] 'sample_size_u8' is the sample size
 */
PRIVATE void get_chi_squared_confidence_bound(
        CONFIDENCE_BOUND* const p_confidence_bound_s,
        const F32 variance_f32,
        const U8 sample_size_u8 );

/**
 * @details This function checks whether normalized innovation squared (NIS) is within the specified chi-squared confidence region.
 * @param[return] the test result in trilean (passed, failed or not available
 * @param[out] 'p_confidence_bound_s' is pointer to NIS confidence bound
 * @param[in] 'p_sequence_s' is pointer to NIS sequence
 * @param[in] 'min_sequence_length_u8' is the minimum of sequence length to allow the check
 */
PRIVATE TEST_RESULT_TRILEAN check_NIS(
        const INNOVATION_SEQUENCE* const p_sequence_s,
        const CONFIDENCE_BOUND* const p_confidence_bound_s
        );

#ifdef simulator
/**
 * @details This function puts innovation into innovation sequence.
 * @param[in,out] 'p_innovation_sequence_s' is pointer to innovation sequence
 * @param[in] 'innovation_value_f32' is the innovation
 */
PRIVATE void update_innovation_sequence(
        INNOVATION_SEQUENCE* const p_innovation_sequence_s,
        const F32 innovation_value_f32 );
#endif


/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

void innovation_consistency_check_init(
        INNOVATION_CHECK_STRUCT* const p_innovation_check_s)
{
    if (NULL != p_innovation_check_s)
    {
        /* reset consistency check struct */
        (void)memset(p_innovation_check_s, 0, sizeof(INNOVATION_CHECK_STRUCT));

        /* initialize ring buffers */
        InitializeCachedRingBuffer( &p_innovation_check_s->NIS_sequence_s.ring_struct_s,
                                     p_innovation_check_s->NIS_sequence_s.buffer_vf32,
                                     DIM_INNOVATION_SEQUENCE_BACKGROUND_BUFFER,
                                     INNOVATION_SEQUENCE_DELAY);

    #ifdef simulator
        InitializeCachedRingBuffer( &p_innovation_check_s->innovation_sequence_s.ring_struct_s,
                                     p_innovation_check_s->innovation_sequence_s.buffer_vf32,
                                     DIM_INNOVATION_SEQUENCE_BACKGROUND_BUFFER,
                                     INNOVATION_SEQUENCE_DELAY);
    #endif
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

}

void innovation_consistency_check(
        INNOVATION_CHECK_STRUCT* const p_innovation_check_s,
        const F32 innovation_value_f32,
        const F32 innovation_variance_f32 )
{
    if (NULL != p_innovation_check_s)
    {

    #ifdef simulator
        /* store innovation to innovation sequence */
        update_innovation_sequence( &p_innovation_check_s->innovation_sequence_s,
                                    innovation_value_f32 );
    #endif

        /* compute and store normalized innovation squared (NIS) into sequence */
        update_NIS_sequence( &p_innovation_check_s->NIS_sequence_s,
                             innovation_value_f32,
                             innovation_variance_f32 );

        /* perform the check only if sequence length is larger than min_sequence_length_u8 and
         * the moving average is valid */

        if (    (!isINVALID(p_innovation_check_s->NIS_sequence_s.moving_average_f32))
             && (p_innovation_check_s->NIS_sequence_s.ring_struct_s.current_length_u8 >= kalfilchk_min_consistency_check_sequence_length_u8) )
        {
            /* compute confidence bound for NIS by chi-squared test */
            get_chi_squared_confidence_bound( &p_innovation_check_s->NIS_confidence_bound_s,
                                              innovation_variance_f32,
                                              p_innovation_check_s->NIS_sequence_s.ring_struct_s.current_length_u8);

            /* perform NIS check */
            p_innovation_check_s->NIS_test_result_e = check_NIS( &p_innovation_check_s->NIS_sequence_s,
                                                                 &p_innovation_check_s->NIS_confidence_bound_s);
        }
        else
        {
            p_innovation_check_s->NIS_confidence_bound_s.lower_limit_f32 = INVALID_NUMBER;
            p_innovation_check_s->NIS_confidence_bound_s.upper_limit_f32 = INVALID_NUMBER;
            p_innovation_check_s->NIS_test_result_e = TEST_RESULT_NOT_AVAILABLE;
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}

#ifdef simulator
PRIVATE void update_innovation_sequence(
        INNOVATION_SEQUENCE* const p_innovation_sequence_s,
        const F32 innovation_value_f32 )
{
    p_innovation_sequence_s->current_value_f32 = innovation_value_f32;

    /* put into sequence ring buffer */
    InsertIntoCachedRingBuffer( &p_innovation_sequence_s->ring_struct_s,
                                 innovation_value_f32 );

    /* calculate moving average from sequence ring buffer */
    p_innovation_sequence_s->moving_average_f32 = ComputeCachedRingBufferMovingAverageAll( &p_innovation_sequence_s->ring_struct_s );
}
#endif

PRIVATE void update_NIS_sequence(
        INNOVATION_SEQUENCE* const p_NIS_sequence_s,
        const F32 innovation_value_f32,
        const F32 innovation_variance_f32)
{
    if (innovation_variance_f32 > (SMALL_NUMBER * SMALL_NUMBER) )
    {
        /* compute normalized innovation squared (NIS) */
        p_NIS_sequence_s->current_value_f32 = squaref(innovation_value_f32) / innovation_variance_f32;

        /* put into sequence ring buffer */
        InsertIntoCachedRingBuffer( &p_NIS_sequence_s->ring_struct_s,
                                     p_NIS_sequence_s->current_value_f32 );

        /* calculate moving average from sequence ring buffer */
        p_NIS_sequence_s->moving_average_f32 = ComputeCachedRingBufferMovingAverageAll( &p_NIS_sequence_s->ring_struct_s );
    }
    else
    {
        /* reset current value and do not update ring buffer and moving average */
        p_NIS_sequence_s->current_value_f32 = 0.0F;
    }

}

PRIVATE TEST_RESULT_TRILEAN check_NIS(const INNOVATION_SEQUENCE* const p_sequence_s, const CONFIDENCE_BOUND* const p_confidence_bound_s)
{
    TEST_RESULT_TRILEAN check_result_e;

    /* test passed if moving average is within the specified confidence bound */
    if (   (p_confidence_bound_s->lower_limit_f32 <= p_sequence_s->moving_average_f32)
        && (p_confidence_bound_s->upper_limit_f32 >= p_sequence_s->moving_average_f32)
        )
    {
        check_result_e = TEST_RESULT_PASSED;
    }
    else
    {
        check_result_e = TEST_RESULT_FAILED;
    }

    return check_result_e;

}

PRIVATE void get_chi_squared_confidence_bound(
        CONFIDENCE_BOUND* const p_confidence_bound_s,
        const F32 variance_f32,
        const U8 sample_size_u8 )
{
    /*
     * input parameters for the chi-squared test are:
     * - sample variance
     * - sample size (as degree of freedom)
     * - significance level = 0.95 (constant)
     */
    U8 index_chib_u8;


    /* range check of sample_size_u8 */
    if (sample_size_u8 < 1)
    {
        ASSERT(FALSE)
        /* set array index to min allowed index */
        index_chib_u8 = 0;
    }
    else if (sample_size_u8 > MAX_NUMBER_CHI_SQUARED_INVERSE_BKPTS)
    {
        ASSERT(FALSE)
        /* set array index to max allowed index */
        index_chib_u8 = MAX_NUMBER_CHI_SQUARED_INVERSE_BKPTS - 1;
    }
    else
    {
        /* get index of chi squared array */
        index_chib_u8 = sample_size_u8 - 1;
    }

    if (NULL != p_confidence_bound_s)
    {
        p_confidence_bound_s->lower_limit_f32   = chib_low_vf32[index_chib_u8];
        p_confidence_bound_s->upper_limit_f32   = chib_up_vf32[index_chib_u8];
    }
    else
    {
        ASSERT(FALSE)
    }

}
