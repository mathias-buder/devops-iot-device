/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_max-30102_Cfg.h"
#include "dd_types_Cfg.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
DD_MAX_30102_MODE        dd_max_30102_mode_cfg_e        = DD_MAX_30102_MODE_SPO2;
DD_MAX_30102_SAMPLE_RATE dd_max_30102_sample_rate_cfg_e = DD_MAX_30102_SAMPLE_RATE_100;
DD_MAX_30102_ADC_RANGE   dd_max_30102_adc_range_cfg_e   = DD_MAX_30102_ADC_RANGE_4096;
DD_MAX_30102_PULSE_WIDTH dd_max_30102_pulse_width_cfg_e = DD_MAX_30102_PULSE_WIDTH_411;
DD_MAX_30102_SAMPLE_AVG  dd_max_30102_sample_avg_cfg_e  = DD_MAX_30102_SAMPLE_AVG_16;

/* Default is 0x1F which corresponds to 6.4mA
 * amplitude = 0x02, 0.4mA  - Presence detection of ~4 inch
 * amplitude = 0x1F, 6.4mA  - Presence detection of ~8 inch
 * amplitude = 0x7F, 25.4mA - Presence detection of ~8 inch
 * amplitude = 0xFF, 50.0mA - Presence detection of ~12 inch */
U8 dd_max_30102_led_amplitude_cfg_u8        = 0x1F;

/* Configuration parameter for function dd_max_30102_get_temperature() */
U8 dd_max_30102_temp_time_out_cnt_cfg_u8    = 3U;
U8 dd_max_30102_temp_delay_ticks_cfg_u8     = 10U;

/* This register sets the IR ADC count that will trigger the beginning of HR or SpO2 mode. The threshold is defined as
* the 8 MSBs bits of the ADC count. For example, if PROX_INT_THRESH[7:0] = 0x01, then a 17-bit ADC value of 1023
* (decimal) or higher triggers the PROX_INT interrupt. If PROX_INT_THRESH[7:0] = 0xFF, then only a saturated ADC
* triggers the interrupt. */
U8 dd_max_30102_prox_threshold_cfg_u8       = 0x01;

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

