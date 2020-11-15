/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_ina-219_Cfg.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_INA_219_CFG_H
#define DD_INA_219_CFG_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../Core/dd_types.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
/* Log message TAG string */
#define DD_INA_219_LOG_MSG_TAG                  "DD_INA_219"

#define DD_INA_219_I2C_ADDR                     0x40        /**< I2C write address, A0 = A1 = VDD */
#define DD_INA_219_SHUNT_RESISTOR_VALUE_OHM     ( 0.1F )    /**< Shunt resistor value @unit Ohm */


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
extern DD_INA_219_SHUNT_VOL_RANGE dd_ina_219_shunt_voltage_range_cfg_e;
extern DD_INA_219_SADC_RES_AVE    dd_ina_219_shunt_voltage_adc_res_ave_cfg_e;
extern DD_INA_219_BUS_VOL_RANGE   dd_ina_219_bus_voltage_range_cfg_e;
extern DD_INA_219_BADC_RES_AVE    dd_ina_219_bus_voltage_adc_res_ave_cfg_e;
extern F32                        dd_ina_219_max_current_mA_cfg_f32;

#endif /* DD_INA_219_CFG_H */