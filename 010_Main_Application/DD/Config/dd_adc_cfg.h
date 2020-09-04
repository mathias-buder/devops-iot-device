/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_adc_cfg.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_ADC_CFG_H
#define DD_ADC_CFG_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "driver/adc.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>

#include "../../types.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
/* Log message TAG string */
#define DD_ADC_LOG_MSG_TAG              "DD_ADC"

#define DD_ADC_UNIT                     ADC_UNIT_1              /**< @details ADC unit in use */

#define DD_ADC_RESOLUTION               ADC_WIDTH_BIT_12        /**< @details ADC resolution */

#define DD_ADC_FULL_SCALE_VALUE_12BIT   4095U                   /**< @details Maximum ADC value in 12-Bit mode */

#define DD_ADC_DEFAULT_V_REF            1100U                   /**< @details Default reference voltage in mV */

#define DD_ADC_NUM_SAMPLES_AVG          50U                     /**< @details Number of subsequent ADC readings to be averaged */

#define DD_ADC_CHANNEL                  ADC1_CHANNEL_0

#define DD_ADC_ATTENUATION              ADC_ATTEN_DB_0

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
extern F32 dd_adc_level_alpha_filter_coeff_f32;


#endif /* DD_ADC_CFG_H */
