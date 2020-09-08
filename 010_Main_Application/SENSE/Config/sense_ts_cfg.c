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

        @file sense_ts_cfg.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "sense_ts_cfg.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
F32 sense_ts_adc_alpha_filter_coeff_a_f32      = 0.4F;

F32 sense_ts_adc_alpha_beta_filter_coeff_a_f32 = 1.5F;
F32 sense_ts_adc_alpha_beta_filter_coeff_b_f32 = 0.5F;
