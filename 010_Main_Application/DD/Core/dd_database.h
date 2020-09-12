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
 * @file dd_database.h
 * @details Some detailed description
 */

#ifndef DD_DATABASE_H
#define DD_DATABASE_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
extern DD_SD_DATA        dd_sd_data_s;
extern DD_ADC_DATA       dd_adc_data_s;
extern DD_MCPWM_DATA     dd_mcpwm_data_s;
extern DD_I2C_ERROR_DATA dd_i2c_error_s;
extern DD_ICM_20600_DATA dd_icm_20600_data_s;
extern DD_MAX_30102_DATA dd_max_30102_data_s;

#endif /* DD_DATABASE_H */
