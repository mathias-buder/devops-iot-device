
/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: ring_structure.c $
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

/**********************************/
/* GLOBAL QAC WARNING SUPPRESSION */
/**********************************/

/*  DEVIATION:     0491
    DESCRIPTION:   Array subscripting applied to an object of pointer type.
    JUSTIFICATION: This file contains well used legacy code. The QAC help text (below) associated
    with this warning also implies that it is a question of company coding standards rather than
    outright 'bad' code:-

    "This is perfectly legitimate in the C language providing the pointer addresses
    an array element; but some coding standards recommend that if a parameter refers to an
    array, it should be declared as an array."  */
    /* PRQA S 0491 EOF*/

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
/*      FILE DEFINITIONS                                     */
/*************************************************************/

#define RING_STRUCTURE          /*  defines this file */
#define UTIL_SUBSYSTEM          /*  defines the subsystem that this file resides in */

/*************************************************************/
/*      INCLUDE FILES                                        */
/*************************************************************/

#include <assert.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../TRACK/TRACK.h"

#include "../UTIL/ring_structure.h"
#include "../UTIL/vector_manipulation.h"
#include "../UTIL/matrix_manipulation.h"
#include "../UTIL/util_types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/*************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                        */
/*************************************************************/

/*************************************************************/
/*      CODE                                                 */
/*************************************************************/


void InitializeRingBuffer(RING_STRUCT* const ring_buffer_ps, F32* const buffer_vf32, U8 max_length_u8)
{
    if((NULL != ring_buffer_ps)
        && (NULL != buffer_vf32))
    {
        ring_buffer_ps->buffer_vf32        = buffer_vf32;
        ring_buffer_ps->max_length_u8      = max_length_u8;
        ring_buffer_ps->current_length_u8  = 0;
        ring_buffer_ps->next_free_index_u8 = 0;
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}

void InitializeCachedRingBuffer(CACHED_RING_STRUCT* const p_cached_ring_buffer_s,  F32* const buffer_vf32, const U8 buffer_length_u8, const U8 cache_length_u8)
{
    if((NULL != p_cached_ring_buffer_s) && (NULL != buffer_vf32))
    {
        InitializeRingBuffer(&p_cached_ring_buffer_s->background_buffer_s, buffer_vf32, buffer_length_u8);
        p_cached_ring_buffer_s->max_cache_length_u8     = cache_length_u8;
        p_cached_ring_buffer_s->current_cache_length_u8 = 0U;
        p_cached_ring_buffer_s->cache_f32               = 0.0F;
        p_cached_ring_buffer_s->current_length_u8       = 0U;
    }
    else
    {
        ASSERT((NULL != p_cached_ring_buffer_s) && (NULL != buffer_vf32))
    }
}

/******************************************************************************/
void InitializeRingBuffer32(RING_STRUCT_32* const ring_buffer_ps)
{
    if(NULL != ring_buffer_ps)
    {
        ring_buffer_ps->current_length_u8 = 0;
        ring_buffer_ps->next_free_index_u8 = 0;
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != ring_buffer_ps)
    }
}

void InsertIntoRingBuffer32(RING_STRUCT_32* const ring_buffer_ps, const S8 current_value_s8)
{
    if(NULL != ring_buffer_ps)
    {
        /*  add to next free position */
        ring_buffer_ps->buffer_vs8[ring_buffer_ps->next_free_index_u8] = current_value_s8;

        /*  set current length */
        if (ring_buffer_ps->current_length_u8 < MAX_RING_BUFFER_32)
        {
            ring_buffer_ps->current_length_u8++;
        }


        /*  set current free index */
        if (ring_buffer_ps->next_free_index_u8 < (MAX_RING_BUFFER_32 - 1) )
        {
            ring_buffer_ps->next_free_index_u8++;
        }
        else
        {
            ring_buffer_ps->next_free_index_u8 = 0;
        }
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != ring_buffer_ps)
    }
}

