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

/**
 * @file dd_icm-20600_Cfg.h
 * @details Some detailed description
 */

#ifndef DD_MAX_30102_CFG_H
#define DD_MAX_30102_CFG_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../Core/dd_types.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
/* Log message TAG string */
#define DD_MAX_30105_LOG_MSG_TAG           "DD_MAX_30102"

#define DD_MAX_30105_I2C_ADDR               0x57 /* Write Address */
#define DD_MAX_30105_DEVICE_ID              0x15 /* Unique device id */


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

extern DD_MAX_30102_SAMPLE_AVG  dd_max_30102_sample_avg_cfg_e;
extern DD_MAX_30102_LED_MODE    dd_max_30102_mode_cfg_e;
extern DD_MAX_30102_ADC_RANGE   dd_max_30102_adc_range_cfg_e;
extern DD_MAX_30102_SAMPLE_RATE dd_max_30102_sample_rate_cfg_e;
extern U8                       dd_max_30102_led_amplitude_cfg_u8;
extern U8                       dd_max_30102_temp_time_out_cnt_cfg_u8;
extern U8                       dd_max_30102_temp_delay_ticks_cfg_u8;

/**
 * @details This function returns pointer to the osl object for the given id
 * of the linked environment object.
 * @param[in] linked_env_id_u8 is id of the linked environment object of the requested osl object
 * @return pointer to osl object or NULL if no appropriate object exists.
 */

#endif /* DD_MAX_30102_CFG_H */
