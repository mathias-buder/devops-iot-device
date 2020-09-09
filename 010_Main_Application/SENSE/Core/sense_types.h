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

        @file sense_types.h
        @details Some detailed description

*********************************************************************/

#ifndef SENSE_CORE_TYPES_H_
#define SENSE_CORE_TYPES_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "esp_err.h"

#include "../../types.h"

#include "../Config/sense_hr_cfg.h"
#include "../Config/sense_mtn_cfg.h"
#include "../Config/sense_spo2_cfg.h"
#include "../Config/sense_ts_cfg.h"

#include "../Interface/sense_public_types.h"

#include "../../DD/DD.h"

/*************************************************************/
/*      GENERAL STRUCTURES                                   */
/*************************************************************/
typedef struct SENSE_FIR_CONFIDENCE_TAG
{
    U64 asso_history_u64;   /**< bit field association history */
    F32 confidence_f32;     /**< confidence, FIR on association history */
    F32 confidence_max_f32; /**< maximum of confidence, FIR on association history */
} SENSE_FIR_CONFIDENCE;

/*************************************************************/
/*      ENUMERATORS                                          */
/*************************************************************/


/*************************************************************/
/*      HEART-RATE STRUCTURES                                */
/*************************************************************/


/*************************************************************/
/*      MOTION STRUCTURES                                    */
/*************************************************************/

/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup SenseStructures
 */
typedef struct SENSE_MTN_QUATERNION_TAG
{
    F32 Q1_f32;
    F32 Q2_f32;
    F32 Q3_f32;
    F32 Q4_f32;
} SENSE_MTN_QUATERNION;


/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup SenseStructures
 */
typedef struct SENSE_MTN_DATA_TAG
{
    SENSE_MTN_QUATERNION Quaternion_s;  /**< @details Acceleration raw data */
} SENSE_MTN_DATA;




/*************************************************************/
/*      SPO STRUCTURES                                       */
/*************************************************************/


/*************************************************************/
/*      TOUCH SENSOR STRUCTURES                              */
/*************************************************************/

/**
 * @brief   Touch Sensor Database Structure
 * @details Contains all global data required to process the
 *          touch sensor.
 * @ingroup SenseStructures
 */
typedef struct SENSE_TS_DATA_TAG
{
    DD_ADC_DATA*         p_adc_input_s;                     /**< @details Pointer to ADC input structure */
    F32                  alpha_filtered_adc_level_f32;      /**< @details Current alpha filtered raw 12-bit ADC value @unit [LSB] */
    F32                  alpha_beta_filtered_adc_level_f32; /**< @details Current alpha/beta filtered raw 12-bit ADC value @unit [LSB] */
    SENSE_FIR_CONFIDENCE sensor_conf_s;                     /**< @details Touch confidence with a range of 0.0 to 1.0 */
} SENSE_TS_DATA;

#endif /* SENSE_CORE_TYPES_H_ */
