/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_pga-302_cfg.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "dd_types_cfg.h"
#include "dd_pga-302_cfg.h"


/*********************************************************************/
/*      COMPILETIME CONFIGURATION                                    */
/*********************************************************************/

/*********************************************************************/
/*      RUNTIME CONFIGURATION                                        */
/*********************************************************************/
DD_PGA_302_P_GAIN       dd_pga_303_p_gain_cfg_e    = DD_PGA_302_P_GAIN_1_33;
DD_PGA_302_P_INPUT_MODE dd_pga_303_p_mode_cfg_e    = DD_PGA_302_P_INPUT_MODE_SINGLE_ENDED;
DD_PGA_302_P_INPUT_MUX  dd_pga_303_p_mux_cfg_e     = DD_PGA_302_P_INPUT_MUX_VINPP_VINPN;
BOOLEAN                 dd_pga_303_p_inverse_cfg_b = FALSE;
