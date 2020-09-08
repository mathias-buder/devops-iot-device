
/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: queue.h $
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

#ifndef QUEUE_H
#define QUEUE_H

/**
 * @brief Struct type describing a queue containing U8 data.
 * @invariant length_u8 <= max_length_u8
 * @invariant length_u8 == ((index_next_free_u8 - index_front_u8) % max_length_u8 + max_length_u8) % max_length_u8
 * @invariant nnz(buffer_vu8) <= length_u8
 */
typedef struct U8_QUEUE_TAG
{
    /** @privatesection */
    U8* buffer_vu8;         /**< @brief the buffer storing the actual data. */
    U8 index_front_u8;      /**< @brief the index of the front element - this element will be dequeued next. */
    U8 index_next_free_u8;  /**< @brief index of the next free slot in the buffer, next enqueue will be added here */
    U8 max_length_u8;       /**< @brief the maximal length of the queue (equal to the size of the buffer) */
    U8 length_u8;           /**< @brief the current length of the queue */
} U8_QUEUE;

/**
 * @brief Initializes a new U8_QUEUE.
 * @memberof U8_QUEUE_TAG
 * @param [in,out] p_queue_s    the pointer to the U8_QUEUE to be initialized.
 * @param [in] buffer_vu8       the U8 array to be used in p_queue_s.
 * @param [in] max_length_u8    the length of buffer_vu8.
 * @internal
 * @param [out] p_queue_s->buffer_vu8
 * @param [out] p_queue_s->index_front_u8
 * @param [out] p_queue_s->index_next_free_u8
 * @param [out] p_queue_s->max_length_u8
 * @param [out] p_queue_s->length_u8
 */
void init_u8_queue(U8_QUEUE* const p_queue_s, U8 buffer_vu8[], const U8 max_length_u8);

/**
 * @brief Removes and returns the front element of the given U8_QUEUE.
 * @details Removes and returns the front element of a given U8_QUEUE if
 * the length is at least 1. Returns 0 and does not remove anything, otherwise.
 * @memberof U8_QUEUE_TAG
 * @param [in,out] p_queue_s the pointer to the U8_QUEUE.
 * @return the front element of p_queue_s.
 * @internal
 * @param [in,out] p_queue_s->buffer_vu8[]
 * @param [in,out] index_front_u8
 * @param [in] p_queue_s->max_length_u8
 * @param [in,out] p_queue_s->length_u8
 */
U8   dequeue_u8_queue(U8_QUEUE* const p_queue_s);

/**
 * @brief Adds an element to the end of a given U8_QUEUE.
 * @details Adds an element to the end of a given U8_QUEUE if the current
 * length is less than the maximal length. Does not add the element, otherwise.
 * @memberof U8_QUEUE_TAG
 *
 * @param [in] p_queue_s  the pointer to the U8_QUEUE.
 * @param [in] element_u8 the element to be added to p_queue_s.
 * @internal
 * @param [in,out] p_queue_s->buffer_vu8[]
 * @param [in,out] index_next_free_u8
 * @param [in] p_queue_s->max_length_u8
 * @param [in,out] p_queue_s->length_u8
 */
void enqueue_u8_queue(U8_QUEUE* const p_queue_s, U8 element_u8);

/**
 * @brief Returns the length of a given U8_QUEUE.
 * @memberof U8_QUEUE_TAG
 * @param [in] p_queue_s
 * @return the length of p_queue_s
 *
 * @internal
 * @param [in] p_queue_s->length_u8
 */
U8   length_u8_queue(const U8_QUEUE* const p_queue_s);

#endif /* QUEUE_H */
