/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file ve_grid.c
        @details ...

        (c) SEWELA 2020

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "ve_grid_cfg.h"

/*********************************************************************/
/*      GLOBAL PARAMETERS                                            */
/*********************************************************************/
VE_GRID_VIRBATOR_TYPE ve_grid_vibrator_config_vs[VE_GRID_VIBRATOR_SIZE] = {
                            /*  PWM channel assignment      X-Position [mm]     Y-Position [mm]     Duty Cycle [%] */
#ifndef JTAG_DEBUG_SUPPORT_ENABLE
        /* VIBRATOR 1 */    {   DD_MCPWM_CHANNEL_1,         0.0F,               0.0F,               0.0F    },
#endif
        /* VIBRATOR 2 */    {   DD_MCPWM_CHANNEL_2,         0.0F,               0.0F,               0.0F    },
        /* VIBRATOR 3 */    {   DD_MCPWM_CHANNEL_3,         0.0F,               0.0F,               0.0F    },
        /* VIBRATOR 4 */    {   DD_MCPWM_CHANNEL_4,         0.0F,               0.0F,               0.0F    },
        /* VIBRATOR 5 */    {   DD_MCPWM_CHANNEL_5,         0.0F,               0.0F,               0.0F    },
        /* VIBRATOR 6 */    {   DD_MCPWM_CHANNEL_6,         0.0F,               0.0F,               0.0F    },
#ifndef JTAG_DEBUG_SUPPORT_ENABLE
        /* VIBRATOR 7 */    {   DD_MCPWM_CHANNEL_7,         0.0F,               0.0F,               0.0F    },
        /* VIBRATOR 8 */    {   DD_MCPWM_CHANNEL_8,         0.0F,               0.0F,               0.0F    },
        /* VIBRATOR 9 */    {   DD_MCPWM_CHANNEL_9,         0.0F,               0.0F,               0.0F    },
#endif
        /* VIBRATOR 10 */   {   DD_MCPWM_CHANNEL_10,        0.0F,               0.0F,               0.0F    }
};
