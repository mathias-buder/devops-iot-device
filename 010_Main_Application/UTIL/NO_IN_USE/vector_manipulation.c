/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: vector_manipulation.c $
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
 * @file vector_manipulation.c
 * @brief This file summarizes functions to perform vector manipulations.
 *        We use INVALID_NUMBER instead of NAN, because the cross-compiler has problems with NAN.
 *        Note, that then NAN is mentioned in comments below, INVALID_NUMBER is meant.
 */


/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <assert.h>
#ifdef simulator
    #include <stdio.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <mathf.h>

#include "../FOUNDATION/FOUNDATION.h"
#include "../UTIL/util_types.h"
#include "../UTIL/vector_manipulation.h"


/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/


void assertFloat(F32 value_f32, F32 reference_f32, F32 allowered_delta_f32)
{

    ASSERT( (value_f32 <= reference_f32 + allowered_delta_f32) && (value_f32 >= (reference_f32 - allowered_delta_f32)) )

}


void assertOneBitSet(U16 bitfield_u16)
{
#ifdef simulator
    BOOLEAN exactly_one_bit_set_b = FALSE;
    switch(bitfield_u16)
    {
        case BIT0:
        case BIT1:
        case BIT2:
        case BIT3:
        case BIT4:
        case BIT5:
        case BIT6:
        case BIT7:
        case BIT8:
        case BIT9:
        case BIT10:
        case BIT11:
        case BIT12:
        case BIT13:
        case BIT14:
        case BIT15:
        {
            exactly_one_bit_set_b = TRUE;
            break;
        }
        default:
        {
            exactly_one_bit_set_b = FALSE;
            break;
        }
    }
    ASSERT( exactly_one_bit_set_b )
#endif
}


F32 sprod(const F32 a_vf32[], const F32 b_vf32[], U16 vector_length_u16, U16* const p_nr_NaNs_u16)
{
    register F32 sum_f32 = 0.0F;
    U16 index_u16;
    U16 NaN_counter_u16 = 0U;

    if((NULL != a_vf32)
        && (NULL != b_vf32) )
    {
        for (index_u16 = 0; index_u16 < vector_length_u16; index_u16++)
        {

            if (!isINVALID(a_vf32[index_u16])) /*  both are not nan at this point (see assertion) */
            {
                sum_f32+= a_vf32[index_u16] * b_vf32[index_u16];
            }
            else
            {
                NaN_counter_u16++;
            }

            if ( !( ( isINVALID(a_vf32[index_u16]) && isINVALID(b_vf32[index_u16]) ) || ( ( !isINVALID(a_vf32[index_u16]) ) && ( !isINVALID(b_vf32[index_u16] ) ) ) ) ) /*  isINVALID(a) <=> isINVALID(b) */
            {
                ASSERT(FALSE) /* Impossible vector combination */
            }
        }
    }
    else
    {
        /*  do nothing */
        ASSERT(FALSE)
    }


    if ( NULL != p_nr_NaNs_u16)
    {
        (*p_nr_NaNs_u16) = NaN_counter_u16;
#ifdef simulator
#ifndef PSIL_TEST_ENABLE
        if ( (*p_nr_NaNs_u16) == vector_length_u16)
        {
            printf("Warning in function sprod(..): Only NaN's in vector, returning 0");
        }
#endif
#endif
    }
    return (F32) sum_f32;
}


void remove_NaN(F32 a_vf32[], U16* const p_vector_length_u16)
{
    U16 search_index_u16;      /*  the index in the "old" vector */
    U16 vector_index_u16 = 0U; /*  the index in the new vector we are at */
    U16 new_length_u16   = 0U; /*  the new length of a */

    if((NULL != p_vector_length_u16)
        && (NULL != a_vf32) )
    {
        for (search_index_u16 = 0; search_index_u16 < (*p_vector_length_u16); search_index_u16++)
        {
            if (!isINVALID(a_vf32[search_index_u16]))
            {
                a_vf32[vector_index_u16] = a_vf32[search_index_u16];
                new_length_u16++;
                vector_index_u16++;
            }
        }

        (*p_vector_length_u16) = new_length_u16; /*  assign new length to vector */
    }
    else
    {
        /*  do nothing */
        ASSERT(FALSE)
    }

}


