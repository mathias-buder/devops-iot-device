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
        @details Some detailed description

*********************************************************************/

/**********************************/
/* GLOBAL QAC WARNING SUPPRESSION */
/**********************************/

/*  DEVIATION:     0491
    DESCRIPTION:   Array subscripting applied to an object of pointer type.
    JUSTIFICATION: This file contains well used legacy code. The QAC help text (below) associated
    with this warning also implies that it is a question of company coding standards rather than
    outright 'bad' code:-

    "This is perfectly legitimate in the C language providing the pointer addresses
    an array element; but some coding standards recommend that if a parameter refers to an
    array, it should be declared as an array."  */
    /* PRQA S 0491 EOF*/

/*************************************************************/
/*      FILE DEFINITIONS                                     */
/*************************************************************/

#define SW_UTILS               /* defines this file*/
#define FOUNDATION_SUBSYSTEM   /* defines the subsystem that this file resides in*/

/*************************************************************/
/*      INCLUDE FILES                                        */
/*************************************************************/
#include "../types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/*************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                        */
/*************************************************************/

/*************************************************************/
/*      CODE                                                 */
/*************************************************************/


/****************************************************************************/
/*                                                                          */
/*  Function    : squaref                                                   */
/*                                                                          */
/*  Parameters  : x_f32 - input value                                       */
/*                                                                          */
/*  Returns     : F32   - the square of input value                         */
/*                                                                          */
/*  Description : Returns the square of floating point value                */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
F32 squaref( const F32 x_f32 )
{
    return ( x_f32 * x_f32 );
}


/****************************************************************************/
/*                                                                          */
/*  Function    : map_1d                                                    */
/*                                                                          */
/*  Parameters  : *mapin - A pointer to the specified map                   */
/*                 x     - input value to the map                           */
/*                                                                          */
/*  Returns     : F32  - the interpolated value o/p from the map           */
/*                                                                          */
/*  Description : Returns an interpolated floating point value from a 1D    */
/*                map with floating point variable breakpoints.             */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

F32 map_1d(const MAPTYPE *mapin, F32 x)
{
    F32 *data = (F32*)mapin->map_pts;
    F32 *axis = (F32*)mapin->x_bpoints;
    U32 num_pt = mapin->num_of_x_bpoints;
    U32 i;
    F32 return_value;

    if(x >= axis[num_pt-1U])     /* value is greater than max def. input*/
    {
        return_value = data[num_pt-1U]; /* output is output from max.x point*/
    }
    else
    {
        if(x <= axis[0])            /* value is below the minimum def.*/
        {
            return_value = data[0];        /* first of the output values*/
        }
        else
        {
            /* find the index of the neighboured xvalue*/
            i=num_pt-2U;
            while( x<axis[i] )
            {
                i--;
            }

            return_value = (data[i] + (((x - axis[i]) * (data[i+1U] - data[i])) /
                           (axis[i+1U] - axis[i])));
        }
    }

    return( return_value );
}


