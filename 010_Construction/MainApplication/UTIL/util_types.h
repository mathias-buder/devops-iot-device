/*********************************************************************
    MODULE SPECIFICATION:

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file util_types.h
        @details Some detailed description

*********************************************************************/
#ifndef UTIL_TYPES_H_
#define UTIL_TYPES_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../types.h"
#include "util_matrix_indices.h"

/*************************************************************/
/*      DEFINES                                              */
/*************************************************************/

#define INVALID_NUMBER              10000000000000.0F
#define INVALID_NUMBER_VICINITY     1000000000000.0F
#define SMALL_NUMBER_SQUARED        (SMALL_NUMBER * SMALL_NUMBER)

/*************************************************************/
/*      MACROS                                               */
/*************************************************************/

#define IS_INVALID(xf32) (((F32) (xf32)) > (INVALID_NUMBER-INVALID_NUMBER_VICINITY))

/*************************************************************/
/*      DATATYPES                                            */
/*************************************************************/

/**
 * @details general one dimensional distribution
 */
typedef struct STATE_1D_TAG
{
    F32     mean_f32;       /**<  state vector   */
    F32     variance_f32;   /**<  state variance */

} STATE_1D;

/**
 * @details general two dimensional distribution
 */
typedef struct STATE_2D_TAG
{
    F32     mean_vf32[2U];                          /**<  state vector     */
    F32     covariance_vf32[NR_ELEMENTS_SYM(2U)];   /**<  state covariance */

} STATE_2D;

/**
 * @details general measurement of two dimensional distribution
 */
typedef struct STATE_2D_MEAS_TAG
{
    BOOLEAN is_valid_b;                             /**< is measurement valid */
    F32     sample_vf32[2U];                        /**<  state vector        */
    F32     covariance_vf32[NR_ELEMENTS_SYM(2U)];   /**<  state covariance    */

}STATE_2D_MEAS;

/**
 * @details generic confidence bound with lower and upper limit
 */
typedef struct CONFIDENCE_BOUND_TAG
{
    F32 lower_limit_f32;
    F32 upper_limit_f32;
}CONFIDENCE_BOUND;

/**
 * @details generic trilean to indicate test result passed, failed or not available
 * (i.e. test is not performed because the test precondition is not fulfilled).
 */
typedef enum TEST_RESULT_TRILEAN_TYP
{
    TEST_RESULT_NOT_AVAILABLE,
    TEST_RESULT_PASSED,
    TEST_RESULT_FAILED,
} TEST_RESULT_TRILEAN;


/**
 * A ring structure that is optimized for calculating a moving average and
 * will cache max_cache_length_u8 values before actually adding them
 * to the buffer.
 * @sa ring_structure.h
 * @sa InitializeCachedRingBuffer
 * @sa InsertIntoCachedRingBuffer
 * @sa InitializeCachedRingBuffer
 * @sa ComputeCachedRingBufferMovingAverageAll
 */
typedef struct CACHED_RING_STRUCT_TAG
{
    RING_STRUCT background_buffer_s;
    U8          current_cache_length_u8;
    U8          max_cache_length_u8;
    F32         cache_f32;
    U8          current_length_u8;
} CACHED_RING_STRUCT;

#endif /* UTIL_TYPES_H_*/
