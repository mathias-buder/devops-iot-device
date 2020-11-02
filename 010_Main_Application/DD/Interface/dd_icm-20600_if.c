/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_icm-20600_if.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_icm-20600_if.h"

#include <stdio.h>
#include <math.h>

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

DD_ICM_20600_DATA_TYPE* dd_icm_20600_get_database( void )
{
    return &dd_icm_20600_data_s;
}

BOOLEAN dd_icm_20600_get_accl_raw_data( S16*                    p_data_s16,
                                        DD_ICM_20600_ACCEL_TYPE type_e )
{
    if (    ( NULL != p_data_s16 )
         && ( DD_ICM_20600_ACCEL_SIZE > type_e ) )
    {
        *p_data_s16 = dd_icm_20600_data_s.accel_data_raw_s16[type_e];
        return TRUE;
    }
    else
    {
        assert( NULL != p_data_s16 );
        assert( DD_ICM_20600_ACCEL_SIZE > type_e );
    }

    return FALSE;
}

BOOLEAN dd_icm_20600_get_gyro_raw_data( S16*                   p_data_s16,
                                        DD_ICM_20600_GYRO_TYPE type_e )
{
    if (    ( NULL != p_data_s16 )
         && ( DD_ICM_20600_GYRO_SIZE > type_e ) )
    {
        *p_data_s16 = dd_icm_20600_data_s.gyro_data_raw_s16[type_e];
        return TRUE;
    }
    else
    {
        assert( NULL != p_data_s16 );
        assert( DD_ICM_20600_GYRO_SIZE > type_e );
    }

    return FALSE;
}
