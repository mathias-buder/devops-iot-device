/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_mcpwm.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#include "dd_mcpwm.h"
#include "dd_database.h"

#include "../../types.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE BOOLEAN dd_mcpwm_update_channel( void );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

BOOLEAN dd_mcpwm_init( void )
{
    U8        idx_u8, idy_u8;
    esp_err_t error_t = ESP_OK;

    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Initializing ..." );

    /* Assign pointer to global channel configuration */
    dd_mcpwm_data_s.p_channel_s = &dd_mcpwm_channel_cfg_vs[0U];

    /* Initialize all GPIOs to be multiplexed to the corresponding PWM channel */
    for ( idx_u8 = 0U; idx_u8 < DD_MCPWM_CHANNEL_SIZE; ++idx_u8 )
    {
        error_t = mcpwm_gpio_init( dd_mcpwm_channel_cfg_vs[idx_u8].unit_e,
                                   dd_mcpwm_channel_cfg_vs[idx_u8].io_signal_e,
                                   dd_mcpwm_channel_cfg_vs[idx_u8].io_pin_e );

        if ( ESP_OK != error_t )
        {
            ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Couldn't initialize all GPIOs to be multiplexed to the corresponding PWM channel: %s", esp_err_to_name( error_t ) );
            return FALSE;
        }
    }

    /* Initialize all timers with global configuration dd_mcpwm_timer_cfg_s */
    for ( idx_u8 = 0U; idx_u8 < MCPWM_UNIT_MAX; ++idx_u8 )
    {
        for ( idy_u8 = 0U; idy_u8 < MCPWM_TIMER_MAX; ++idy_u8 )
        {
            error_t = mcpwm_init( (mcpwm_unit_t) idx_u8, (mcpwm_timer_t) idy_u8, &dd_mcpwm_timer_cfg_s );

            if ( ESP_OK != error_t )
            {
                ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Couldn't initialize timers with global configuration: %s", esp_err_to_name( error_t ) );
                return FALSE;
            }
        }
    }

    /* Initialize all PWM channels according to configuration in dd_mcpwm_channel_cfg_vs */
    if( FALSE == dd_mcpwm_update_channel() )
    {
        ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Couldn't initialize PWM channels according to global configuration dd_mcpwm_channel_cfg_vs." );
        return FALSE;
    }

    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Done" );

    return TRUE;
}


void dd_mcpwm_main( void )
{

    /* Continuously update all PWM channels according to global configuration dd_mcpwm_channel_cfg_vs */
    dd_mcpwm_update_channel();

}



PRIVATE BOOLEAN dd_mcpwm_update_channel( void )
{

    U8        channel_idx_u8;
    esp_err_t error_t = ESP_OK;

    for ( channel_idx_u8 = 0U; channel_idx_u8 < DD_MCPWM_CHANNEL_SIZE; ++channel_idx_u8 )
    {
            switch ( dd_mcpwm_channel_cfg_vs[channel_idx_u8].mode_e )
            {

            case DD_MCPWM_MODE_OFF:
                /* Set corresponding I/O pin to permanent low-level */
                error_t = mcpwm_set_signal_low( dd_mcpwm_channel_cfg_vs[channel_idx_u8].unit_e,
                                                dd_mcpwm_channel_cfg_vs[channel_idx_u8].timer_e,
                                                dd_mcpwm_channel_cfg_vs[channel_idx_u8].operator_e );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                break;

            case DD_MCPWM_MODE_ON:
                /* Set corresponding I/O pin to permanent high-level */
                error_t = mcpwm_set_signal_high( dd_mcpwm_channel_cfg_vs[channel_idx_u8].unit_e,
                                                 dd_mcpwm_channel_cfg_vs[channel_idx_u8].timer_e,
                                                 dd_mcpwm_channel_cfg_vs[channel_idx_u8].operator_e );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                break;

            case DD_MCPWM_MODE_PWM:
                /* Update corresponding PWM settings */
                error_t = mcpwm_set_duty( dd_mcpwm_channel_cfg_vs[channel_idx_u8].unit_e,
                                          dd_mcpwm_channel_cfg_vs[channel_idx_u8].timer_e,
                                          dd_mcpwm_channel_cfg_vs[channel_idx_u8].operator_e,
                                          dd_mcpwm_channel_cfg_vs[channel_idx_u8].duty_cycle_f32 );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                error_t = mcpwm_set_duty_type( dd_mcpwm_channel_cfg_vs[channel_idx_u8].unit_e,
                                               dd_mcpwm_channel_cfg_vs[channel_idx_u8].timer_e,
                                               dd_mcpwm_channel_cfg_vs[channel_idx_u8].operator_e,
                                               dd_mcpwm_timer_cfg_s.duty_mode );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                break;

            default:
                /* Should never happen */
                assert( dd_mcpwm_channel_cfg_vs[channel_idx_u8].mode_e == DD_MCPWM_MODE_OFF );
                assert( dd_mcpwm_channel_cfg_vs[channel_idx_u8].mode_e == DD_MCPWM_MODE_ON  );
                assert( dd_mcpwm_channel_cfg_vs[channel_idx_u8].mode_e == DD_MCPWM_MODE_PWM );

                break;
            }

            ESP_LOGD( DD_MCPWM_LOG_MSG_TAG, "Channel %i: mode: %i, unit %i, timer %i, operator %i, duty cycle: %0.2f, duty mode: %i", channel_idx_u8 + 1U,
                                                                                                                                      dd_mcpwm_channel_cfg_vs[channel_idx_u8].mode_e,
                                                                                                                                      dd_mcpwm_channel_cfg_vs[channel_idx_u8].unit_e,
                                                                                                                                      dd_mcpwm_channel_cfg_vs[channel_idx_u8].timer_e,
                                                                                                                                      dd_mcpwm_channel_cfg_vs[channel_idx_u8].operator_e,
                                                                                                                                      dd_mcpwm_channel_cfg_vs[channel_idx_u8].duty_cycle_f32,
                                                                                                                                      dd_mcpwm_timer_cfg_s.duty_mode );
    }

    return TRUE;
}
