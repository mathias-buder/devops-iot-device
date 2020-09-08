/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: linear_buffer.c $
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

/*************************************************************/
/*      INCLUDE FILES                                        */
/*************************************************************/

#include <string.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../UTIL/linear_buffer.h"
#include "../UTIL/util_types.h"


/*************************************************************/
/*      CODE                                                 */
/*************************************************************/


void linearBuffer_f32_initialize(F32_LINEAR_BUFFER* const p_linear_buffer_s, F32 * buffer_vf32, U8 max_length_u8)
{
    if(    (NULL != p_linear_buffer_s)
        && (NULL != buffer_vf32))
    {
        p_linear_buffer_s->buffer_vf32        = buffer_vf32;
        p_linear_buffer_s->max_length_u8      = max_length_u8;
        p_linear_buffer_s->current_length_u8  = 0;
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}


void linearBuffer_f32_reset( F32_LINEAR_BUFFER* const p_linear_buffer_s)
{
    if(NULL != p_linear_buffer_s)
    {
        p_linear_buffer_s->current_length_u8  = 0;
    }
    else
    {
        /*  do nothing */
        ASSERT(NULL != p_linear_buffer_s)
    }
}


void linearBuffer_f32_push(F32_LINEAR_BUFFER* const p_linear_buffer_s, const F32 current_value_f32)
{
    if( NULL != p_linear_buffer_s)
    {

        if( p_linear_buffer_s->current_length_u8 == p_linear_buffer_s->max_length_u8)
        {
            /* buffer full, forget the last element */
            p_linear_buffer_s->current_length_u8--;
        }
        else
        {
            /* do nothing */
        }

        /* shift elements */
        (void)memmove(&p_linear_buffer_s->buffer_vf32[1U], &p_linear_buffer_s->buffer_vf32[0U], sizeof(F32)*p_linear_buffer_s->current_length_u8 );

        /* insert first element */
        p_linear_buffer_s->buffer_vf32[0] = current_value_f32;
        p_linear_buffer_s->current_length_u8++;
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != p_linear_buffer_s)
    }
}


BOOLEAN linearBuffer_f32_pop(F32_LINEAR_BUFFER* const p_linear_buffer_s, F32* const p_popped_element_f32)
{
    BOOLEAN successful_b;

    if(   ( NULL != p_linear_buffer_s)
       && (p_linear_buffer_s->current_length_u8 > 0))
    {
        /* return the popped element */
        if(NULL != p_popped_element_f32)
        {
            *p_popped_element_f32 = p_linear_buffer_s->buffer_vf32[0];
        }
        else
        {
            /* optional output not requested */
        }

        /* decrement size */
        p_linear_buffer_s->current_length_u8--;
        successful_b = TRUE;

        /* shift elements */
        (void)memmove(&p_linear_buffer_s->buffer_vf32[0U], &p_linear_buffer_s->buffer_vf32[1U], sizeof(F32)*p_linear_buffer_s->current_length_u8 );
    }
    else
    {
        /* buffer empty */
        successful_b = FALSE;

        ASSERT(NULL != p_linear_buffer_s)
    }

    return successful_b;
}


F32 linearBuffer_f32_get(const F32_LINEAR_BUFFER* const p_linear_buffer_s, const U8 element_index_u8)
{
    F32 element_value_f32;

    if(   ( NULL != p_linear_buffer_s )
       && ( element_index_u8 < p_linear_buffer_s->max_length_u8 ))
    {
        element_value_f32 = p_linear_buffer_s->buffer_vf32[element_index_u8];
    }
    else
    {
        element_value_f32 = INVALID_NUMBER;

        ASSERT(FALSE)
    }

    return element_value_f32;
}


void linearBuffer_f32_set( F32_LINEAR_BUFFER* const p_linear_buffer_s, const U8 element_index_u8, const F32 value_f32)
{
    if(   (NULL != p_linear_buffer_s)
       && (element_index_u8 < p_linear_buffer_s->max_length_u8))
    {
        p_linear_buffer_s->buffer_vf32[element_index_u8] = value_f32;
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }
}


const F32* linearBuffer_f32_getArray( const F32_LINEAR_BUFFER* const p_linear_buffer_s, U8* p_length_u8 )
{
    U8         length_u8;
    const F32* array_vf32;

    if( NULL != p_linear_buffer_s)
    {
        array_vf32 = p_linear_buffer_s->buffer_vf32;
        length_u8  = p_linear_buffer_s->current_length_u8;
    }
    else
    {
        array_vf32 = NULL;
        length_u8  = 0U;

        ASSERT(NULL != p_linear_buffer_s)
    }

    if (NULL != p_length_u8)
    {
        *p_length_u8 = length_u8;
    }
    else
    {
        /* optional output not requested */
    }

    return array_vf32;
}