/****************************************************************************/
/*                                                                          */
/*  Function    : map_2d                                                    */
/*                                                                          */
/*  Parameters  : *mapin - A pointer to the specified map                   */
/*                 x     - input value to the map                           */
/*                 y     - input value to the map                           */
/*                                                                          */
/*  Returns     : F32  - the interpolated value o/p from the map           */
/*                                                                          */
/*  Description : Returns an interpolated floating point value from a 2D    */
/*                map with floating point variable breakpoints              */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
F32 map_2d(const MAPTYPE *mapin, F32 x, F32 y)
{
    U32 num_ptx = mapin->num_of_x_bpoints;
    U32 num_pty = mapin->num_of_y_bpoints;
    F32 *xval = (F32*)mapin->x_bpoints;
    F32 *yval = (F32*)mapin->y_bpoints;
    F32 *zval = (F32*)mapin->map_pts;
    U32 ix;
    U32 iy;
    F32 dy, dz1, dz2, dz3;
    F32 low_x, up_x, delta_y;

    if(x <= xval[0]) /* xvalue is minimum*/
    {
        x = xval[0];
        ix = 0;
    }
    else
    {
        if (x >= xval[num_ptx-1U])
        {
            x = xval[num_ptx-1U];
            ix = num_ptx-2U;
        }
        else
        {
            /* find the index of the neighboured xvalue*/
            ix=num_ptx-2U;
            while( x < xval[ix] )
            {
                ix--;
            }
        }
    }

    if(y <= yval[0]) /* yvalue is minimum*/
    {
        y = yval[0];
        iy = 0;
    }
    else
    {
        if (y >= yval[num_pty-1U])
        {
            y = yval[num_pty-1U];
            iy = num_pty-2U;
        }
        else
        {
            /* find the index of the neighboured xvalue*/
            iy=num_pty-2U;
            while( y < yval[iy] )
            {
                iy--;
            }
        }                                               /* iy is left-low from the input value*/
    }
    /* ok !*/

    dy = yval[iy+1U]-yval[iy];
    delta_y = y - yval[iy];        /* difference between actual value and next lower y value (positive)*/

    /* calc 1d lookup table in y for the x value below*/
    dz1   = zval[(ix*num_pty)+iy+1U] - zval[(ix*num_pty)+iy];
    low_x = (zval[(ix*num_pty)+iy] + ((delta_y*dz1)/dy));

    /* now the upper table*/
    dz2  = zval[((ix+1U)*num_pty)+iy+1U] - zval[((ix+1U)*num_pty)+iy];
    up_x = (zval[((ix+1U)*num_pty)+iy] + ((delta_y * dz2)/dy));

    /* interpolate between low_x and up_x :*/
    dz3 = up_x - low_x;

    return(low_x + ( ((x - xval[ix])*dz3) / (xval[ix+1U] - xval[ix]) ) );
}


/****************************************************************************/
/*                                                                          */
/*  Function    : convert_float_to_S32                                      */
/*                                                                          */
/*  Parameters  : value_f          F32 input value                          */
/*                inverse_resn_f   F32 1/resolution                         */
/*                offset_f         F32 applied offset value                 */
/*                                                                          */
/*  Returns     : S32  -           fixed point equivalent                   */
/*                                                                          */
/*  Description : Returns the fixed point equivalent of a floating point    */
/*                value with appropriate scaling and offset.                */
/*                                                                          */
/****************************************************************************/
S32  convert_float_to_S32( F32 value_f, F32 inverse_resn_f, F32 offset_f)
{
    F32 temp_var_f32;

    temp_var_f32 = (value_f - offset_f) * inverse_resn_f;

    /* Inc or dec by 0.5 prior to truncation */
    (temp_var_f32 >= 0.0F) ? (temp_var_f32 += 0.5F) : (temp_var_f32 -= 0.5F);

    return ((S32)temp_var_f32);
}

/****************************************************************************/
/*                                                                          */
/*  Function    : convert_float_to_U32                                      */
/*                                                                          */
/*  Parameters  : value_f          F32 input value                          */
/*                inverse_resn_f   F32 1/resolution                         */
/*                offset_f         F32 applied offset value                 */
/*                                                                          */
/*  Returns     : U32  -           fixed point equivalent                   */
/*                                                                          */
/*  Description : Returns the fixed point equivalent of a floating point    */
/*                value with appropriate scaling and offset.                */
/*                                                                          */
/****************************************************************************/
U32  convert_float_to_U32( F32 value_f, F32 inverse_resn_f, F32 offset_f)
{
    F32 temp_var_f32;

    temp_var_f32 = ((value_f - offset_f) * inverse_resn_f)+ 0.5F; /*Perform scaling and rounding*/

    if(temp_var_f32 < 0.0F)
    {
        temp_var_f32 = 0.0F;
    }

    return ((U32)temp_var_f32);
}

