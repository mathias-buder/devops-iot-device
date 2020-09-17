/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_mcpwm_if.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_mcpwm_if.h"

#include <stdio.h>
#include <string.h>

#include "../Core/dd_database.h"
#include "../Core/dd_mcpwm.h"

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
BOOLEAN dd_mcpwm_set_duty_cycle( DD_MCPWM_CHANNEL_NUM channel_e,
                                 F32                  duty_cycle_f32 )
{
    if (    ( DD_MCPWM_CHANNEL_SIZE   > channel_e       )
         && ( 0.0F                    <= duty_cycle_f32 )
         && ( DD_MCPWM_MAX_DUTY_CYCLE >= duty_cycle_f32 ) )
    {
        /* Update corresponding duty cycle in case all values are in range */
        dd_mcpwm_data_s.p_channel_s[channel_e].duty_cycle_f32 = duty_cycle_f32;

        return TRUE;
    }

    return FALSE;
}


F32 dd_mcpwm_get_duty_cycle( DD_MCPWM_CHANNEL_NUM channel_e )
{
    F32 duty_cycle_f32 = BIG_NUMBER;

    if ( DD_MCPWM_CHANNEL_SIZE > channel_e )
    {
        /* Set corresponding duty cycle in case all values are in range */
        duty_cycle_f32 = dd_mcpwm_data_s.p_channel_s[channel_e].duty_cycle_f32;
    }

    return duty_cycle_f32;
}


BOOLEAN dd_mcpwm_set_mode( DD_MCPWM_CHANNEL_NUM channel_e,
                           DD_MCPWM_MODE        mode_e )
{
    if (    ( DD_MCPWM_CHANNEL_SIZE > channel_e )
         && ( DD_MCPWM_MODE_SIZE    > mode_e    ) )
    {
        /* Update corresponding channel mode in case all values are in range */
        dd_mcpwm_data_s.p_channel_s[channel_e].mode_e = mode_e;

        return TRUE;
    }

    return FALSE;
}


DD_MCPWM_MODE dd_mcpwm_get_mode( DD_MCPWM_CHANNEL_NUM channel_e )
{
    DD_MCPWM_MODE mode_e = DD_MCPWM_MODE_SIZE;

    if ( DD_MCPWM_CHANNEL_SIZE > channel_e )
    {
        /* Set corresponding channel mode in case all values are in range */
        mode_e = dd_mcpwm_data_s.p_channel_s[channel_e].mode_e;
    }

    return mode_e;
}
