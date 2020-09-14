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
#include "dd_mcpwm_cfg.h"
#include "dd_types_cfg.h"
#include "../Core/dd_types.h"

/*********************************************************************/
/*      GLOBAL PARAMETERS                                            */
/*********************************************************************/

DD_MCPWM_CHANNEL channel_cfg_vs[DD_MCPWM_CHANNEL_SIZE] = {
    {   /* CHANNEL_1 */
        .unit_e         = MCPWM_UNIT_0,
        .io_signal_e    = MCPWM0A,
        .timer_e        = MCPWM_TIMER_0,
        .operator_e     = MCPWM_OPR_A,
        .mode_e         = DD_MCPWM_MODE_OFF,
        .duty_cycle_f32 = 0.0F
    },
    {   /* CHANNEL_2 */
        .unit_e         = MCPWM_UNIT_0,
        .io_signal_e    = MCPWM0A,
        .timer_e        = MCPWM_TIMER_0,
        .operator_e     = MCPWM_OPR_A,
        .mode_e         = DD_MCPWM_MODE_OFF,
        .duty_cycle_f32 = 0.0F
    }
};
