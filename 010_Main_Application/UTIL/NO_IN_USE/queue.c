
/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: queue.c $
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
    JUSTIFICATION: The QAC help text (below) associated
    with this warning also implies that it is a question of company coding standards rather than
    outright 'bad' code:-

    "This is perfectly legitimate in the C language providing the pointer addresses
    an array element; but some coding standards recommend that if a parameter refers to an
    array, it should be declared as an array."  */
    /* PRQA S 0491 EOF*/

#include <mathf.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../UTIL/queue.h"

void init_u8_queue(U8_QUEUE* const p_queue_s, U8 buffer_vu8[], const U8 max_length_u8)
{
    if (NULL != p_queue_s)
    {
        p_queue_s->buffer_vu8 = buffer_vu8;
        p_queue_s->max_length_u8 = max_length_u8;
        p_queue_s->index_front_u8 = 0;
        p_queue_s->index_next_free_u8 = 0;
        p_queue_s->length_u8 = 0;
    }
    else
    {
        ASSERT(NULL != p_queue_s)
    }
}

U8 dequeue_u8_queue(U8_QUEUE* const p_queue_s)
{
    U8 element_u8;

    if (    (NULL != p_queue_s)
         && (p_queue_s->length_u8 > 0))
    {
        element_u8 = p_queue_s->buffer_vu8[p_queue_s->index_front_u8];

        /* overwrite removed element with 0 (not actually needed but facilitates consistency checks)*/
        p_queue_s->buffer_vu8[p_queue_s->index_front_u8] = 0U;

        p_queue_s->index_front_u8 = (p_queue_s->index_front_u8 + 1) % p_queue_s->max_length_u8;
        p_queue_s->length_u8--;
    }
    else
    {
        /* underflow: return default value */
        element_u8 = 0U;
        ASSERT(NULL != p_queue_s)
    }

    return element_u8;
}

void enqueue_u8_queue(U8_QUEUE* const p_queue_s, U8 element_u8)
{
    if (    (NULL != p_queue_s)
         && (p_queue_s->length_u8 < p_queue_s->max_length_u8))
    {
        p_queue_s->buffer_vu8[p_queue_s->index_next_free_u8] = element_u8;
        p_queue_s->index_next_free_u8 = (p_queue_s->index_next_free_u8 + 1) % p_queue_s->max_length_u8;
        p_queue_s->length_u8++;
    }
    else
    {
        /* overflow: do not add element */
        ASSERT(NULL != p_queue_s)
    }
}

U8 length_u8_queue(const U8_QUEUE* const p_queue_s)
{
    U8 queue_length_u8 = 0U;

    if (NULL != p_queue_s)
    {
        queue_length_u8 = p_queue_s->length_u8;
    }
    else
    {
        ASSERT(NULL != p_queue_s)
    }

    return queue_length_u8;
}