F32 lin_interpol(const F32 x_f32, const F32 x1_f32, const F32 y1_f32, const F32 x2_f32, const F32 y2_f32)
{
    F32 m_f32;
    F32 t_f32;

    if ((x2_f32 - x1_f32) < 0.000001F)
    {
        m_f32 = 0.0F;
    }
    else
    {
        m_f32 = (y2_f32 - y1_f32) / (x2_f32 - x1_f32);
    }
    t_f32 = y1_f32 - ( m_f32 * x1_f32 );

    return ( (m_f32 * x_f32) + t_f32);
}


F32 linear_interpolation( const F32 x_f32, const F32 x1_f32, const F32 y1_f32, const F32 x2_f32, const F32 y2_f32, const BOOLEAN is_constant_extrapolation_b )
{
    /* locals */
    F32 slope_f32;
    F32 y_interpol_or_extrapol_f32;

    /* check for non-singular slope */
    if ( fabsf( x2_f32 - x1_f32 ) < SMALL_NUMBER )
    {
        slope_f32 = 0.0F;
    }
    else
    {
        slope_f32 = ( y2_f32 - y1_f32 ) / ( x2_f32 - x1_f32 );
    }

    /* get interpolated or extrapolated value */
    y_interpol_or_extrapol_f32 = y1_f32 + ( slope_f32 * ( x_f32 - x1_f32 ) );

    /* */
    if ( TRUE == is_constant_extrapolation_b )
    {
        y_interpol_or_extrapol_f32 = CLAMP( y_interpol_or_extrapol_f32, fminf( y1_f32, y2_f32 ), fmaxf( y1_f32, y2_f32 ) );
    }
    else
    {
        /* do nothing */
        /* y_interpol_or_extrapol_f32 = y_interpol_or_extrapol_f32; */
    }

    return ( y_interpol_or_extrapol_f32 );
}


