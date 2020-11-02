/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_i2c_if.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_i2c_if.h"

#include <stdio.h>
#include <string.h>

#include "../Core/dd_database.h"
#include "../Core/dd_i2c.h"

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
DD_I2C_ERROR_INFO_TYPE* dd_i2c_get_last_error( void )
{
    DD_I2C_ERROR_INFO_TYPE* p_error_info_s = NULL;

    if ( DD_I2C_ERROR_BUFFER_LENGTH > dd_i2c_error_s.last_error_idx_u8 )
    {
        p_error_info_s = &dd_i2c_error_s.error_info_vs[dd_i2c_error_s.last_error_idx_u8];
    }
    else
    {
        assert ( DD_I2C_ERROR_BUFFER_LENGTH > dd_i2c_error_s.last_error_idx_u8 );
    }

    return p_error_info_s;
}


DD_I2C_ERROR_DATA_TYPE* dd_i2c_get_error_database( void )
{
    return &dd_i2c_error_s;
}
