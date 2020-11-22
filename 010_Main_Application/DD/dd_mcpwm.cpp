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
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
const mcpwm_config_t DD_MCPWM_C::timer_cfg_s = {

    .frequency    = DD_MCPWM_FREQUANCY,   /*!< Default PWM frequency for all channels */
    .cmpr_a       = 0.0F,                 /*!< Default duty cycle for operator a(MCPWMXA) set to 0.0 */
    .cmpr_b       = 0.0F,                 /*!< Default duty cycle for operator b(MCPWMXB) set to 0.0 */
    .duty_mode    = DD_MCPWM_DUTY_MODE,   /*!< Default duty mode for all channels */
    .counter_mode = DD_MCPWM_COUNTER_MODE /*!< Default counter mode for all channels */
};


DD_MCPWM_CONFIG_TYPE DD_MCPWM_C::unit_cfg_s = {

        {   /* DD_MCPWM_CHANNEL_TYPE */

                               /* PWM Unit          UNIT I/O Signal    I/O Pin          Timer             PWM Operator      Operation Mode        Duty Cycle */
    #ifndef JTAG_DEBUG_SUPPORT_ENABLE
            /* CHANNEL_1 */  {    MCPWM_UNIT_0,     MCPWM0A,           GPIO_NUM_14,     MCPWM_TIMER_0,    MCPWM_OPR_A,    { DD_MCPWM_MODE_OFF,    0.0F }   },  /*!< Shared with JTAG pin: TMS (14) */
    #endif
            /* CHANNEL_2 */  {    MCPWM_UNIT_0,     MCPWM0B,           GPIO_NUM_27,     MCPWM_TIMER_0,    MCPWM_OPR_B,    { DD_MCPWM_MODE_OFF,    0.0F }   },
            /* CHANNEL_3 */  {    MCPWM_UNIT_0,     MCPWM1A,           GPIO_NUM_32,     MCPWM_TIMER_1,    MCPWM_OPR_A,    { DD_MCPWM_MODE_OFF,    0.0F }   },
            /* CHANNEL_4 */  {    MCPWM_UNIT_0,     MCPWM1B,           GPIO_NUM_33,     MCPWM_TIMER_1,    MCPWM_OPR_B,    { DD_MCPWM_MODE_OFF,    0.0F }   },
            /* CHANNEL_5 */  {    MCPWM_UNIT_0,     MCPWM2A,           GPIO_NUM_26,     MCPWM_TIMER_2,    MCPWM_OPR_A,    { DD_MCPWM_MODE_OFF,    0.0F }   },
            /* CHANNEL_6 */  {    MCPWM_UNIT_0,     MCPWM2B,           GPIO_NUM_25,     MCPWM_TIMER_2,    MCPWM_OPR_B,    { DD_MCPWM_MODE_OFF,    0.0F }   },
    #ifndef JTAG_DEBUG_SUPPORT_ENABLE
            /* CHANNEL_7 */  {    MCPWM_UNIT_1,     MCPWM0A,           GPIO_NUM_13,     MCPWM_TIMER_0,    MCPWM_OPR_A,    { DD_MCPWM_MODE_OFF,    0.0F }   },  /*!< Shared with JTAG pin: TCK (13) */
            /* CHANNEL_8 */  {    MCPWM_UNIT_1,     MCPWM0B,           GPIO_NUM_12,     MCPWM_TIMER_0,    MCPWM_OPR_B,    { DD_MCPWM_MODE_OFF,    0.0F }   },  /*!< Shared with JTAG pin: TDI (12) */
            /* CHANNEL_9 */  {    MCPWM_UNIT_1,     MCPWM1A,           GPIO_NUM_15,     MCPWM_TIMER_1,    MCPWM_OPR_A,    { DD_MCPWM_MODE_OFF,    0.0F }   },  /*!< Shared with JTAG pin: TDO (15) */
    #endif
            /* CHANNEL_10 */ {    MCPWM_UNIT_1,     MCPWM1B,           GPIO_NUM_2,      MCPWM_TIMER_1,    MCPWM_OPR_B,    { DD_MCPWM_MODE_OFF,    0.0F }   },
            /* CHANNEL_11 */ {    MCPWM_UNIT_1,     MCPWM2A,           GPIO_NUM_0,      MCPWM_TIMER_2,    MCPWM_OPR_A,    { DD_MCPWM_MODE_OFF,    0.0F }   },
            /* CHANNEL_12 */ {    MCPWM_UNIT_1,     MCPWM2B,           GPIO_NUM_4,      MCPWM_TIMER_2,    MCPWM_OPR_B,    { DD_MCPWM_MODE_OFF,    0.0F }   }
        }
};

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

BOOLEAN DD_MCPWM_C::init( void )
{
    return DD_MCPWM_C::init( DD_MCPWM_C::unit_cfg_s );
}

