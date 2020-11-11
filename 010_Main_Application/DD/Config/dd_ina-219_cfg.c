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
DD_INA_219_SHUNT_VOL_RANGE dd_ina_219_shunt_voltage_range_cfg_e = DD_INA_219_SHUNT_VOL_RANGE_40MV;
DD_INA_219_BUS_VOL_RANGE   dd_ina_219_bus_voltage_range_cfg_e   = DD_INA_219_BUS_VOL_RANGE_16V;

F32                        dd_ina_219_max_current_mA_f32        = 500.0F;
