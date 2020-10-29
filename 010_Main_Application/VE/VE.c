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

#include "Core/ve_database.h"
#include "Core/ve_grid.h"

#include "esp_log.h"

#include <stdio.h>
#include <string.h>


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
#define VE_LOG_MSG_TAG      "VE"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

void ve_init(void)
{
    ve_grid_init();
}

void ve_main(void)
{
    ve_grid_main();
}
