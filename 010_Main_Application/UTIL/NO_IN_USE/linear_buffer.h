/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: linear_buffer.h $
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

#ifndef LINEAR_BUFFER_H
#define LINEAR_BUFFER_H

/**
 * @details This file defines operations on the
 *
 *           LINEAR BUFFER:
 *           -> expensive to push and pop elements: O(n)
 *           -> easy to access elements: O(1)
 *           -> easy to access the full array:  O(1)
 */


/*************************************************************/
/*      GLOBAL DATATYPES                                     */
/*************************************************************/

/**
 * @details   Struct type describing a linear data buffer containing F32 data.
 *            This structure just manages a data storage (F32-array),
 *            it does not store the data by itself!
 * @invariant current_length_u8 <= max_length_u8
 *
 *
 */
typedef struct LINEAR_BUFFER_TAG
{
    F32 *buffer_vf32;        /**< pointer to the first element of data array */
    U8   current_length_u8;  /**< current length of linear buffer            */
    U8   max_length_u8;      /**< the maximal length of the buffer           */
} F32_LINEAR_BUFFER;



/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/

/**
 * @details initializes a linear buffer
 * @memberof F32_LINEAR_BUFFER
 * @param[out] 'p_linear_buffer_s' resulting initialized linear buffer
 * @param[in]  'buffer_vf32'       data storage, will be managed by 'p_linear_buffer_s'.
 *                                 Array of F32 with at least 'max_length_u8' entries!
 * @param[in]  'max_length_u8'     length of the buffer_vf32
 */
void       linearBuffer_f32_initialize( F32_LINEAR_BUFFER* const p_linear_buffer_s, F32* buffer_vf32, U8 max_length_u8 );

/**
 * @details get a value at a certain position from a linear buffer
 * @memberof F32_LINEAR_BUFFER
 * @param[in] 'p_linear_buffer_s'
 * @param[in] 'element_index_u8'  index of requested element
 * @return    value of requested element
 */
F32        linearBuffer_f32_get( const F32_LINEAR_BUFFER* const p_linear_buffer_s, const U8 element_index_u8 );

/**
 * @details get array with the complete buffer data
 * @memberof F32_LINEAR_BUFFER
 * @param[in]  'p_linear_buffer_s'
 * @param[out] 'p_length_u8' current length of data buffer
 * @return     pointer to first element of buffer array
 */
const F32* linearBuffer_f32_getArray( const F32_LINEAR_BUFFER* const p_linear_buffer_s, U8* p_length_u8 );

/**
 * @details set a value at a certain position at a linear buffer
 * @memberof F32_LINEAR_BUFFER
 * @param[out] 'p_linear_buffer_s'
 * @param[in]  'element_index_u8'  index of element to set
 * @param[in]  'value_f32' value to set
 */
void       linearBuffer_f32_set( F32_LINEAR_BUFFER* const p_linear_buffer_s, const U8 element_index_u8, const F32 value_f32 );

/**
 * @details resets a linear buffer
 * @memberof F32_LINEAR_BUFFER
 * @param[out] 'p_linear_buffer_s' linear buffer to be reset
 */
void       linearBuffer_f32_reset( F32_LINEAR_BUFFER* const p_linear_buffer_s );

/**
 * @details push a value at the front of a linear buffer
 *          - shifts all already stored data physically, to preserve a well ordered data array.
 *          - if the buffer is already full, the last element will be deleted
 * @memberof F32_LINEAR_BUFFER
 * @param[out] 'p_linear_buffer_s'
 * @param[in]  'current_value_f32' value to push
 */
void       linearBuffer_f32_push( F32_LINEAR_BUFFER* const p_linear_buffer_s, const F32 current_value_f32 );

/**
 * @details removes the first value of a linear buffer
 *          - shifts all already stored data physically, to preserve a well ordered data array.
 * @memberof F32_LINEAR_BUFFER
 * @param[out] 'p_linear_buffer_s'
 * @param[out] 'p_popped_element_f32' first element, that will be removed from buffer
 * @return     FALSE if buffer is already empty
 */
BOOLEAN    linearBuffer_f32_pop( F32_LINEAR_BUFFER* const p_linear_buffer_s, F32* const p_popped_element_f32 );



#endif /*LINEAR_BUFFER_H */
