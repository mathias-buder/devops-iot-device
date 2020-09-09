/*********************************************************************
    MODULE SPECIFICATION:

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file sense_ts.h
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "../Config/sense_ts_cfg.h"
#include "sense_database.h"
#include "sense_ts.h"

#include "../../DD/DD.h"
#include "../../UTIL/UTIL.h"

#include "esp_log.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
F32 xk_f32, vk_f32, rk_f32, xk_1, vk_1; /* Used for Alpha-Beta filter */
F32 dt_f32 = 0.1F;                      /* TODO: Use global cycle time define here ( currently 100 ms ) */
/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE void sense_ts_filter_pressure( void );
PRIVATE void sense_ts_calc_sensor_confidence( void );

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
BOOLEAN sense_ts_init( void )
{
    /* Initialize database to 0 */
    memset( &sense_ts_data_s, 0U, sizeof( sense_ts_data_s ) );

    /* Get pointer to ADC database structure */
    sense_ts_data_s.p_adc_input_s = dd_adc_get_database();

    return TRUE;
}


void sense_ts_main( void )
{

    sense_ts_filter_pressure();

    ESP_LOGD( SENSE_TS_LOG_MSG_TAG, "ALvl: %0.3f, ABLvl: %0.3f", sense_ts_data_s.alpha_filtered_adc_level_f32,
                                                                 sense_ts_data_s.alpha_beta_filtered_adc_level_f32 );
}

PRIVATE void sense_ts_filter_pressure( void )
{
    /* Alpha filter signal */
    sense_ts_data_s.alpha_filtered_adc_level_f32 =   ( sense_ts_data_s.alpha_filtered_adc_level_f32 * sense_ts_adc_alpha_filter_coeff_a_f32            )
                                                   + ( ( 1.0F - sense_ts_adc_alpha_filter_coeff_a_f32 ) * sense_ts_data_s.p_adc_input_s->raw_level_f32 );

    /* Alpha-Beta filter signal */
    xk_f32 = xk_1 + ( vk_1 * dt_f32 );
    vk_f32 = vk_1;

    rk_f32 = sense_ts_data_s.p_adc_input_s->raw_level_f32 - xk_f32;

    xk_f32 += sense_ts_adc_alpha_beta_filter_coeff_a_f32 * rk_f32;
    vk_f32 += ( sense_ts_adc_alpha_beta_filter_coeff_b_f32 * rk_f32 ) / dt_f32;

    xk_1 = xk_f32;
    vk_1 = vk_f32;

    /* Store filtered output */
    /* TODO: Output need to be clamped to a range of 0.0 to 1.0 */
    sense_ts_data_s.alpha_beta_filtered_adc_level_f32 = xk_1;
}

PRIVATE void sense_ts_calc_sensor_confidence( void )
{

}


