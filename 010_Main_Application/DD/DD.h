/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file DD.h
        @details Some detailed description

*********************************************************************/
#ifndef DD_H_
#define DD_H_


/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_adc.h"
#include "dd_mcpwm.h"
#include "dd_ina-219.h"
#include "dd_tmp-102.h"
#include "dd_icm-20600.h"
#include "dd_max-30102.h"

/*************************************************************
*      STRUCTURES                                            *
*************************************************************/
/**
 * @brief   Device Driver Output Input Structure
 * @details Contains all data that is required by the Device Driver (DD) domain
 * @ingroup DriverStructures
 */
typedef struct DD_DATA_IN_TYPE_TAG
{
    DD_MCPWM_DATA_IN_TYPE mcpwm_data_in_s;

} DD_DATA_IN_TYPE;




/**
 * @brief   Device Driver Output Data Structure
 * @details Contains all data provided by the Device Driver (DD) domain
 * @ingroup DriverStructures
 */
typedef struct DD_DATA_OUT_TYPE_TAG
{
    const DD_ADC_DATA_OUT_TYPE*       p_adc_data_out_s;       /**< @details ADC output data structure */
    const DD_INA_219_DATA_OUT_TYPE*   p_ina_219_data_out_s;   /**< @details INA-219 output data structure */
    const DD_TMP_102_DATA_OUT_TYPE*   p_tmp_102_data_out_s;   /**< @details TMP-102 output data structure */
    const DD_ICM_20600_DATA_OUT_TYPE* p_icm_20600_data_out_s; /**< @details ICN-20600 output data structure */
    const DD_MAX_30102_DATA_OUT_TYPE* p_max_30102_data_out_s; /**< @details MAX-30102 output data structure */
} DD_DATA_OUT_TYPE;


/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/
class DD_C {

  private:
    static DD_DATA_OUT_TYPE dd_data_out_s; /**< @details Contains all data provided by the Device Driver (DD) domain */

  public:
    /**
     * @details This function initialized the INA-219 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    static void init( void );

    /**
     * @details This function initialized the INA-219 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    static DD_DATA_OUT_TYPE process_inputs( void );

    /**
     * @details This function initialized the INA-219 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    static void process_outputs( DD_DATA_IN_TYPE dd_data_in_s );

};

#endif /* DD_H_ */
