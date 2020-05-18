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

        @file dlg_log_if.c
        @details Some detailed description

*********************************************************************/

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dlg_log_if.h"

#include <stdio.h>
#include <string.h>

#include "../Core/dlg_database.h"
#include "../Core/dlg_log.h"

#include "../OS/os_time.h"
#include "../DD/DD.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/*************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                        */
/*************************************************************/

/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

void dlg_log_get_data( void )
{
    /* Acquire I2C error database pointer */
    p_dlg_i2c_error_data_s = dd_i2c_get_error_database();

    /* Acquire ICM-20600 database pointer */
    p_dlg_icm_20600_data_s = dd_icm_20600_get_database();

    /* Acquire MAX-30102 database pointer */
    p_dlg_max_30102_data_s = dd_max_30102_get_database();
}
