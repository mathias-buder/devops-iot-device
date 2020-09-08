
/* PRQA S 0292 17 */
/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: ConfRadVid.c $
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

/**
 * @file
 * @brief general details of file
 */

/*************************************************************/
/*      FILE DEFINITIONS                                     */
/*************************************************************/

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "util_conf.h"
#include "util_gen.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*************************************************************/
/*      LOCAL VARIABLES                                      */
/*************************************************************/

/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

void util_update_fir_confidence( F32* const                      p_confidence_f32,
                                 F32* const                      p_confidence_max_f32,
                                 U64* const                      p_asso_history_u64,
                                 const U8                        history_length_u8,
                                 const UTIL_CONF_DETECTION_STATE detection_status_e )
{
    /* local variables*/
    U8  fir_index_u8;
    F32 current_fir_weight_f32;
    F32 sum_fir_weights_32;
    U8  fir_length_u8;

    /* length of ring buffer of FIR filter */
    fir_length_u8 = min( history_length_u8, UTIL_CONF_MAX_FIR_CONFIDENCE_BUFFER_LENGTH );

    if (    ( NULL != p_confidence_f32     )
         && ( NULL != p_confidence_max_f32 )
         && ( NULL != p_asso_history_u64   ) )
    {
        if ( detection_status_e > NO_UPDATE )
        {
            /* advance ring buffer */
            *p_asso_history_u64 = ( ( *p_asso_history_u64 ) << 1 );

            /* update FIR confidence*/
            if ( IS_DETECTION == detection_status_e )
            {
                /* set rightmost bit to one*/
                *p_asso_history_u64 = ( ( *p_asso_history_u64 ) | BIT_0 );
            }
            else
            {
                /* set rightmost bit to zero */
                /* Automatic? Has been padded with zero by left shift <<? */
                /* *p_asso_history_u64 = ( (*p_asso_history_u64) | 0U ); */
            }
        }
        else
        {
            /* no update */
        }

        /* now do the FIR filtering*/
        /* first set confidences to zero*/
        *p_confidence_f32 = 0.0F;

        /* initialize normalization */
        sum_fir_weights_32 = 0.0F;

        /* loop through all FIR_LENGTH bits*/
        for ( fir_index_u8 = 0; fir_index_u8 < fir_length_u8; fir_index_u8++ )
        {
            /* compute weight, e. g. linear */
            current_fir_weight_f32 = ( F32 )( fir_length_u8 - fir_index_u8 );
            /* compute weight, e. g. constant = association counter */
            /*current_FIR_weight_f32 = 1.0F;*/

            /* compute sum for normalization */
            sum_fir_weights_32 += current_fir_weight_f32;

            /* check only the bit under consideration: right shift to desired bit location and check this one bit,
               cast to U32 for bit operations with BIT0 since this is also U32
            */
            if ( BIT_0 == ( ( U32 )( ( *p_asso_history_u64 ) >> fir_index_u8 ) & BIT_0 ) )
            {
                *p_confidence_f32 += current_fir_weight_f32;
            }
            else
            {
                /* leave unchanged*/
            }
        }

        /* normalize confidences*/
        *p_confidence_f32 /= sum_fir_weights_32;

        /* compute maxima ... */
        if (    ( *p_confidence_f32 > *p_confidence_max_f32 )
             || ( *p_confidence_f32 < SMALL_NUMBER          ) )
        {
            *p_confidence_max_f32 = *p_confidence_f32;
        }
        else
        {
            /* leave unchanged*/
        }
    }
    else
    {
        /* do nothing */
        assert( NULL != p_confidence_f32     );
        assert( NULL != p_confidence_max_f32 );
        assert( NULL != p_asso_history_u64   );
    }
}
