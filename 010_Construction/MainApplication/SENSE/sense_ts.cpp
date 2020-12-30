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

        @file sense_ts.cpp
        @details Contains all touch sensor functions

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "../UTIL/UTIL.h"

#include "sense_ts.h"


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
SENSE_TS_C::SENSE_TS_C( void )
{
    this->alpha_filter_coeff_a_f32      = 0.0F;
    this->alpha_beta_filter_coeff_a_f32 = 0.0F;
    this->alpha_beta_filter_coeff_b_f32 = 0.0F;
    this->min_touch_conf_level_f32      = 0.0F;
    this->min_touch_hyst_conf_level_f32 = 0.0F;

    this->xk_f32                        = 0.0F;
    this->vk_f32                        = 0.0F;
    this->rk_f32                        = 0.0F;
    this->xk_1_f32                      = 0.0F;
    this->vk_1_f32                      = 0.0F;
}

SENSE_TS_C::~SENSE_TS_C( void )
{

}


SENSE_TS_DATA_OUT_TYPE* SENSE_TS_C::init( void )
{
    SENSE_TS_CONFIG_TYPE default_cfg_s = {
        .alpha_filter_coeff_a_f32      = 0.4F,
        .alpha_beta_filter_coeff_a_f32 = 1.5F,
        .alpha_beta_filter_coeff_b_f32 = 0.5F,
        .min_touch_conf_level_f32      = 0.2F,
        .min_touch_hyst_conf_level_f32 = 0.1F
    };

    return ( this->init( default_cfg_s ) );
}


SENSE_TS_DATA_OUT_TYPE* SENSE_TS_C::init( SENSE_TS_CONFIG_TYPE& config_s )
{
    this->alpha_filter_coeff_a_f32      = config_s.alpha_filter_coeff_a_f32;
    this->alpha_beta_filter_coeff_a_f32 = config_s.alpha_beta_filter_coeff_a_f32;
    this->alpha_beta_filter_coeff_b_f32 = config_s.alpha_beta_filter_coeff_b_f32;
    this->min_touch_conf_level_f32      = config_s.min_touch_conf_level_f32;
    this->min_touch_hyst_conf_level_f32 = config_s.min_touch_hyst_conf_level_f32;

    return &this->data_out_s;
}


void SENSE_TS_C::main( SENSE_TS_DATA_IN_TYPE& data_in_s )
{

    data_out_s.alpha_filtered_adc_level_f32 = alpha_filter( data_in_s.adc_raw_level_f32,
                                                            data_out_s.alpha_filtered_adc_level_f32,
                                                            this->alpha_filter_coeff_a_f32 );

    data_out_s.alpha_beta_filtered_adc_level_f32 = alpha_bate_filter( data_in_s.adc_raw_level_f32,
                                                                      data_out_s.alpha_beta_filtered_adc_level_f32,
                                                                      this->alpha_beta_filter_coeff_a_f32,
                                                                      this->alpha_beta_filter_coeff_b_f32,
                                                                      0.1F );

    compute_touch_confidence( data_in_s.adc_raw_level_f32,
                              data_out_s.touch_conf_s );

    ESP_LOGD( SENSE_TS_LOG_MSG_TAG, "ALvl: %0.3f, ABLvl: %0.3f, TConf: %0.3f", data_out_s.alpha_filtered_adc_level_f32,
                                                                               data_out_s.alpha_beta_filtered_adc_level_f32,
                                                                               data_out_s.touch_conf_s.confidence_f32 );
}


F32 SENSE_TS_C::alpha_filter( F32 &current_sample_f32,
                              F32  filtered_sample_f32,
                              F32  alpha_coeff_f32 )
{
    return (   ( filtered_sample_f32 * alpha_coeff_f32           )
             + ( ( 1.0F - alpha_coeff_f32 ) * current_sample_f32 ) );
}


F32 SENSE_TS_C::alpha_bate_filter( F32& current_sample_f32,
                                   F32  filtered_sample_f32,
                                   F32  coeff_a_f32,
                                   F32  coeff_b_f32,
                                   F32  dt_f32 )
{
    this->xk_f32 = this->xk_1_f32 + ( this->vk_1_f32 * dt_f32 );
    this->vk_f32 = this->vk_1_f32;

    this->rk_f32 = current_sample_f32 - this->xk_f32;

    this->xk_f32 += coeff_a_f32 * this->rk_f32;
    this->vk_f32 += ( coeff_b_f32 * this->rk_f32 ) / dt_f32;

    this->xk_1_f32 = this->xk_f32;
    this->vk_1_f32 = this->vk_f32;

    /* Store filtered output ( clamped to a range of 0.0 to 1.0 ) */
    return( CLAMP( this->xk_1_f32, 0.0F, 1.0F ) );
}


void SENSE_TS_C::compute_touch_confidence( F32&                       current_level_f32,
                                           SENSE_FIR_CONFIDENCE_TYPE& touch_conf_s )
{
    UTIL_CONF_DETECTION_STATE detection_status_e;

    if ( current_level_f32 > this->min_touch_conf_level_f32 )
    {
        if ( current_level_f32 > ( this->min_touch_conf_level_f32 + this->min_touch_hyst_conf_level_f32 ) )
        {
            detection_status_e = IS_DETECTION;
        }
        else
        {
            detection_status_e = NO_UPDATE;
        }
    }
    else
    {
        detection_status_e = NO_DETECTION;
    }

    util_update_fir_confidence( &touch_conf_s.confidence_f32,
                                &touch_conf_s.confidence_max_f32,
                                &touch_conf_s.asso_history_u64,
                                UTIL_CONF_MAX_FIR_CONFIDENCE_BUFFER_LENGTH,
                                detection_status_e );
}
