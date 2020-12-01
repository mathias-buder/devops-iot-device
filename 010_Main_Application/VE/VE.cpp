/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file VE.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "VE.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
VE_DATA_OUT_TYPE VE_C::data_out_s;

VE_GRID_C ve_grid;


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void VE_C::init( void )
{
    ve_grid.init();
}


VE_DATA_OUT_TYPE VE_C::main( VE_DATA_IN_TYPE &r_data_in_s )
{
    ve_grid.main( r_data_in_s.ve_grid_data_in_s );

    return VE_C::data_out_s;
}


