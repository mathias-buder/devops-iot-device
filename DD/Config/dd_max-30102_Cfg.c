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

DD_MAX_30102_SAMPLE_AVG  dd_max_30102_sample_average_e = DD_MAX_30102_SAMPLE_AVG_4;
DD_MAX_30102_LED_MODE    dd_max_30102_mode_e           = DD_MAX_30102_MODE_RED;
DD_MAX_30102_ADC_RANGE   dd_max_30102_adc_range_e      = DD_MAX_30102_ADC_RANGE_4096;
DD_MAX_30102_SAMPLE_RATE dd_max_30102_sample_rate_e    = DD_MAX_30102_SAMPLE_RATE_100;

/* Default is 0x1F which corresponds to 6.4mA
 * amplitude = 0x02, 0.4mA  - Presence detection of ~4 inch
 * amplitude = 0x1F, 6.4mA  - Presence detection of ~8 inch
 * amplitude = 0x7F, 25.4mA - Presence detection of ~8 inch
 * amplitude = 0xFF, 50.0mA - Presence detection of ~12 inch */

U8 dd_max_30102_led_amplitude_u8 = 0x1F;

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/



/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