void lin_interpol_v(F32 y_vf32[], const F32 x_vf32[], const U8 length_x_u8, const F32 x1_vf32[], const F32 y1_vf32[], const U8 length_input_u8)
{
    U8 output_iterator_u8;
    U8 input_iterator_u8;

    U8 interpolating_upper_index_u8;

    if(  (NULL != y_vf32)
       &&(NULL != x_vf32)
       &&(NULL != x1_vf32)
       &&(NULL != y1_vf32) )
    {
        if ( length_input_u8 > 1U )
        {
            /* for every point x*/
            for (output_iterator_u8 = 0; output_iterator_u8 < length_x_u8; output_iterator_u8++)
            {
                for (input_iterator_u8 = 1U; input_iterator_u8 < length_input_u8; input_iterator_u8++)
                {
                    interpolating_upper_index_u8 = input_iterator_u8;

                    if (x_vf32[output_iterator_u8] < x1_vf32[interpolating_upper_index_u8])
                    {
                        break;
                    }
                }

                ASSERT(interpolating_upper_index_u8 > 0)
                ASSERT(interpolating_upper_index_u8 < length_input_u8)

                y_vf32[output_iterator_u8] = lin_interpol(x_vf32[output_iterator_u8], x1_vf32[interpolating_upper_index_u8-1U], y1_vf32[interpolating_upper_index_u8-1U], x1_vf32[interpolating_upper_index_u8], y1_vf32[interpolating_upper_index_u8]);
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT( (NULL != y_vf32) && (NULL != x_vf32) && (NULL != x1_vf32) && (NULL != y1_vf32) )
    }
}


F32 median(const F32* const a_vf32, const U16 vector_length_u16)
{
    F32 result;

    if(NULL != a_vf32)
    {
        result = prctile(a_vf32, vector_length_u16, 50.0F);
    }
    else
    {
        result = INVALID_NUMBER;

        ASSERT(NULL != a_vf32)
    }

    return result;
}



F32 iqr(const F32* const a_vf32, const U16 vector_length_u16)
{
    F32 result;

    if(NULL != a_vf32)
    {
        result = (prctile(a_vf32, vector_length_u16, 75.0F) - prctile(a_vf32, vector_length_u16, 25.0F));
    }
    else
    {
        result = INVALID_NUMBER;

        ASSERT(NULL != a_vf32)
    }
    return result;
}


F32 calc_prctile(const U16 vector_length_u16, const U16 index_u16)
{
    return ( (100.0F * (((F32) index_u16) + 0.5F)) / ((F32) vector_length_u16) ); /*  + 0.5 instead of - 0.5  because of matlab indexing vs c indexing */
}


F32 prctile(const F32 a_vf32[], const U16 vector_length_u16, F32 percentile_f32)
{
    S32 index_s32;
    F32 index_right_f32; /*  e.g., if index_right == 3.44 => index = 3 */

    F32 prctile_lower_f32;
    F32 prctile_upper_f32;

    F32 result_f32;

    if (    (NULL != a_vf32)
         && (0 < vector_length_u16))
    {
        /*  the next two can be derived from the matlab way of creating the percentiles */
        index_right_f32  = ((((float) vector_length_u16) / 100.0F) * percentile_f32) - 0.5F;

        index_s32       = (S32) floorf(index_right_f32); /*  maybe some wrong results, e.g if true value 1.9999999 evaluates 2.00001 */
                                           /*  then, the wrong index, thus the wrong slope will be used for interpolation */
                                           /*  should not be too frequent nor too severe in any case */

        /*  extrema */
        if (index_s32 < 0L)
        {
            result_f32 = a_vf32[0];
        }
        else
        {
            if (index_s32 >= (S32)(vector_length_u16 - 1U))
            {
                result_f32 = a_vf32[vector_length_u16 - 1U];
            }
            else
            {
                prctile_lower_f32 = calc_prctile(vector_length_u16, (U16) index_s32);
                prctile_upper_f32 = calc_prctile(vector_length_u16, (U16) index_s32 + 1U);

                /*  normal cases */
                result_f32 = lin_interpol(percentile_f32, prctile_lower_f32, a_vf32[(U16) index_s32], prctile_upper_f32, a_vf32[(U16) index_s32 + 1U]);
            }
        }
    }
    /*  input vector empty? (e.g. case then only NaNs in vector) */
    else
    {
        result_f32 = INVALID_NUMBER;

        ASSERT(FALSE)
    }

    return result_f32;
}


F32 Overlap_Relative_To_Second_Interval( const F32 interval_a_start_f32, const F32 interval_a_end_f32, const F32 interval_b_start_f32, const F32 interval_b_end_f32 )
{
    /*
    The obvious method to find the intersection c of intervals a and b is the formula
    c.start = max(a.start, b.start)
    c.end   = min(a.end, b.end)
    with the caveat that there is no intersection if c.end <= c.start.
    */

    /* locals*/
    F32 interval_c_start_f32;
    F32 interval_c_end_f32;
    F32 relative_interval_c_f32;


    /* get overlap start (right) and end (left) points in tan coordinates*/
    interval_c_start_f32    = fmaxf( interval_a_start_f32, interval_b_start_f32 );
    interval_c_end_f32      = fminf( interval_a_end_f32,   interval_b_end_f32   );

    /* now compute relative overlap*/
    if (   (interval_c_end_f32 <= interval_c_start_f32)                         /* no overlap*/
        || (interval_a_end_f32 <= interval_a_start_f32)                         /* negative or zero interval a*/
        || ( ( interval_b_end_f32 - interval_b_start_f32 ) < SMALL_NUMBER )     /* interval b too small or negative*/
        )
    {
        /* no overlap, set to zero*/
        relative_interval_c_f32 = 0.0F;
    }
    else
    {
        /* compute relative overlap*/
        /* note: if interval b is larger than interval a then the overlap will be < 1.0, e. g. if vehicle width*/
        /* is larger than lane width and the vehicle entirely overlaps the lane, the overlap will be < 1.0!*/
        relative_interval_c_f32 = ( interval_c_end_f32 - interval_c_start_f32 )
                                    / ( interval_b_end_f32 - interval_b_start_f32 );
    }

    return relative_interval_c_f32;
}


F32 min_v(const F32 a_vf32[], U16 vector_length_u16)
{
    U16 i_u16;
    F32 min_value_f32 = BIG_NUMBER;

    if(NULL != a_vf32)
    {
        for  ( i_u16 = 0; i_u16 < vector_length_u16; i_u16++ )
        {
            if ((!isINVALID(a_vf32[i_u16])) && (min_value_f32 > a_vf32[i_u16]))
            {
                min_value_f32 = a_vf32[i_u16];
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != a_vf32)
    }

    return min_value_f32;
}


F32 max_v(const F32 a_vf32[], U16 vector_length_u16)
{
    U16 i_u16;
    F32 max_element_f32 = 0.0F;

    if(NULL != a_vf32)
    {
        for  ( i_u16 = 0; i_u16 < vector_length_u16; i_u16++ )
        {
            if ( (!isINVALID(a_vf32[i_u16])) && (max_element_f32 < a_vf32[i_u16]))
            {
                max_element_f32 = a_vf32[i_u16];
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != a_vf32)
    }

    return max_element_f32;
}


F32 mean(const F32 a_vf32[], U16 vector_length_u16)
{
    register F32 sum_f32 = 0.0F;
    U16 i_u16;
    U16 nr_NaN_u16;
    F32 result_f32 = 0.0F;

    if(NULL != a_vf32)
    {
        nr_NaN_u16 = 0U;

        for ( i_u16 = 0; i_u16 < vector_length_u16; i_u16++ )
        {
            if (!isINVALID(a_vf32[i_u16]))
            {
                sum_f32 += a_vf32[i_u16];
            }
            else
            {
                nr_NaN_u16++;
            }
        }

        if ( nr_NaN_u16 == vector_length_u16)
        {
#ifdef simulator
            printf("attention: vector for mean evaluation contains only invalid values, returning 0 for mean");
#endif
            result_f32 = 0.0F;
        }
        else
        {
            result_f32 = sum_f32 / ((F32)( vector_length_u16 - nr_NaN_u16 ));
        }
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != a_vf32)
    }
    return result_f32;
}


U16 nr_invalid(const F32 a_vf32[], U16 vector_length_u16)
{
    U16 i_u16;
    U16 sum_invalid_u16 = 0U;

    if(NULL != a_vf32)
    {
        for ( i_u16 = 0; i_u16 < vector_length_u16; i_u16++ )
        {
            if (isINVALID(a_vf32[i_u16]))
            {
                sum_invalid_u16++;
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != a_vf32)
    }
    return sum_invalid_u16;
}


void center_vector(F32 a_vf32[], U16 vector_length_u16)
{
    F32 mean_a_f32;
    U16 i_u16;

    if(NULL != a_vf32)
    {
        mean_a_f32 = mean(a_vf32, vector_length_u16);

        for ( i_u16 = 0; i_u16 < vector_length_u16; i_u16++ )
        {
            if (!isINVALID(a_vf32[i_u16]))
            {
                a_vf32[i_u16] = a_vf32[i_u16] - mean_a_f32;
            }
        }
    }
    else
    {
        /* do nothing */
        ASSERT(NULL != a_vf32)
    }
}


F32 cov_vec(const F32* const a_vf32, const F32* const b_vf32, U16 vector_length_u16)
{
    U16 nr_NaNs_u16;
    F32 result_f32 = 0.0F;
    F32 cov_ab;

    if((NULL != a_vf32)
       && (NULL != b_vf32) )
    {
        cov_ab = sprod(a_vf32, b_vf32, vector_length_u16, &nr_NaNs_u16);

        if (nr_NaNs_u16 > 25U)
        {
#ifdef simulator
            printf("many nans for mirror effect covariance computation, setting cov to 0");
#endif
            result_f32 = 0.0F;
        }
        else
        {
            if ( vector_length_u16 > 1U)
            {
                cov_ab = cov_ab / ((F32)(vector_length_u16 - nr_NaNs_u16 - 1U)); /* correction factor -1 due to sample mean (see literature, e.g.,*/
            /*   http://de.wikipedia.org/wiki/Korrigierte_Stichprobenvarianz#Erwartungstreue_Sch.C3.A4tzung_der_Varianz_der_Grundgesamtheit )*/
            }
            result_f32 = cov_ab;
        }
    }
    else
    {
        /* do nothing */
        ASSERT(FALSE)
    }

    return result_f32;
}

