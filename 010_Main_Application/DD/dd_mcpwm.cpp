/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_mcpwm.c
        @details IMCPW device driver implementation

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_err.h"
#include "soc/mcpwm_periph.h"

#include "dd_mcpwm.h"


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
DD_MCPWM_C::DD_MCPWM_C( void )
{

}


DD_MCPWM_C::~DD_MCPWM_C()
{

}


BOOLEAN DD_MCPWM_C::init( const DD_MCPWM_CONFIG_TYPE &r_config_s )
{
    esp_err_t error_t = ESP_OK;

    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Initializing channel %i / %i", r_config_s.name_e + 1U, DD_MCPWM_CHANNEL_SIZE );

    /* Initialize corresponding GPIO to be multiplexed to the corresponding PWM channel */
    error_t = mcpwm_gpio_init( r_config_s.unit_e,
                               r_config_s.io_signal_e,
                               r_config_s.io_pin_e );

    if ( ESP_OK != error_t )
    {
        ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Couldn't initialize GPIO %o to be multiplexed to the corresponding PWM channel: %s", r_config_s.io_pin_e, esp_err_to_name( error_t ) );
        return FALSE;
    }

    /* Initialize corresponding timers with global configuration timer_cfg_s */
    error_t = mcpwm_init( r_config_s.unit_e, r_config_s.timer_e, &this->m_timer_cfg_s );

    if ( ESP_OK != error_t )
    {
        ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Couldn't initialize timers with global configuration: %s", esp_err_to_name( error_t ) );
        return FALSE;
    }


    switch ( r_config_s.mode_e )
    {

    case DD_MCPWM_MODE_OFF:
        /* Set corresponding I/O pin to permanent low-level */
        error_t = mcpwm_set_signal_low( r_config_s.unit_e,
                                        r_config_s.timer_e,
                                        r_config_s.operator_e );

        if ( ESP_OK != error_t )
        {
            ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
            return FALSE;
        }

        break;

    case DD_MCPWM_MODE_ON:
        /* Set corresponding I/O pin to permanent high-level */
        error_t = mcpwm_set_signal_high( r_config_s.unit_e,
                                         r_config_s.timer_e,
                                         r_config_s.operator_e );

        if ( ESP_OK != error_t )
        {
            ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
            return FALSE;
        }

        break;

    case DD_MCPWM_MODE_PWM:
        /* Update corresponding PWM settings */
        error_t = mcpwm_set_duty( r_config_s.unit_e,
                                  r_config_s.timer_e,
                                  r_config_s.operator_e,
                                  r_config_s.duty_cycle_f32 );

        if ( ESP_OK != error_t )
        {
            ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
            return FALSE;
        }

        error_t = mcpwm_set_duty_type( r_config_s.unit_e,
                                       r_config_s.timer_e,
                                       r_config_s.operator_e,
                                       this->m_timer_cfg_s.duty_mode );

        if ( ESP_OK != error_t )
        {
            ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
            return FALSE;
        }

        break;

    default:
        /* Should never happen */
        assert( r_config_s.mode_e == DD_MCPWM_MODE_OFF );
        assert( r_config_s.mode_e == DD_MCPWM_MODE_ON  );
        assert( r_config_s.mode_e == DD_MCPWM_MODE_PWM );

        return FALSE;
    }

    /* Store channel configuration */
    this->m_channel_s = r_config_s;

    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Done" );
    return TRUE;
}


BOOLEAN DD_MCPWM_C::update( DD_MCPWM_DATA_IN_TYPE &r_data_in_s )
{

    esp_err_t error_t = ESP_OK;

    switch ( r_data_in_s.mode_e )
    {

    case DD_MCPWM_MODE_OFF:
        /* Set corresponding I/O pin to permanent low-level */
        error_t = mcpwm_set_signal_low( this->m_channel_s.unit_e,
                                        this->m_channel_s.timer_e,
                                        this->m_channel_s.operator_e );

        if ( ESP_OK != error_t )
        {
            ESP_LOGE( DD_MCPWM_LOG_MSG_TAG,
                      "Can't update channel-%i-mode configuration: %s",
                      this->m_channel_s.name_e + 1U,
                      esp_err_to_name( error_t ) );

            return FALSE;
        }

        break;

    case DD_MCPWM_MODE_ON:
        /* Set corresponding I/O pin to permanent high-level */
        error_t = mcpwm_set_signal_high( this->m_channel_s.unit_e,
                                         this->m_channel_s.timer_e,
                                         this->m_channel_s.operator_e );

        if ( ESP_OK != error_t )
        {
            ESP_LOGE( DD_MCPWM_LOG_MSG_TAG,
                      "Can't update channel-%i-mode configuration: %s",
                      this->m_channel_s.name_e + 1U,
                      esp_err_to_name( error_t ) );

            return FALSE;
        }

        break;

    case DD_MCPWM_MODE_PWM:
        /* Update corresponding PWM settings */
        error_t = mcpwm_set_duty( this->m_channel_s.unit_e,
                                  this->m_channel_s.timer_e,
                                  this->m_channel_s.operator_e,
                                  r_data_in_s.duty_cycle_f32 );

        if ( ESP_OK != error_t )
        {
            ESP_LOGE( DD_MCPWM_LOG_MSG_TAG,
                      "Can't update channel-%i-mode configuration: %s",
                      this->m_channel_s.name_e + 1U,
                      esp_err_to_name( error_t ) );

            return FALSE;
        }

        error_t = mcpwm_set_duty_type( this->m_channel_s.unit_e,
                                       this->m_channel_s.timer_e,
                                       this->m_channel_s.operator_e,
                                       this->m_timer_cfg_s.duty_mode );

        if ( ESP_OK != error_t )
        {
            ESP_LOGE( DD_MCPWM_LOG_MSG_TAG,
                      "Can't update channel-%i-mode configuration: %s",
                      this->m_channel_s.name_e + 1U,
                      esp_err_to_name( error_t ) );

            return FALSE;
        }

        break;

    default:
        /* Should never happen */
        assert( r_data_in_s.mode_e == DD_MCPWM_MODE_OFF );
        assert( r_data_in_s.mode_e == DD_MCPWM_MODE_ON  );
        assert( r_data_in_s.mode_e == DD_MCPWM_MODE_PWM );

        return FALSE;
    }

    return TRUE;
}
