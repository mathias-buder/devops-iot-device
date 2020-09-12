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
#define GPIO_PWM0A_OUT 32   //Set GPIO 15 as PWM0A
// #define GPIO_PWM0B_OUT 16   //Set GPIO 16 as PWM0B

mcpwm_config_t pwm_config;

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

BOOLEAN dd_mcpwm_init( void )
{
    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Initializing ..." );

    mcpwm_gpio_init( MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT );

    pwm_config.frequency    = 1000;   //frequency = 500Hz,
    pwm_config.cmpr_a       = 0;      //duty cycle of PWMxA = 0
    pwm_config.cmpr_b       = 0;      //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode    = MCPWM_DUTY_MODE_0;

    mcpwm_init( MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config );   //Configure PWM0A & PWM0B with above settings

    mcpwm_set_signal_low( MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B );

    mcpwm_set_duty( MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 75.0F );
    mcpwm_set_duty_type( MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0 );   //call this each time, if operator was previously in low/high state

    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Done" );

    return TRUE;
}

void dd_mcpwm_main( void )
{

}


