/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: ring_structure.h $
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
 * @details This file defines operations on the Ring structures which are defined in foundation.h
 */

#ifndef RING_STRUCTURE_H
#define RING_STRUCTURE_H

#include "../UTIL/util_types.h"

/*************************************************************/
/*      DEFINITIONS EXPORTED FROM THIS SUBSYSTEM             */
/*************************************************************/

/*************************************************************/
/*      RING BUFFER                                          */
/*                                                           */
/*  -> easy to add and remove elements: O(1)                 */
/*  -> easy to access elements: O(1)                         */
/*  -> expensive to access the full array:  O(n)             */
/*                                                           */
/*************************************************************/


/**
 * @details
 * @param[out] 'ring_buffer_ps'
 * @param[in] 'buffer_vf32'
 * @param[in] 'max_length_u8'
 */
void InitializeRingBuffer(RING_STRUCT* const ring_buffer_ps, F32* const buffer_vf32, U8 max_length_u8);

/**
 * @details This function initializes a cached ringbuffer.
 * @memberof CACHED_RING_STRUCT_TAG
 * @param[in,out] p_cached_ring_buffer_s pointer of the buffer to be initialized
 * @param[in] buffer_vf32                array to store the data
 * @param[in] buffer_length_u8           the length of buffer_vf32
 * @param[in] cache_length_u8            the number of elements to cache before insertion into background ringbuffer.
 */
void InitializeCachedRingBuffer(CACHED_RING_STRUCT* const p_cached_ring_buffer_s,  F32* const buffer_vf32, const U8 buffer_length_u8, const U8 cache_length_u8);

/**
 * @details
 * @param[out] 'ring_buffer_ps'
 */
void ResetRingBuffer(RING_STRUCT* const ring_buffer_ps);

/**
 * @details this function resets the contents of the given cached ringbuffer
 * @memberof CACHED_RING_STRUCT_TAG
 * @param p_cached_ring_buffer_s the pointer to the cached ringbuffer to be reset.
 */
void ResetCachedRingBuffer(CACHED_RING_STRUCT* const p_cached_ring_buffer_s);

/**
 * @details
 * @param[out] 'ring_buffer_ps'
 * @param[in] 'current_value_f32'
 */
void InsertIntoRingBuffer(RING_STRUCT* const ring_buffer_ps, const F32 current_value_f32);

/**
 * Insert an element into the cached ringbuffer.
 * @memberof CACHED_RING_STRUCT_TAG
 * @param[in,out] p_cached_ring_buffer_s pointer to the cached ringbuffer.
 * @param[in] current_value_f32 the element to be inserted.
 */
void InsertIntoCachedRingBuffer(CACHED_RING_STRUCT* const p_cached_ring_buffer_s, const F32 current_value_f32);

/**
 * @details
 * @param[in] 'ring_buffer_ps'
 * @param[in] 'element_u8'
 */
F32 GetRingElement(const RING_STRUCT* const ring_buffer_ps, const U8 element_u8);

/**
 * @details This function initializes the 'next_free_index' and 'current_length' attributes.
 * @param[out] 'ring_buffer_ps'
 */
void InitializeRingBuffer32(RING_STRUCT_32* const ring_buffer_ps);

/**
 * @details This function inserts the element into ring buffer.
 * @param[out] 'ring_buffer_ps'
 * @param[in] 'current_value_s8'
 */
void InsertIntoRingBuffer32(RING_STRUCT_32* const ring_buffer_ps, const S8 current_value_s8);

/**
 * @details This function computes the compute weighted mean.
 * @param[in] 'ring_buffer_ps'
 * @return
 */
F32 ComputeWeightedSuperposition32(const RING_STRUCT_32* const ring_buffer_ps);

/**
 * @details This function computes the number of times the track with 'track_id_s8' has been associated.
 * @param[in] 'ring_buffer_ps'
 * @param[in] 'track_id_s8'
 * @return
 */
U8 ComputeNumberCoincidences32(const RING_STRUCT_32* const ring_buffer_ps, const S8 track_id_s8);

/**
 * @details This function compares the two ring buffers.
 * @param[in] 'ring_buffer_1_ps'
 * @param[in] 'ring_buffer_2_ps'
 */
U8 ComputeNumberCoincidencesInRingStructs32(const RING_STRUCT_32* const ring_buffer_1_ps, const RING_STRUCT_32* const ring_buffer_2_ps);

/**
 * @details This function gets the first min(size, MAX_BIG_RING_BUFFER) elements in array array.
 * If ring_buffer_ps->current_length_u8 < size, fill rest of elements with NAN.
 * @param[out] 'array_pf32'
 * @param[in] 'ring_buffer_ps'
 * @param[in] 'size_u8'
 * @return
 */
U16 GetRingAsArray(F32* const array_pf32, const RING_STRUCT* const ring_buffer_ps, U8 size_u8);

/**
 * @details This function calculates simple moving average, which is the unweighted mean of the previous n elements in ring buffer.
 * n is the specified window size, which is clamped between 0 and the current ring buffer length.
 * @param[in] 'ring_buffer_ps' is the pointer to the ring buffer
 * @param[in] 'windows_size_u8' is the specified window size
 *  */
F32 ComputeRingBufferSimpleMovingAverage(const RING_STRUCT* const ring_buffer_ps, U8 window_size_u8);

/**
 * @details This function calculates simple moving average, which is the unweighted mean of the previous n elements in ring buffer.
 * n is the current ring buffer length.
 * @param[in] 'ring_buffer_ps' is the pointer to the ring buffer
 *  */
F32 ComputeRingBufferSimpleMovingAverageAll(const RING_STRUCT* const ring_buffer_ps);

/**
 * @details This functions calculates the moving average of a given cached buffer.
 * @memberof CACHED_RING_STRUCT_TAG
 * @param[in] p_cached_ring_buffer_s pointer to the cached buffer.
 * @return the moving average of the cached buffer if the buffer is up-to-date, INVALIDNUMBER else.
 */
F32 ComputeCachedRingBufferMovingAverageAll(const CACHED_RING_STRUCT* const p_cached_ring_buffer_s);

#endif /*RING_STRUCTURE_H */
