/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_max-30102_if.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>

#include "dd_max-30102_if.h"
#include "../Core/dd_database.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/



/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
DD_MAX_30102_DATA* dd_max_30102_get_database( void )
{
    return &dd_max_30102_data_s;
}