F32 ComputeWeightedSuperposition32(const RING_STRUCT_32* const ring_buffer_ps)
{
    /* locals */
    U8          index_u8                            = 0;
    U8          weights_index_u8                    = 0;
    F32         weighted_mean_f32                   = 0.0F;

    /*  those weights need to be adapted if a different ring buffer size is used! */
    const F32   weights_vf32[MAX_RING_BUFFER_32]       =
    {
        1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0,
        17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0
    };

    if(NULL != ring_buffer_ps)
    {
        /*  sum over weighted ring buffer */
        for (index_u8 = 0; index_u8 < ring_buffer_ps->current_length_u8; index_u8++)
        {
            /*  weights index */
            weights_index_u8 = ( MAX_RING_BUFFER_32 - ring_buffer_ps->next_free_index_u8 ) + index_u8;

            /*  take care of index overflow */
            if (weights_index_u8 >= MAX_RING_BUFFER_32)
            {
                weights_index_u8 = weights_index_u8 - MAX_RING_BUFFER_32;
            }
            else
            {
                /*  do nothing */
            }

            /*  compute weighted mean */
            weighted_mean_f32 = weighted_mean_f32 + ( weights_vf32[weights_index_u8] * (F32)ring_buffer_ps->buffer_vs8[index_u8] );
        }
    }
    else
    {
        weighted_mean_f32 = 0.0F;

        ASSERT(NULL != ring_buffer_ps)
    }

    /*  return value */
    return weighted_mean_f32;
}

U8 ComputeNumberCoincidences32(const RING_STRUCT_32* const ring_buffer_ps, const S8 track_id_s8)
{
    /* locals */
    U8          index_u8;
    U8          number_coincidences_u8 = 0;

    if(NULL != ring_buffer_ps)
    {
        /*  sum over weighted ring buffer */
        for (index_u8 = 0; index_u8 < ring_buffer_ps->current_length_u8; index_u8++)
        {
            /*  check for identical ids */
            if (ring_buffer_ps->buffer_vs8[index_u8] == track_id_s8)
            {
                number_coincidences_u8++;
            }
            else
            {
                /*  do nothing */
            }
        }
    }
    else
    {
        /*  do nothing */
        ASSERT(NULL != ring_buffer_ps)
    }

    /*  return value */
    return number_coincidences_u8;
}

U8 ComputeNumberCoincidencesInRingStructs32(const RING_STRUCT_32* const ring_buffer_1_ps, const RING_STRUCT_32* const ring_buffer_2_ps)
{
    /* locals */
    U8          index_u8;
    U8          number_coincidences_u8 = 0;

    if((NULL != ring_buffer_1_ps)
        &&(NULL != ring_buffer_2_ps) )
    {
        /*  look for same values over the minimum of the lengths of both ring buffers */
        for (index_u8 = 1; index_u8 <= min(ring_buffer_1_ps->current_length_u8, ring_buffer_2_ps->current_length_u8); index_u8++)
        {
            /*  check for identical ids, do not count no associations */
            /*  search backwards from last used index = next_free_index_u8 - 1 */
            if (    (ring_buffer_1_ps->buffer_vs8[( ( MAX_RING_BUFFER_32 + ring_buffer_1_ps->next_free_index_u8 ) - index_u8) % MAX_RING_BUFFER_32]
                     == ring_buffer_2_ps->buffer_vs8[( ( MAX_RING_BUFFER_32 + ring_buffer_2_ps->next_free_index_u8 ) - index_u8) % MAX_RING_BUFFER_32])
                 && (ring_buffer_1_ps->buffer_vs8[( ( MAX_RING_BUFFER_32 + ring_buffer_1_ps->next_free_index_u8 ) - index_u8) % MAX_RING_BUFFER_32] != NO_TARGET)
                )
            {
                number_coincidences_u8++;
            }
            else
            {
                /*  do nothing */
            }
        }
    }
    else
    {
        /*  do nothing */
        ASSERT(FALSE)
    }

    /*  return value */
    return number_coincidences_u8;
}

/******************************************************************************/
void ResetRingBuffer(RING_STRUCT* const ring_buffer_ps)
{
    if(NULL != ring_buffer_ps)
    {
        ring_buffer_ps->current_length_u8  = 0;
        ring_buffer_ps->next_free_index_u8 = 0;
    }
    else
    {
        /*  do nothing */
        ASSERT(NULL != ring_buffer_ps)
    }
}

