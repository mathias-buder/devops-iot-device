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
 * @brief   Touch Sensor Input Data Structure
 * @details Contains all data that is read in from different
 *          domain.
 * @ingroup SenseStructures
 */
typedef struct SENSE_TS_INPUT_TAG
{
    U16 raw_adc_sample_u16; /**< @details Current raw 12-bit ADC value @unit [LSB] */
    F32 raw_adc_level_f32;  /**< @details ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
} SENSE_TS_INPUT;

/**
 * @brief   Touch Sensor Database Structure
 * @details Contains all global data required to process the
 *          touch sensor.
 * @ingroup SenseStructures
 */
typedef struct SENSE_TS_DATA_TAG
{
    SENSE_TS_INPUT* p_input_s;                         /**< @details Pointer to the touch sensor input data structure */
    F32             alpha_filtered_adc_level_f32;      /**< @details Current alpha filtered raw 12-bit ADC value @unit [LSB] */
    F32             alpha_beta_filtered_adc_level_f32; /**< @details Current alpha/beta filtered raw 12-bit ADC value @unit [LSB] */
} SENSE_TS_DATA;

#endif /* SENSE_CORE_TYPES_H_ */
