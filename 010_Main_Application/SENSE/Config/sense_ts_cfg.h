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

        @file sense_ts_cfg.h
        @details Some detailed description

*********************************************************************/
#ifndef SENSE_CONFIG_TS_CFG_H_
#define SENSE_CONFIG_TS_CFG_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

/* Log message TAG string */
#define SENSE_TS_LOG_MSG_TAG              "SENSE_TS"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
extern F32 sense_ts_adc_alpha_filter_coeff_a_f32;
extern F32 sense_ts_adc_alpha_beta_filter_coeff_a_f32;
extern F32 sense_ts_adc_alpha_beta_filter_coeff_b_f32;
extern F32 sense_ts_min_touch_conf_level_f32;
extern F32 sense_ts_min_touch_hyst_conf_level_f32;

#endif /* SENSE_CONFIG_TS_CFG_H_ */
