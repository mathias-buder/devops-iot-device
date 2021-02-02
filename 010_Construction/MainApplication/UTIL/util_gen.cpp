/*********************************************************************
    MODULE SPECIFICATION:

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file util_gen.cpp
        @details Contains the implementation of the general
                 utility functionality.

*********************************************************************/

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "util_gen.h"

#include "../types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/*************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                        */
/*************************************************************/

/*************************************************************/
/*      FUNCTION DEFINITION                                  */
/*************************************************************/
F32 util_squaref( const F32 x_f32 )
{
    return ( x_f32 * x_f32 );
}


F32 util_map_1d( const MAPTYPE* p_mapin_s, F32 x_f32 )
{
    F32* p_data_f32 = (F32*) p_mapin_s->map_pts;
    F32* p_axis_f32 = (F32*) p_mapin_s->x_bpoints;
    U32  num_pt_u32 = p_mapin_s->num_of_x_bpoints;
    U32  i_u32;
    F32  return_value_f32;

    if ( x_f32 >= p_axis_f32[num_pt_u32 - 1U] ) /* value is greater than max def. input*/
    {
        return_value_f32 = p_data_f32[num_pt_u32 - 1U]; /* output is output from max.x point*/
    }
    else
    {
        if ( x_f32 <= p_axis_f32[0] ) /* value is below the minimum def.*/
        {
            return_value_f32 = p_data_f32[0]; /* first of the output values*/
        }
        else
        {
            /* find the index of the neighboured xvalue*/
            i_u32 = num_pt_u32 - 2U;
            while ( x_f32 < p_axis_f32[i_u32] )
            {
                i_u32--;
            }

            return_value_f32 = ( p_data_f32[i_u32] + ( ( ( x_f32 - p_axis_f32[i_u32] ) * ( p_data_f32[i_u32 + 1U] - p_data_f32[i_u32] ) ) / ( p_axis_f32[i_u32 + 1U] - p_axis_f32[i_u32] ) ) );
        }
    }

    return ( return_value_f32 );
}


F32 util_map_2d( const MAPTYPE* p_mapin_s, F32 x_f32, F32 y_f32 )
{
    U32  num_ptx_u32 = p_mapin_s->num_of_x_bpoints;
    U32  num_pty_u32 = p_mapin_s->num_of_y_bpoints;
    F32* p_xval_f32  = (F32*) p_mapin_s->x_bpoints;
    F32* p_yval_f32  = (F32*) p_mapin_s->y_bpoints;
    F32* p_zval_f32  = (F32*) p_mapin_s->map_pts;
    U32  ix_u32, iy_u32;
    F32  dy, dz1, dz2, dz3;
    F32  low_x, up_x, delta_y;

    /* xvalue is minimum */
    if ( x_f32 <= p_xval_f32[0] )
    {
        x_f32  = p_xval_f32[0];
        ix_u32 = 0;
    }
    else
    {
        if ( x_f32 >= p_xval_f32[num_ptx_u32 - 1U] )
        {
            x_f32  = p_xval_f32[num_ptx_u32 - 1U];
            ix_u32 = num_ptx_u32 - 2U;
        }
        else
        {
            /* find the index of the neighboured xvalue*/
            ix_u32 = num_ptx_u32 - 2U;
            while ( x_f32 < p_xval_f32[ix_u32] )
            {
                ix_u32--;
            }
        }
    }

    /* yvalue is minimum */
    if ( y_f32 <= p_yval_f32[0] )
    {
        y_f32  = p_yval_f32[0];
        iy_u32 = 0;
    }
    else
    {
        if ( y_f32 >= p_yval_f32[num_pty_u32 - 1U] )
        {
            y_f32  = p_yval_f32[num_pty_u32 - 1U];
            iy_u32 = num_pty_u32 - 2U;
        }
        else
        {
            /* find the index of the neighboured xvalue*/
            iy_u32 = num_pty_u32 - 2U;
            while ( y_f32 < p_yval_f32[iy_u32] )
            {
                iy_u32--;
            }
        } /* iy is left-low from the input value*/
    }
    /* ok !*/

    dy = p_yval_f32[iy_u32 + 1U] - p_yval_f32[iy_u32];

    /* difference between actual value and next lower y value (positive) */
    delta_y = y_f32 - p_yval_f32[iy_u32];

    /* calc 1d lookup table in y for the x value below*/
    dz1   = p_zval_f32[( ix_u32 * num_pty_u32 ) + iy_u32 + 1U] - p_zval_f32[( ix_u32 * num_pty_u32 ) + iy_u32];
    low_x = ( p_zval_f32[( ix_u32 * num_pty_u32 ) + iy_u32] + ( ( delta_y * dz1 ) / dy ) );

    /* now the upper table*/
    dz2  = p_zval_f32[( ( ix_u32 + 1U ) * num_pty_u32 ) + iy_u32 + 1U] - p_zval_f32[( ( ix_u32 + 1U ) * num_pty_u32 ) + iy_u32];
    up_x = ( p_zval_f32[( ( ix_u32 + 1U ) * num_pty_u32 ) + iy_u32] + ( ( delta_y * dz2 ) / dy ) );

    /* interpolate between low_x and up_x :*/
    dz3 = up_x - low_x;

    return ( low_x + ( ( ( x_f32 - p_xval_f32[ix_u32] ) * dz3 ) / ( p_xval_f32[ix_u32 + 1U] - p_xval_f32[ix_u32] ) ) );
}


S32 util_convert_float_to_S32( F32 value_f32, F32 inverse_resn_f32, F32 offset_f32 )
{
    F32 temp_var_f32;

    temp_var_f32 = ( value_f32 - offset_f32 ) * inverse_resn_f32;

    /* Inc or dec by 0.5 prior to truncation */
    ( temp_var_f32 >= 0.0F ) ? ( temp_var_f32 += 0.5F ) : ( temp_var_f32 -= 0.5F );

    return ( (S32) temp_var_f32 );
}


U32 util_convert_float_to_U32( F32 value_f32, F32 inverse_resn_f32, F32 offset_f32 )
{
    F32 temp_var_f32;

    /* Perform scaling and rounding */
    temp_var_f32 = ( ( value_f32 - offset_f32 ) * inverse_resn_f32 ) + 0.5F;

    if ( temp_var_f32 < 0.0F )
    {
        temp_var_f32 = 0.0F;
    }

    return ( (U32) temp_var_f32 );
}


void util_rotate_point_2d( POINT_2D_TYPE& r_point_s,
                           F32            angle_deg_f32 )
{
    F32 x_f32         = r_point_s.x_f32;
    F32 y_f32         = r_point_s.y_f32;
    F32 angle_rad_f32 = angle_deg_f32 * DEG_TO_RAD;

    /* x´ = x * cos(angle) - y * sin(angle) */
    r_point_s.x_f32 = ( x_f32 * cosf( angle_rad_f32 ) )
                      - ( y_f32 * sinf( angle_rad_f32 ) );

    /* y´ = x * sin(angle) + y * cos(angle) */
    r_point_s.y_f32 = ( x_f32 * sinf( angle_rad_f32 ) )
                      + ( y_f32 * cosf( angle_rad_f32 ) );
}
