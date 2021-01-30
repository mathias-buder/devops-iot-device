/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file ve_grid.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <math.h>
#include <string.h>

#include "ve_grid.h"
#include "../UTIL/UTIL.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
VE_GRID_C::VE_GRID_C( void )
{
}

VE_GRID_C::~VE_GRID_C()
{
}

VE_GRID_DATA_OUT_TYPE* VE_GRID_C::init( void )
{
    /* Default unit configuration */
    VE_GRID_CONFIG_TYPE default_cfg_s = {
        /* vibrator_vs */
        {                 /*  x [mm]   y [mm]   */
         /* VINRATOR 1  */ {  0.0F,    0.0F  },
         /* VINRATOR 2  */ {  11.3F,   10.0F },
         /* VINRATOR 3  */ { -11.3F,   10.0F },
         /* VINRATOR 4  */ {  0.0F,    16.4F },
         /* VINRATOR 5  */ {  5.3F,    26.0F },
         /* VINRATOR 6  */ { -5.3F,    26.0F },
         /* VINRATOR 7  */ {  0.0F,    40.0F },
         /* VINRATOR 8  */ { -11.0F,  -8.0F  },
         /* VINRATOR 9  */ {  11.0F,  -8.0F  },
         /* VINRATOR 10 */ {  0.0F,   -16.8F },
         /* VINRATOR 11 */ {  0.0F,   -30.0F }
        },

        /* vp_size_x [mm] : */ 10.0F,
        /* vp_size_y [mm] : */ 5.0F
    };

    return init( default_cfg_s );
}

VE_GRID_DATA_OUT_TYPE* VE_GRID_C::init( VE_GRID_CONFIG_TYPE& r_config_s )
{
    U8 idx_u8;

    for ( idx_u8 = 0U; idx_u8 < VE_GRID_VIBRATOR_SIZE; ++idx_u8 )
    {
        this->vibrator_vs[idx_u8].position_s         = r_config_s.vibrator_pos_vs[idx_u8];
        this->vibrator_vs[idx_u8].pwm_duty_cycle_f32 = 0.0F;
    }

    /* Set default virtual point size */
    this->virtual_point_props_s.rx_f32 = r_config_s.vp_size_x_f32;
    this->virtual_point_props_s.ry_f32 = r_config_s.vp_size_y_f32;

    return &this->data_out_s;
}

void VE_GRID_C::main( VE_GRID_DATA_IN_TYPE& r_data_in_s )
{
    U8      idx_u8;
    F32     dx_square_f32, dy_square_f32;

    /* Set virtual vibration point */
    // this->virtual_point_s = r_data_in_s.virtual_point_s;

    /* TODO: Remove test implementation to update ellipse size per cycle */
    this->virtual_point_props_s.rx_f32 -= 0.025F;
    this->virtual_point_props_s.ry_f32 += 1.0F;

    for ( idx_u8 = 0U; idx_u8 < VE_GRID_VIBRATOR_SIZE; ++idx_u8 )
    {
        this->vibrator_vs[idx_u8].dist_to_vp_f32 = compute_euclidean_distance( this->vibrator_vs[idx_u8].position_s,
                                                                               this->virtual_point_props_s.point_s.position_s,
                                                                               dx_square_f32,
                                                                               dy_square_f32 );

        this->vibrator_vs[idx_u8].is_inside_act_area_b = check_point_inisde_ellipse( dx_square_f32,
                                                                                     dy_square_f32,
                                                                                     this->virtual_point_props_s.rx_f32,
                                                                                     this->virtual_point_props_s.ry_f32 );
    }

    /* Compute distance from center of ellipse to each vibrator and
     * normalize to size in this direction */
}

F32 VE_GRID_C::compute_euclidean_distance( POINT_2D_TYPE& r_vib_pos_s,
                                           POINT_2D_TYPE& r_vp_pos_s,
                                           F32&           r_dx_square_f32,
                                           F32&           r_dy_square_f32 )
{

    F32 dx_f32, dy_f32;

    /* Compute euclidean distance to each vibrator */
    dx_f32 = r_vib_pos_s.x_f32 - r_vp_pos_s.x_f32;
    dy_f32 = r_vib_pos_s.y_f32 - r_vp_pos_s.y_f32;

    r_dx_square_f32 = SQUARE( dx_f32 );
    r_dy_square_f32 = SQUARE( dy_f32 );

    return ( sqrtf( r_dx_square_f32 + r_dy_square_f32 ) );
}

BOOLEAN VE_GRID_C::check_point_inisde_ellipse( F32 dx_square_f32,
                                               F32 dy_square_f32,
                                               F32 rx_f32,
                                               F32 ry_f32 )
{
    F32     rxry_square_f32;
    F32     distance_f32;
    BOOLEAN is_inside_b = FALSE;

     /*
     * Equation / Condition (https://math.stackexchange.com/questions/76457/check-if-a-point-is-within-an-ellipse):
     *
     * ry^2 * ( x - h )^2 + r_x^2 * ( y - k )^2 <= r_x^2 * r_y^2
     *
     * rx : ellipse radius in x
     * ry : ellipse radius in y
     * x  : x-coordinate of poin to check
     * y  : y-coordinate of point to check
     * h  : x-coordinate of ellipse origin
     * k  : y-coordinate of ellipse origin
     * */

    rxry_square_f32 = SQUARE( rx_f32 * ry_f32 );
    distance_f32    = ( SQUARE( ry_f32 ) * dx_square_f32 ) + ( SQUARE( rx_f32 ) * dy_square_f32 );

    if ( distance_f32 <= rxry_square_f32 )
    {
        is_inside_b = TRUE;
    }

    return is_inside_b;
}


/*********************************************************************/
/*   GET/SET FUNCTION DEFINITIONS                                    */
/*********************************************************************/

VE_GRID_VIRTUAL_POINT_PROPS_TYPE& VE_GRID_C::get_virtual_point( void )
{
    return this->virtual_point_props_s;
}


