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

#include "../../VE/VE.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE void dd_mcpwm_if_wrap_ve_grid( VE_GRID_VIRBATOR_TYPE* p_vibrator_config_s );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
BOOLEAN dd_mcpwm_if_init( void )
{
    return TRUE;
}

void dd_mcpwm_if_main( void )
{
    VE_GRID_VIRBATOR_TYPE* p_vibrator_config_s = ve_get_vibrator_config();

    dd_mcpwm_if_wrap_ve_grid( p_vibrator_config_s );
}

PRIVATE void dd_mcpwm_if_wrap_ve_grid( VE_GRID_VIRBATOR_TYPE* p_vibrator_config_s )
{
    U8               idx_u8;
    DD_MCPWM_CHANNEL pwm_channel_e = DD_MCPWM_CHANNEL_SIZE; /* Initialize with invalid value */

    if ( NULL != p_vibrator_config_s )
    {
        /* Loop over all vibrators */
        for ( idx_u8 = 0U; idx_u8 < VE_GRID_VIBRATOR_SIZE; ++idx_u8 )
        {
            /* Get corresponding PWM channel */
            pwm_channel_e = p_vibrator_config_s[idx_u8].assignment_e;

            if ( DD_MCPWM_CHANNEL_SIZE > pwm_channel_e )
            {
                dd_mcpwm_data_s.p_channel_s[pwm_channel_e].duty_cycle_f32 = p_vibrator_config_s[idx_u8].duty_cycle_f32;
            }
            else
            {
                assert( DD_MCPWM_CHANNEL_SIZE > pwm_channel_e );
            }
        }
    }
    else
    {
        assert( NULL != p_vibrator_config_s );
    }
}

DD_MCPWM_DATA* dd_mcpwm_get_database( void )
{
    return &dd_mcpwm_data_s;
}

BOOLEAN dd_mcpwm_set_duty_cycle( DD_MCPWM_CHANNEL channel_e,
                                 F32              duty_cycle_f32 )
{
    if (    ( DD_MCPWM_CHANNEL_SIZE   > channel_e )
         && ( 0.0F                    <= duty_cycle_f32 )
         && ( DD_MCPWM_MAX_DUTY_CYCLE >= duty_cycle_f32 ) )
    {
        /* Update corresponding duty cycle in case all values are in range */
        dd_mcpwm_data_s.p_channel_s[channel_e].duty_cycle_f32 = duty_cycle_f32;

        return TRUE;
    }

    return FALSE;
}

F32 dd_mcpwm_get_duty_cycle( DD_MCPWM_CHANNEL channel_e )
{
    F32 duty_cycle_f32 = BIG_NUMBER;

    if ( DD_MCPWM_CHANNEL_SIZE > channel_e )
    {
        /* Set corresponding duty cycle in case all values are in range */
        duty_cycle_f32 = dd_mcpwm_data_s.p_channel_s[channel_e].duty_cycle_f32;
    }

    return duty_cycle_f32;
}

BOOLEAN dd_mcpwm_set_mode( DD_MCPWM_CHANNEL channel_e,
                           DD_MCPWM_MODE    mode_e )
{
    if ( ( DD_MCPWM_CHANNEL_SIZE > channel_e )
         && ( DD_MCPWM_MODE_SIZE > mode_e ) )
    {
        /* Update corresponding channel mode in case all values are in range */
        dd_mcpwm_data_s.p_channel_s[channel_e].mode_e = mode_e;

        return TRUE;
    }

    return FALSE;
}

DD_MCPWM_MODE dd_mcpwm_get_mode( DD_MCPWM_CHANNEL channel_e )
{
    DD_MCPWM_MODE mode_e = DD_MCPWM_MODE_SIZE;

    if ( DD_MCPWM_CHANNEL_SIZE > channel_e )
    {
        /* Set corresponding channel mode in case all values are in range */
        mode_e = dd_mcpwm_data_s.p_channel_s[channel_e].mode_e;
    }

    return mode_e;
}

