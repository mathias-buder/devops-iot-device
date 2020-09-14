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
#define GPIO_U0_PWM0A_OUT 32   //Set GPIO 32 as PWM0A
#define GPIO_U0_PWM0B_OUT 33   //Set GPIO 33 as PWM0B

#define GPIO_U0_PWM1A_OUT 25   //Set GPIO 25 as PWM1A
#define GPIO_U0_PWM1B_OUT 26   //Set GPIO 26 as PWM1B

#define GPIO_U0_PWM2A_OUT 27   //Set GPIO 27 as PWM2A
#define GPIO_U0_PWM2B_OUT 14   //Set GPIO 14 as PWM2B



#define GPIO_U1_PWM0A_OUT 12   //Set GPIO 32 as PWM0A
#define GPIO_U1_PWM0B_OUT 13   //Set GPIO 33 as PWM0B

#define GPIO_U1_PWM1A_OUT 19   //Set GPIO 25 as PWM1A
#define GPIO_U1_PWM1B_OUT 23   //Set GPIO 26 as PWM1B

#define GPIO_U1_PWM2A_OUT 18   //Set GPIO 27 as PWM2A
#define GPIO_U1_PWM2B_OUT 5    //Set GPIO 14 as PWM2B

#define N_MCPWM           12U

mcpwm_config_t pwm_config_s;

F32 duty_cycle_f32 = 90.0F;

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

BOOLEAN dd_mcpwm_init( void )
{
    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Initializing ..." );


    pwm_config_s.frequency    = 1000;   //frequency = 1000Hz,
    pwm_config_s.cmpr_a       = 0;      //duty cycle of PWMxA = 0
    pwm_config_s.cmpr_b       = 0;      //duty cycle of PWMxb = 0
    pwm_config_s.counter_mode = MCPWM_UP_COUNTER;
    pwm_config_s.duty_mode    = MCPWM_DUTY_MODE_0;

    mcpwm_gpio_init( MCPWM_UNIT_0, MCPWM0A, GPIO_U0_PWM0A_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_0, MCPWM0B, GPIO_U0_PWM0B_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_0, MCPWM1A, GPIO_U0_PWM1A_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_0, MCPWM1B, GPIO_U0_PWM1B_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_0, MCPWM2A, GPIO_U0_PWM2A_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_0, MCPWM2B, GPIO_U0_PWM2B_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_1, MCPWM0A, GPIO_U1_PWM0A_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_1, MCPWM0B, GPIO_U1_PWM0B_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_1, MCPWM1A, GPIO_U1_PWM1A_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_1, MCPWM1B, GPIO_U1_PWM1B_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_1, MCPWM2A, GPIO_U1_PWM2A_OUT );
    mcpwm_gpio_init( MCPWM_UNIT_1, MCPWM2B, GPIO_U1_PWM2B_OUT );


    mcpwm_init( MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config_s );
    mcpwm_init( MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config_s );
    mcpwm_init( MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config_s );
    mcpwm_init( MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config_s );
    mcpwm_init( MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_config_s );
    mcpwm_init( MCPWM_UNIT_1, MCPWM_TIMER_2, &pwm_config_s );

    mcpwm_set_signal_low( MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A );
    mcpwm_set_signal_low( MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B );
    mcpwm_set_signal_low( MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A );
    mcpwm_set_signal_low( MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B );
    mcpwm_set_signal_low( MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A );
    mcpwm_set_signal_low( MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B );
    mcpwm_set_signal_low( MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_A );
    mcpwm_set_signal_low( MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_B );
    mcpwm_set_signal_low( MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_A );
    mcpwm_set_signal_low( MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_B );
    mcpwm_set_signal_low( MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A );
    mcpwm_set_signal_low( MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_B );


    mcpwm_set_duty( MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, (duty_cycle_f32 / (F32 ) N_MCPWM) * 1.0F );
    mcpwm_set_duty( MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, (duty_cycle_f32 / (F32 ) N_MCPWM) * 2.0F );
    mcpwm_set_duty( MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, (duty_cycle_f32 / (F32 ) N_MCPWM) * 3.0F );
    mcpwm_set_duty( MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B, (duty_cycle_f32 / (F32 ) N_MCPWM) * 4.0F );
    mcpwm_set_duty( MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, (duty_cycle_f32 / (F32 ) N_MCPWM) * 5.0F );
    mcpwm_set_duty( MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B, (duty_cycle_f32 / (F32 ) N_MCPWM) * 6.0F );
    mcpwm_set_duty( MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_A, (duty_cycle_f32 / (F32 ) N_MCPWM) * 7.0F );
    mcpwm_set_duty( MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_B, (duty_cycle_f32 / (F32 ) N_MCPWM) * 8.0F );
    mcpwm_set_duty( MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_A, (duty_cycle_f32 / (F32 ) N_MCPWM) * 9.0F );
    mcpwm_set_duty( MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_B, (duty_cycle_f32 / (F32 ) N_MCPWM) * 10.0F );
    mcpwm_set_duty( MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A, (duty_cycle_f32 / (F32 ) N_MCPWM) * 11.0F );
    mcpwm_set_duty( MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_B, (duty_cycle_f32 / (F32 ) N_MCPWM) * 12.0F );


    mcpwm_set_duty_type( MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_B, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_A, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_B, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A, MCPWM_DUTY_MODE_0 );
    mcpwm_set_duty_type( MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_B, MCPWM_DUTY_MODE_0 );



    ESP_LOGI( DD_MCPWM_LOG_MSG_TAG, "Done" );

    return TRUE;
}

void dd_mcpwm_main( void )
{

}