BOOLEAN DD_MCPWM_C::init( DD_MCPWM_CONFIG_TYPE &config_s )
{
    U8        idx_u8, idy_u8;
    esp_err_t error_t = ESP_OK;

    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Initializing ..." );

    /* Overwrite default unit configuration */
   // DD_MCPWM_C::unit_cfg_s = config_s; --> To be checked with JMD

    /* Initialize all GPIOs to be multiplexed to the corresponding PWM channel */
    for ( idx_u8 = 0U; idx_u8 < DD_MCPWM_CHANNEL_SIZE; ++idx_u8 )
    {
        error_t = mcpwm_gpio_init( unit_cfg_s.p_channel_cfg_vs[idx_u8].unit_e,
                                   unit_cfg_s.p_channel_cfg_vs[idx_u8].io_signal_e,
                                   unit_cfg_s.p_channel_cfg_vs[idx_u8].io_pin_e );

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
            error_t = mcpwm_init( (mcpwm_unit_t) idx_u8, (mcpwm_timer_t) idy_u8, &DD_MCPWM_C::timer_cfg_s );

            if ( ESP_OK != error_t )
            {
                ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Couldn't initialize timers with global configuration: %s", esp_err_to_name( error_t ) );
                return FALSE;
            }
        }
    }

    for ( idx_u8 = 0U; idx_u8 < DD_MCPWM_CHANNEL_SIZE; ++idx_u8 )
    {
            switch ( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].pwm_cfg_s.mode_e )
            {

            case DD_MCPWM_MODE_OFF:
                /* Set corresponding I/O pin to permanent low-level */
                error_t = mcpwm_set_signal_low( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].unit_e,
                                                DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].timer_e,
                                                DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].operator_e );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                break;

            case DD_MCPWM_MODE_ON:
                /* Set corresponding I/O pin to permanent high-level */
                error_t = mcpwm_set_signal_high( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].unit_e,
                                                 DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].timer_e,
                                                 DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].operator_e );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                break;

            case DD_MCPWM_MODE_PWM:
                /* Update corresponding PWM settings */
                error_t = mcpwm_set_duty( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].unit_e,
                                          DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].timer_e,
                                          DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].operator_e,
                                          DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].pwm_cfg_s.duty_cycle_f32 );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                error_t = mcpwm_set_duty_type( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].unit_e,
                                               DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].timer_e,
                                               DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].operator_e,
                                               DD_MCPWM_C::timer_cfg_s.duty_mode );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                break;

            default:
                /* Should never happen */
                assert( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].pwm_cfg_s.mode_e == DD_MCPWM_MODE_OFF );
                assert( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].pwm_cfg_s.mode_e == DD_MCPWM_MODE_ON  );
                assert( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[idx_u8].pwm_cfg_s.mode_e == DD_MCPWM_MODE_PWM );

                return FALSE;
            }
    }


    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Done" );
    return TRUE;
}

BOOLEAN DD_MCPWM_C::update_channels( DD_MCPWM_DATA_IN_TYPE data_in_s )
{

    U8        channel_idx_u8;
    esp_err_t error_t = ESP_OK;

        for ( channel_idx_u8 = 0U; channel_idx_u8 < DD_MCPWM_CHANNEL_SIZE; ++channel_idx_u8 )
        {
            switch ( data_in_s.pwm_cfg_s[channel_idx_u8].mode_e )
            {

            case DD_MCPWM_MODE_OFF:
                /* Set corresponding I/O pin to permanent low-level */
                error_t = mcpwm_set_signal_low( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].unit_e,
                                                DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].timer_e,
                                                DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].operator_e );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                break;

            case DD_MCPWM_MODE_ON:
                /* Set corresponding I/O pin to permanent high-level */
                error_t = mcpwm_set_signal_high( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].unit_e,
                                                 DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].timer_e,
                                                 DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].operator_e );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                break;

            case DD_MCPWM_MODE_PWM:
                /* Update corresponding PWM settings */
                error_t = mcpwm_set_duty( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].unit_e,
                                          DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].timer_e,
                                          DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].operator_e,
                                          data_in_s.pwm_cfg_s[channel_idx_u8].duty_cycle_f32 );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                error_t = mcpwm_set_duty_type( DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].unit_e,
                                               DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].timer_e,
                                               DD_MCPWM_C::unit_cfg_s.p_channel_cfg_vs[channel_idx_u8].operator_e,
                                               DD_MCPWM_C::timer_cfg_s.duty_mode );

                if ( ESP_OK != error_t )
                {
                    ESP_LOGE( DD_MCPWM_LOG_MSG_TAG, "Can't set channel-mode configuration: %s", esp_err_to_name( error_t ) );
                    return FALSE;
                }

                break;

            default:
                /* Should never happen */
                assert( data_in_s.pwm_cfg_s[channel_idx_u8].mode_e == DD_MCPWM_MODE_OFF );
                assert( data_in_s.pwm_cfg_s[channel_idx_u8].mode_e == DD_MCPWM_MODE_ON );
                assert( data_in_s.pwm_cfg_s[channel_idx_u8].mode_e == DD_MCPWM_MODE_PWM );

                return FALSE;
            }
        }

    return TRUE;
}
