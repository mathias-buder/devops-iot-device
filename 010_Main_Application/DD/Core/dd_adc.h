/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_adc.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_ADC_H
#define DD_ADC_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "../Config/dd_adc_cfg.h"
#include "../Config/dd_types_cfg.h"

#include "dd_types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/**
 * @details This function initialized the ADC interface
 */
BOOLEAN dd_adc_init( void );

void dd_adc_main( void );

#endif /* DD_ADC_H */
