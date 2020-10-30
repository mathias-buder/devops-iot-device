/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file ve_grid_if.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <math.h>

#include "../Core/ve_database.h"
#include "ve_grid_if.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

VE_GRID_VIRBATOR_TYPE* ve_get_vibrator_config( void )
{
    return ve_grid_data_s.p_vibrator_config_s;
}