void ResetCachedRingBuffer(CACHED_RING_STRUCT* const p_cached_ring_buffer_s)
{
    if (NULL != p_cached_ring_buffer_s)
    {
        ResetRingBuffer(&p_cached_ring_buffer_s->background_buffer_s);
        p_cached_ring_buffer_s->cache_f32 = 0.0F;
        p_cached_ring_buffer_s->current_cache_length_u8 = 0U;
        p_cached_ring_buffer_s->current_length_u8 = 0U;
    }
    else
    {
        ASSERT(NULL != p_cached_ring_buffer_s)
    }
}


/******************************************************************************/
void InsertIntoRingBuffer(RING_STRUCT* const ring_buffer_ps, const F32 current_value_f32)
{
    if(NULL != ring_buffer_ps)
    {
        /*  add to next free position */
        ring_buffer_ps->buffer_vf32[ring_buffer_ps->next_free_index_u8] = current_value_f32;

        /*  set current length */
        if (ring_buffer_ps->current_length_u8 < ring_buffer_ps->max_length_u8)
        {
            ring_buffer_ps->current_length_u8++;
        }


        /*  set current free index */
        if (ring_buffer_ps->next_free_index_u8 < (ring_buffer_ps->max_length_u8 - 1) )
        {
            ring_buffer_ps->next_free_index_u8++;
        }
        else
        {
            ring_buffer_ps->next_free_index_u8 = 0;
        }
    }
    else
    {
        /*  do nothing */
        ASSERT(NULL != ring_buffer_ps)
    }
}

void InsertIntoCachedRingBuffer(CACHED_RING_STRUCT* const p_cached_ring_buffer_s, const F32 current_value_f32)
{
    if (NULL != p_cached_ring_buffer_s)
    {
        p_cached_ring_buffer_s->cache_f32 += current_value_f32;
        if (p_cached_ring_buffer_s->current_cache_length_u8 < (p_cached_ring_buffer_s->max_cache_length_u8 - 1))
        {
            /* save to short-term memory */
            p_cached_ring_buffer_s->current_cache_length_u8++;
        }
        else
        {
            /* insert into background buffer and wipe short-term memory */
            InsertIntoRingBuffer(&p_cached_ring_buffer_s->background_buffer_s, p_cached_ring_buffer_s->cache_f32);
            p_cached_ring_buffer_s->cache_f32 = 0.0F;
            p_cached_ring_buffer_s->current_cache_length_u8 = 0U;
            p_cached_ring_buffer_s->current_length_u8 = p_cached_ring_buffer_s->background_buffer_s.current_length_u8 * p_cached_ring_buffer_s->max_cache_length_u8;
        }
    }
    else
    {
        ASSERT(NULL != p_cached_ring_buffer_s)
    }
}

F32 GetRingElement(const RING_STRUCT* const ring_buffer_ps, const U8 element_u8)
{
    U8 nr_element_u8;
    U8 limited_element_u8;
    F32 result;

    if(NULL != ring_buffer_ps)
    {
        limited_element_u8 = element_u8 % ring_buffer_ps->max_length_u8;
        nr_element_u8 = ( ( ring_buffer_ps->max_length_u8 + ring_buffer_ps->next_free_index_u8 ) - (limited_element_u8+1) ) % ring_buffer_ps->max_length_u8;

        if ( nr_element_u8 >= ring_buffer_ps->max_length_u8 )
        {
            nr_element_u8 = 0;

            ASSERT(FALSE)
        }

        ASSERT(element_u8 < ring_buffer_ps->current_length_u8)
        ASSERT(ring_buffer_ps->buffer_vf32[nr_element_u8] == ring_buffer_ps->buffer_vf32[( ring_buffer_ps->max_length_u8 + ring_buffer_ps->next_free_index_u8 - (limited_element_u8+1) ) % ring_buffer_ps->max_length_u8])

        result = ring_buffer_ps->buffer_vf32[nr_element_u8];
    }
    else
    {
        result = INVALID_NUMBER;

        ASSERT(NULL != ring_buffer_ps)
    }

    return result;
}


U16 GetRingAsArray(F32* const array_pf32, const RING_STRUCT* const ring_buffer_ps, U8 size_u8)
{
    U8 i_u8;
    U8 result;

    if(NULL != ring_buffer_ps)
    {
        for ( i_u8 = 0; i_u8 < size_u8; i_u8++ )
        {
            if ( i_u8 < ring_buffer_ps->current_length_u8 )
            {
                array_pf32[i_u8] = GetRingElement(ring_buffer_ps, i_u8);
            }
            else
            {
                array_pf32[i_u8] = INVALID_NUMBER;
            }
        }
        result = ring_buffer_ps->current_length_u8;
    }
    else
    {
        result = 0U;

        ASSERT(NULL != ring_buffer_ps)
    }

    return result;
}


