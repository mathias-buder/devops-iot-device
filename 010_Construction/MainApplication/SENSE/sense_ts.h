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

#ifndef SENSE_TS_H_
#define SENSE_TS_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../types.h"

/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
#define SENSE_TS_LOG_MSG_TAG              "SENSE_TS"


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/



/*************************************************************
*       ENUMERATORS                                          *
*************************************************************/



/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

typedef struct SENSE_FIR_CONFIDENCE_TYPE_TAG
{
    U64 asso_history_u64;   /**< bit field association history */
    F32 confidence_f32;     /**< confidence, FIR on association history */
    F32 confidence_max_f32; /**< maximum of confidence, FIR on association history */
} SENSE_FIR_CONFIDENCE_TYPE;

/**
 * @brief   MAX-30102 Output Interface Data Structure
 * @details MAX-30102 Output Interface Data Structure ...
 * @ingroup DriverStructures
 */
typedef struct SENSE_TS_CONFIG_TYPE_TAG
{
    F32 alpha_filter_coeff_a_f32;
    F32 alpha_beta_filter_coeff_a_f32;
    F32 alpha_beta_filter_coeff_b_f32;
    F32 min_touch_conf_level_f32;
    F32 min_touch_hyst_conf_level_f32;
} SENSE_TS_CONFIG_TYPE;

/**
 * @brief   Touch Sensor Database Structure
 * @details Contains all global data required to process the
 *          touch sensor.
 * @ingroup SenseStructures
 */
typedef struct SENSE_TS_DATA_IN_TYPE_TAG
{
    F32 adc_raw_level_f32;  /**< @details ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    U16 adc_voltage_mV_u16; /**< @details Actual voltage measured at the ADC input pin @unit [mV] */
} SENSE_TS_DATA_IN_TYPE;

/**
 * @brief   Touch Sensor Database Structure
 * @details Contains all global data required to process the
 *          touch sensor.
 * @ingroup SenseStructures
 */
typedef struct SENSE_TS_DATA_OUT_TYPE_TAG
{
    F32                       alpha_filtered_adc_level_f32;      /**< @details Alpha filtered ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    F32                       alpha_beta_filtered_adc_level_f32; /**< @details Alpha/Beta filtered ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
    SENSE_FIR_CONFIDENCE_TYPE touch_conf_s;                      /**< @details Touch confidence with a range of 0.0 to 1.0 */
} SENSE_TS_DATA_OUT_TYPE;


/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/

class SENSE_TS_C {

  private:
    SENSE_TS_DATA_OUT_TYPE data_out_s;
    F32                    xk_f32, vk_f32, rk_f32, xk_1_f32, vk_1_f32; /* Used for Alpha-Beta filter */
    F32                    alpha_filter_coeff_a_f32;
    F32                    alpha_beta_filter_coeff_a_f32;
    F32                    alpha_beta_filter_coeff_b_f32;
    F32                    min_touch_conf_level_f32;
    F32                    min_touch_hyst_conf_level_f32;

    F32 alpha_filter( F32 &current_sample_f32,
                      F32  previous_sample_f32,
                      F32  alpha_coeff_f32 );

    F32 alpha_bate_filter( F32 &current_sample_f32,
                           F32  previous_sample_f32,
                           F32  alpha_coeff_f32,
                           F32  beta_coeff_f32,
                           F32  dt_f32 );

    void compute_touch_confidence( F32&                       current_sample_f32,
                                   SENSE_FIR_CONFIDENCE_TYPE& touch_conf_s );

  public:
    /**
     * @details Default constructor
     */
    SENSE_TS_C( void );

    /**
     * @details Default destructor
     */
    ~SENSE_TS_C();

    /**
     * @details This function initialized the INA-219 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    SENSE_TS_DATA_OUT_TYPE* init( void );

    /**
     * @details This function initialized the INA-219 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    SENSE_TS_DATA_OUT_TYPE* init( SENSE_TS_CONFIG_TYPE& config_s );

    /**
     * @details This is the drivers main function that shall be called
     * cyclicly and will provide all data through DD_INA_219_DATA_OUT_TYPE
     */
    void main( SENSE_TS_DATA_IN_TYPE& data_in_s );
};

#endif /* SENSE_TS_H_ */
