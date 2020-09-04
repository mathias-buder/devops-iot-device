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

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dlg_database.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
DLG_DATA           dlg_database_s;
DLG_LOG_DATA       dlg_log_database_s;
DD_ICM_20600_DATA* p_dlg_icm_20600_data_s;
DD_I2C_ERROR_TYPE* p_dlg_i2c_error_data_s;
DD_ADC_DATA*       p_dlg_adc_data_s;
DD_MAX_30102_DATA* p_dlg_max_30102_data_s;

/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