F32 ComputeRingBufferSimpleMovingAverage(const RING_STRUCT* const ring_buffer_ps, U8 window_size_u8)
{
    /* local variables */
    U8  block1_first_index_u8;
    U8  block1_length_u8;
    U8  block2_first_index_u8;
    U8  block2_length_u8;
    U8  current_index_u8;
    F32 sum_f32;
    F32 moving_average_f32;


    if (NULL != ring_buffer_ps)
    {
        /* clamp window size to current length of ring buffer */
        window_size_u8 = min(window_size_u8, ring_buffer_ps->current_length_u8);

        if (window_size_u8 > 0)
        {
            /* reset sum to zero */
            sum_f32 = 0.0F;

            /* identify blocks containing previous n elements in ring buffer */

            /* define the first block (the elements with indices between 0 and next_free_index_u8-1 in ring buffer)
             * dependent on the specified window size */
            block1_length_u8        = min(ring_buffer_ps->next_free_index_u8, window_size_u8);
            block1_first_index_u8   = ring_buffer_ps->next_free_index_u8 - block1_length_u8;

            /* get the sum of first block */
            for (current_index_u8 = block1_first_index_u8; current_index_u8 < (block1_first_index_u8 + block1_length_u8); current_index_u8++)
            {
                sum_f32 += ring_buffer_ps->buffer_vf32[current_index_u8];
            }

            /* if the specified window size is larger than the length of first block,
             * define the second block (the elements with indices between next_free_index_u8 and max_length_u8-1 in ring buffer),
             * the length of second block is the specified window size - the length of first block */
            if (window_size_u8 > block1_length_u8)
            {
                block2_length_u8        = window_size_u8 - block1_length_u8;
                block2_first_index_u8   = ring_buffer_ps->max_length_u8 - block2_length_u8;

                /* get the sum of second block */
                for (current_index_u8 = block2_first_index_u8; current_index_u8 < (block2_first_index_u8 + block2_length_u8); current_index_u8++)
                {
                    sum_f32 += ring_buffer_ps->buffer_vf32[current_index_u8];
                }
            }

            /* compute simple moving average */
            moving_average_f32 = sum_f32 / (F32)window_size_u8;
        }
        else
        {
            moving_average_f32 = INVALID_NUMBER;
        }
    }
    else
    {
        moving_average_f32 = INVALID_NUMBER;
        ASSERT(FALSE)
    }

    /*  return value */
    return moving_average_f32;
}


F32 ComputeRingBufferSimpleMovingAverageAll(const RING_STRUCT* const ring_buffer_ps)
{
    /* local variables */
    F32 moving_average_f32;


    if (NULL != ring_buffer_ps)
    {
        moving_average_f32 = ComputeRingBufferSimpleMovingAverage(ring_buffer_ps, ring_buffer_ps->current_length_u8);
    }
    else
    {
        moving_average_f32 = INVALID_NUMBER;
        ASSERT(FALSE)
    }

    return moving_average_f32;
}

F32 ComputeCachedRingBufferMovingAverageAll(const CACHED_RING_STRUCT* const p_cached_ring_buffer_s)
{
    /* local variables */
    F32 moving_average_f32;
    F32 background_moving_average_f32;

    if (    (NULL != p_cached_ring_buffer_s)
         && (0U == p_cached_ring_buffer_s->current_cache_length_u8) /* buffer is up-to-date */
         && (p_cached_ring_buffer_s->max_cache_length_u8 > 0U) )
    {
        background_moving_average_f32 = ComputeRingBufferSimpleMovingAverageAll(&p_cached_ring_buffer_s->background_buffer_s);
        if ( !isINVALID(background_moving_average_f32) )
        {
            moving_average_f32 = background_moving_average_f32 / (F32)p_cached_ring_buffer_s->max_cache_length_u8;
        }
        else
        {
            moving_average_f32 = INVALID_NUMBER;
        }
    }
    else
    {
        ASSERT(NULL != p_cached_ring_buffer_s)
        moving_average_f32 = INVALID_NUMBER;
    }

    return moving_average_f32;
}
