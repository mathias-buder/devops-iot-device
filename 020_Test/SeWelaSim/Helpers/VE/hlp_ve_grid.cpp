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

#define HLP_VE_ELLIPSE_BKPS     50U                                     /**< @details Number of brake points to paint ellipse */

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
F32 hlp_ve_ellipse_x_f32[HLP_VE_ELLIPSE_BKPS + 1U];
F32 hlp_ve_ellipse_y_f32[HLP_VE_ELLIPSE_BKPS + 1U];
F32 angle_res_f32;/**< @details Angular resolution for each HLP_VE_ELLIPSE_BKPS */
F32 angle_f32;


/*********************************************************************/
/*   STATIC FUNCTION PROTOTYPES                                      */
/*********************************************************************/
static void hlp_ve_draw_ellipse( void );


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void hlp_ve_init( void )
{

}

void hlp_ve_main( void )
{
    angle_res_f32 = ( ( 2.0F * PI ) / HLP_VE_ELLIPSE_BKPS );

    hlp_ve_draw_ellipse();
}

static void hlp_ve_draw_ellipse( void )
{
    U8 idx_u8;

    F32 rx_f32   = ve_grid.get_virtual_point().rx_f32;
    F32 ry_f32   = ve_grid.get_virtual_point().ry_f32;
    F32 offset_x = ve_grid.get_virtual_point().point_s.x_f32;
    F32 offset_y = ve_grid.get_virtual_point().point_s.y_f32;

    for ( idx_u8 = 0U; idx_u8 <= HLP_VE_ELLIPSE_BKPS; ++idx_u8 )
    {
        hlp_ve_ellipse_x_f32[idx_u8] = offset_x + ( rx_f32 * cosf( angle_f32 ) );
        hlp_ve_ellipse_y_f32[idx_u8] = offset_y + ( ry_f32 * sinf( angle_f32 ) );

        angle_f32 += angle_res_f32;
    }
}
