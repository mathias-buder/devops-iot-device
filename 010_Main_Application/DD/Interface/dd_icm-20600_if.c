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

F32 dd_icm_20600_get_yaw(void)
{
    F32 yaw_f32 = 0.0F;

    yaw_f32 = atan2( 2.0F * (   ( dd_icm_20600_data_s.Quaternion_s.Q2_f32 * dd_icm_20600_data_s.Quaternion_s.Q3_f32 )
                              + ( dd_icm_20600_data_s.Quaternion_s.Q1_f32 * dd_icm_20600_data_s.Quaternion_s.Q4_f32 ) ),
                            (   ( dd_icm_20600_data_s.Quaternion_s.Q1_f32 * dd_icm_20600_data_s.Quaternion_s.Q1_f32 )
                              + ( dd_icm_20600_data_s.Quaternion_s.Q2_f32 * dd_icm_20600_data_s.Quaternion_s.Q2_f32 )
                              - ( dd_icm_20600_data_s.Quaternion_s.Q3_f32 * dd_icm_20600_data_s.Quaternion_s.Q3_f32 )
                              - ( dd_icm_20600_data_s.Quaternion_s.Q4_f32 * dd_icm_20600_data_s.Quaternion_s.Q4_f32 ) ) );

    return ( yaw_f32 * RAD_TO_DEG );
}

F32 dd_icm_20600_get_pitch(void)
{
    F32 pitch_f32 = 0.0F;

    pitch_f32 = -asin( 2.0F * (   ( dd_icm_20600_data_s.Quaternion_s.Q2_f32 * dd_icm_20600_data_s.Quaternion_s.Q4_f32 )
                                - ( dd_icm_20600_data_s.Quaternion_s.Q1_f32 * dd_icm_20600_data_s.Quaternion_s.Q3_f32 ) ) );

    return ( pitch_f32 * RAD_TO_DEG );
}

F32 dd_icm_20600_get_roll(void)
{
    F32 roll_f32 = 0.0F;

    roll_f32 = atan2( 2.0F * (   ( dd_icm_20600_data_s.Quaternion_s.Q1_f32 * dd_icm_20600_data_s.Quaternion_s.Q2_f32 )
                               + ( dd_icm_20600_data_s.Quaternion_s.Q3_f32 * dd_icm_20600_data_s.Quaternion_s.Q4_f32 ) ),
                             (   ( dd_icm_20600_data_s.Quaternion_s.Q1_f32 * dd_icm_20600_data_s.Quaternion_s.Q1_f32 )
                               + ( dd_icm_20600_data_s.Quaternion_s.Q1_f32 * dd_icm_20600_data_s.Quaternion_s.Q1_f32 )
                               - ( dd_icm_20600_data_s.Quaternion_s.Q2_f32 * dd_icm_20600_data_s.Quaternion_s.Q2_f32 )
                               - ( dd_icm_20600_data_s.Quaternion_s.Q4_f32 * dd_icm_20600_data_s.Quaternion_s.Q4_f32 ) ) );

    return ( roll_f32 * RAD_TO_DEG );
}


DD_ICM_20600_DATA* dd_icm_20600_get_database( void )
{
    return &dd_icm_20600_data_s;
}



