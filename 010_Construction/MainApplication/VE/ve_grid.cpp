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

        /* vp_size_x : */ 10.0F,
        /* vp_size_y : */ 5.0F
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
    this->virtual_point_s.size_x_f32 = r_config_s.vp_size_x_f32;
    this->virtual_point_s.size_y_f32 = r_config_s.vp_size_y_f32;

    return &this->data_out_s;
}

void VE_GRID_C::main( VE_GRID_DATA_IN_TYPE& r_data_in_s )
{
    U8  idx_u8;
    F32 dx_f32, dy_f32;

    /* Set virtual vibration point */
    // this->virtual_point_s = r_data_in_s.virtual_point_s;

    /* Calculate euclidean distance to each vibrator */
    for ( idx_u8 = 0U; idx_u8 < VE_GRID_VIBRATOR_SIZE; ++idx_u8 )
    {
        dx_f32 = this->virtual_point_s.x_f32 - this->vibrator_vs[idx_u8].position_s.x_f32;
        dy_f32 = this->virtual_point_s.y_f32 - this->vibrator_vs[idx_u8].position_s.y_f32;

        this->virtual_point_s.dist_to_vib_vf32[idx_u8] = sqrtf( SQUARE( dx_f32 ) + SQUARE( dy_f32 ) );
    }

    this->virtual_point_s.size_x_f32 += 0.01F;
    this->virtual_point_s.size_y_f32 -= 0.01F;
}


