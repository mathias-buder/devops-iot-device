/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/OSL_AEB/Interface/project.pj $
        $RCSfile: osl_aeb_input_interface.c $
        $Revision: 1.1 $
        $Date: 2019/08/13 11:19:37CEST $

        TRW Ltd owns the copyright in this document and associated
        documents and all rights are reserved.  These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of TRW
        Ltd. Any copy of this drawing or document made by any method
        must also include a copy of this legend.

    $CopyrightDate: (c) ZF 2019 $

*********************************************************************/

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/

#include "osl_aeb_helper.h"

#include "../../MainApplication/OSL_AEB/OSL_AEB.h"
#include "../../MainApplication/OSL_AEB/Core/osl_aeb_types.h"
#include "../../MainApplication/OSL_AEB/Core/osl_aeb_database.h"
#include "../../MainApplication/UTIL/UTIL.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

F32 dist_y_at_TTX0_vf32[OSL_AEB_MAX_NUMBER_OBJECTS][DIST_Y_AT_TTX0_BKPT];
F32 dist_y_at_TTX0_bkpt_vf32[DIST_Y_AT_TTX0_BKPT];

/*************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                        */
/*************************************************************/


/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

void osl_aeb_helper_compute_dist_y_at_TTX0( OSL_AEB_OBJECT* p_current_osl_aeb_object_s,
                                            F32*            p_dist_y_at_TTX0_vf32 )
{
    U16 idx_u16;

    F32 variance_f32 = 0.0F;
    F32 mean_f32     = 0.0F;
    F32 offset_x_f32 = 0.0F;

    if(    ( NULL != p_current_osl_aeb_object_s )
        && ( NULL != p_dist_y_at_TTX0_vf32      ) )
    {
        /* Get object properties */
        variance_f32 = p_current_osl_aeb_object_s->mco_data_s.y_at_TTX0_state_s.variance_f32;
        mean_f32     = p_current_osl_aeb_object_s->mco_data_s.y_at_TTX0_state_s.mean_f32;
        // offset_x_f32 = Transformation_XCoordinate( p_current_osl_aeb_object_s->p_input_object_s->Kalman_state_s.mean_vf32[INDEX_X_REL], CS_REAR_AXLE, CS_FRONT_BUMPER );
        offset_x_f32 = p_current_osl_aeb_object_s->p_input_object_s->Kalman_state_s.mean_vf32[INDEX_X_REL];

        /* Calculate distribution values */
        for(idx_u16 = 0; idx_u16 < DIST_Y_AT_TTX0_BKPT; idx_u16++)
        {
            p_dist_y_at_TTX0_vf32[idx_u16] = offset_x_f32 + ( 1.0F / sqrtf(2.0F * PI * variance_f32) ) * exp(-1.0F * ( pow( dist_y_at_TTX0_bkpt_vf32[idx_u16] - mean_f32 , 2 ) / ( 2.0F * variance_f32 ) ) );
        }
    }
}


void osl_aeb_helper_init(void)
{
    U16 idx_u16 = 0U;

    /* Generate break points */
    for( idx_u16 = 0U; idx_u16 < DIST_Y_AT_TTX0_BKPT; idx_u16++ )
    {
        dist_y_at_TTX0_bkpt_vf32[idx_u16] = DIST_Y_AT_TTX0_BKPT_OFFSET + ( idx_u16 * DIST_Y_AT_TTX0_STEP_SIZE );
    }
}


void osl_aeb_helper_main(void)
{
    U8 idx_u8;

    /* Reset distribution values */
    memset( &dist_y_at_TTX0_vf32, 0.0F, sizeof(dist_y_at_TTX0_vf32) );

    /* Loop through all available environment objects */
    for( idx_u8 = 0U; idx_u8 < osl_aeb_environment_s.number_objects_u8; idx_u8++ )
    {
        /* Calculate probability density function */
        osl_aeb_helper_compute_dist_y_at_TTX0( &osl_aeb_environment_s.object_vs[idx_u8],
                                               &dist_y_at_TTX0_vf32[idx_u8][0] );
    }
}



