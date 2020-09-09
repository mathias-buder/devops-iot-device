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

        @file dlg_database.h
        @details Some detailed description

*********************************************************************/
#ifndef DLG_CORE_DATABASE_H_
#define DLG_CORE_DATABASE_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dlg_types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
extern DLG_DATA           dlg_database_s;
extern DLG_LOG_DATA       dlg_log_database_s;

extern DD_ICM_20600_DATA* p_dlg_dd_icm_20600_data_s;
extern DD_I2C_ERROR_DATA* p_dlg_dd_i2c_error_data_s;
extern DD_ADC_DATA*       p_dlg_dd_adc_data_s;
extern DD_MAX_30102_DATA* p_dlg_dd_max_30102_data_s;

extern SENSE_TS_DATA*     p_dlg_sense_ts_data_s;

#endif /* DLG_CORE_DATABASE_H_ */
