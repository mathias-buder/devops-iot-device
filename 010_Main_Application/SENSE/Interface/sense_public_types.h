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

        @file sense_public_types.h
        @details Some detailed description

*********************************************************************/

#ifndef SENSE_INTERFACE_PUBLIC_TYPES_H_
#define SENSE_INTERFACE_PUBLIC_TYPES_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <stdio.h>
#include <esp_err.h>

#include "../../types.h"

#include "../../DD/DD.h"

/*************************************************************/
/*      ENUMERATORS                                          */
/*************************************************************/

/*************************************************************/
/*      GENERAL STRUCTURES                                   */
/*************************************************************/
typedef struct SENSE_FIR_CONFIDENCE_TYPE_TAG
{
    U64 asso_history_u64;   /**< bit field association history */
    F32 confidence_f32;     /**< confidence, FIR on association history */
    F32 confidence_max_f32; /**< maximum of confidence, FIR on association history */
} SENSE_FIR_CONFIDENCE_TYPE;

/*************************************************************/
/*      TOUCH SENSOR STRUCTURES                              */
/*************************************************************/

/**
 * @brief   Touch Sensor Database Structure
 * @details Contains all global data required to process the
 *          touch sensor.
 * @ingroup SenseStructures
 */
typedef struct SENSE_TS_DATA_TYPE_TAG
{
    DD_ADC_DATA_TYPE*         p_adc_input_s;                     /**< @details Pointer to ADC input structure */
    F32                       alpha_filtered_adc_level_f32;      /**< @details Alpha filtered ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    F32                       alpha_beta_filtered_adc_level_f32; /**< @details Alpha/Beta filtered ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    SENSE_FIR_CONFIDENCE_TYPE touch_conf_s;                      /**< @details Touch confidence with a range of 0.0 to 1.0 */
} SENSE_TS_DATA_TYPE;

#endif /* SENSE_INTERFACE_PUBLIC_TYPES_H_ */
