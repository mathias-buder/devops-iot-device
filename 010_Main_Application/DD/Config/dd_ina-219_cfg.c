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

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_ina-219_cfg.h"
#include "dd_types_Cfg.h"

/*********************************************************************/
/*      COMPILETIME CONFIGURATION                                    */
/*********************************************************************/

/*********************************************************************/
/*      RUNTIME CONFIGURATION                                        */
/*********************************************************************/

/* Shunt voltage settings */
DD_INA_219_SHUNT_VOL_RANGE dd_ina_219_shunt_voltage_range_cfg_e       = DD_INA_219_SHUNT_VOL_RANGE_40MV;
DD_INA_219_SADC_RES_AVE    dd_ina_219_shunt_voltage_adc_res_ave_cfg_e = DD_INA_219_SADC_RES_AVE_12BIT_128S_69MS;

/* Bus voltage settings */
DD_INA_219_BUS_VOL_RANGE   dd_ina_219_bus_voltage_range_cfg_e         = DD_INA_219_BUS_VOL_RANGE_16V;
DD_INA_219_BADC_RES_AVE    dd_ina_219_bus_voltage_adc_res_ave_cfg_e   = DD_INA_219_BADC_RES_AVE_12BIT_1S_532US;

/* Calibration settings */
F32                        dd_ina_219_max_current_mA_cfg_f32          = 500.0F;     /**< @details Maximum expected current @unit mA */
