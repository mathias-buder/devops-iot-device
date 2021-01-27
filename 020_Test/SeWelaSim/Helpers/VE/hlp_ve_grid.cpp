/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file hlp_ve_grid.cpp
        @details Vibration grid module helper functions implementation

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "hlp_ve_grid.h"
#include <math.h>

#include "VE/VE.h"

#define HLP_VE_ELLIPSE_BKPS     50U


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
F32 hlp_ve_ellipse_x_f32[HLP_VE_ELLIPSE_BKPS + 1U];
F32 hlp_ve_ellipse_y_f32[HLP_VE_ELLIPSE_BKPS + 1U];
F32 a_f32 = 2.0F;
F32 b_f32 = 3.0F;
F32 angle_res_f32;
F32 angle_f32;

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void hlp_ve_init( void )
{
    U8 idx_u8;

    angle_f32 = 0.0F;
    angle_res_f32 = ( 2.0F * PI ) / HLP_VE_ELLIPSE_BKPS;

    for ( idx_u8 = 0U; idx_u8 <= HLP_VE_ELLIPSE_BKPS; ++idx_u8 )
    {
        hlp_ve_ellipse_x_f32[idx_u8] = ve_grid.virtual_point_s.size_x_f32 * cosf( angle_f32 );
        hlp_ve_ellipse_y_f32[idx_u8] = ve_grid.virtual_point_s.size_y_f32 * sinf( angle_f32 );

        angle_f32 += angle_res_f32;
    }

}



void hlp_ve_main( void )
{

    U8 idx_u8;

    angle_f32 = 0.0F;
    angle_res_f32 = ( 2.0F * PI ) / HLP_VE_ELLIPSE_BKPS;

    for ( idx_u8 = 0U; idx_u8 <= HLP_VE_ELLIPSE_BKPS; ++idx_u8 )
    {
        hlp_ve_ellipse_x_f32[idx_u8] = ve_grid.virtual_point_s.size_x_f32 * cosf( angle_f32 );
        hlp_ve_ellipse_y_f32[idx_u8] = ve_grid.virtual_point_s.size_y_f32 * sinf( angle_f32 );

        angle_f32 += angle_res_f32;
    }

}
