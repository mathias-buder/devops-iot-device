/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_adc.h
        @details ADC device driver implementation

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_ADC_H_
#define DD_ADC_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_adc_cal.h"

#include "esp_log.h"
#include "esp_err.h"

#include "../../types.h"


/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
#define DD_ADC_LOG_MSG_TAG              "DD_ADC"                /**< @details Domain log message tag string */
#define DD_ADC_UNIT                     ADC_UNIT_1              /**< @details ADC unit in use */
#define DD_ADC_RESOLUTION               ADC_WIDTH_BIT_12        /**< @details ADC resolution */
#define DD_ADC_FULL_SCALE_VALUE_12BIT   4095U                   /**< @details Maximum ADC value in 12-Bit mode */
#define DD_ADC_DEFAULT_V_REF            1100U                   /**< @details Default reference voltage @unit mV */
#define DD_ADC_NUM_SAMPLES_AVG          50U                     /**< @details Number of subsequent ADC readings to be averaged */
#define DD_ADC_CHANNEL                  ADC1_CHANNEL_0          /**< @details Channel to be used for ADC conversion */
#define DD_ADC_ATTENUATION              ADC_ATTEN_DB_0          /**< @details Channel attenuation setting */


/*************************************************************
*                      ENUMERATORS                           *
*************************************************************/



/*************************************************************
*      STRUCTURES                                            *
*************************************************************/
/**
 * @brief   ADC Output Interface Data Structure
 * @details Contains all data provided by the ADC module
 * @ingroup DriverStructures
 */
typedef struct DD_ADC_DATA_OUT_TYPE_TAG
{
    U16 raw_sample_u16;          /**< @details Current raw 12-bit ADC sample @unit [LSB] */
    U16 previous_raw_sample_u16; /**< @details Previous raw 12-bit ADC sample @unit [LSB] */
    U16 voltage_mV_u16;          /**< @details Actual voltage measured at the ADC input pin @unit [mV] */
    F32 raw_level_f32;           /**< @details ADC level where Full-Scale (4095) is mapped to 1.0 and 0 is mapped to 0.0 */
} DD_ADC_DATA_OUT_TYPE;

/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/

class DD_ADC_C {

  private:
    static DD_ADC_DATA_OUT_TYPE          data_out_s;            /**< @details Global device output data structure */
    static esp_adc_cal_characteristics_t adc_characteristics_s; /**< @details Stores the ADC characteristics */

    /**
     * @details This function characterizes the ADC and store the characteristics
     *          in parameter p_characteristics_s
     * @param[in] Pointer to the data structure that stores the ADC characteristics
     * @return Always TRUE
     */
    static BOOLEAN characterize( esp_adc_cal_characteristics_t* p_characteristics_s );

  public:
    /**
     * @details Default constructor
     */
    DD_ADC_C();

    /**
     * @details Default destructor
     */
    ~DD_ADC_C();

    /**
     * @details This function initialized the ADC device
     * @return Pointer to global device data structure
     */
    static DD_ADC_DATA_OUT_TYPE* init( void );

    /**
     * @details This is the drivers main function that shall be called
     * cyclicly and will provide all data through DD_ADC_DATA_OUT_TYPE
     */
    static void main( void );
};

#endif /* DD_ADC_H_ */
