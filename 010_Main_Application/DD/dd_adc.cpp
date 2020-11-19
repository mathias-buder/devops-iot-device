/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_adc.c
        @details ADC device driver implementation

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_adc.h"

#include <stdio.h>
#include <string.h>

#include "esp_log.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
esp_adc_cal_characteristics_t DD_ADC_C::adc_characteristics_s;
DD_ADC_DATA_OUT_TYPE          DD_ADC_C::data_out_s;


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
DD_ADC_DATA_OUT_TYPE* DD_ADC_C::init( void )
{
    ESP_LOGI(DD_ADC_LOG_MSG_TAG, "Initializing ..." );

    if(    ( ESP_OK != adc1_config_width( DD_ADC_RESOLUTION ) )
        || ( ESP_OK != adc1_config_channel_atten( DD_ADC_CHANNEL, DD_ADC_ATTENUATION ) )
        || ( FALSE  == DD_ADC_C::characterize( &DD_ADC_C::adc_characteristics_s ) ) )
    {
        ESP_LOGE( DD_ADC_LOG_MSG_TAG, "Error while initializing the ADC" );
        return NULL;
    }

    ESP_LOGI(DD_ADC_LOG_MSG_TAG, "Done" );

    return &DD_ADC_C::data_out_s;
}


void DD_ADC_C::main( void )
{
    U8  idx_u8;
    U32 accumulated_raw_value_u32 = 0U;

    /* Accumulate DD_ADC_NUM_AVG subsequent ADC values */
    for ( idx_u8 = 0U; idx_u8 < DD_ADC_NUM_SAMPLES_AVG; ++idx_u8 )
    {
        accumulated_raw_value_u32 += adc1_get_raw( DD_ADC_CHANNEL );
    }

    /* Store previous ADC reading */
    DD_ADC_C::data_out_s.previous_raw_sample_u16 = DD_ADC_C::data_out_s.raw_sample_u16;

    /* Scale current ADC reading */
    DD_ADC_C::data_out_s.raw_sample_u16 = ( U16 )( accumulated_raw_value_u32 / DD_ADC_NUM_SAMPLES_AVG );

    /* Calculate corresponding voltage level */
    DD_ADC_C::data_out_s.voltage_u16 = esp_adc_cal_raw_to_voltage( DD_ADC_C::data_out_s.raw_sample_u16, &DD_ADC_C::adc_characteristics_s );

    /* Calculate ADC level */
    DD_ADC_C::data_out_s.raw_level_f32 = (F32) ( (F32) DD_ADC_C::data_out_s.raw_sample_u16 / (F32) DD_ADC_FULL_SCALE_VALUE_12BIT );


    ESP_LOGD( DD_ADC_LOG_MSG_TAG, "CVal: %i, CVol: %i mV, CLvl: %0.3f", DD_ADC_C::data_out_s.raw_sample_u16,
                                                                        DD_ADC_C::data_out_s.voltage_u16,
                                                                        DD_ADC_C::data_out_s.raw_level_f32 );
}


BOOLEAN DD_ADC_C::characterize( esp_adc_cal_characteristics_t* p_characteristics_s )
{
    esp_adc_cal_value_t adc_cal_type_e;

    ESP_LOGI(DD_ADC_LOG_MSG_TAG, "Characterizing ADC ..." );

    /* Get ADC characterization based on the given parameters */
    adc_cal_type_e = esp_adc_cal_characterize( DD_ADC_UNIT, DD_ADC_ATTENUATION, DD_ADC_RESOLUTION, DD_ADC_DEFAULT_V_REF, p_characteristics_s );

    switch ( adc_cal_type_e )
    {
    case ESP_ADC_CAL_VAL_EFUSE_VREF:
        ESP_LOGI( DD_ADC_LOG_MSG_TAG, "ADC characterization: eFuse Vref");
        break;

    case ESP_ADC_CAL_VAL_EFUSE_TP:
        ESP_LOGI( DD_ADC_LOG_MSG_TAG, "ADC characterization: Two Point");
        break;

    case ESP_ADC_CAL_VAL_DEFAULT_VREF:
        ESP_LOGI( DD_ADC_LOG_MSG_TAG, "ADC characterization: Default");
        break;
    }

    return TRUE;
}
