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

/*********************************************************************/
/*      GLOBAL PARAMETERS                                            */
/*********************************************************************/

DD_MCPWM_CHANNEL dd_mcpwm_channel_cfg_vs[DD_MCPWM_CHANNEL_SIZE] = {

                           /* PWM Unit          UNIT I/O Signal    I/O Pin          Timer             PWM Operator    Operation Mode        Duty Cycle */
        /* CHANNEL_1 */  {    MCPWM_UNIT_0,     MCPWM0A,           GPIO_NUM_32,     MCPWM_TIMER_0,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F        },
        /* CHANNEL_2 */  {    MCPWM_UNIT_0,     MCPWM0B,           GPIO_NUM_33,     MCPWM_TIMER_0,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F        },
#ifndef JTAG_DEBUG_SUPPORT
        /* CHANNEL_3 */  {    MCPWM_UNIT_0,     MCPWM1A,           GPIO_NUM_14,     MCPWM_TIMER_1,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F        }, /*!< Shared with JTAG pin: TMS */
#endif
        /* CHANNEL_4 */  {    MCPWM_UNIT_0,     MCPWM1B,           GPIO_NUM_27,     MCPWM_TIMER_1,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F        },
#ifndef JTAG_DEBUG_SUPPORT
        /* CHANNEL_5 */  {    MCPWM_UNIT_0,     MCPWM2A,           GPIO_NUM_13,     MCPWM_TIMER_2,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F        }, /*!< Shared with JTAG pin: TCK */
        /* CHANNEL_6 */  {    MCPWM_UNIT_0,     MCPWM2B,           GPIO_NUM_12,     MCPWM_TIMER_2,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F        }, /*!< Shared with JTAG pin: TDI */
#endif
        /* CHANNEL_7 */  {    MCPWM_UNIT_1,     MCPWM0A,           GPIO_NUM_26,     MCPWM_TIMER_0,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F        },
        /* CHANNEL_8 */  {    MCPWM_UNIT_1,     MCPWM0B,           GPIO_NUM_25,     MCPWM_TIMER_0,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F        },
#ifndef JTAG_DEBUG_SUPPORT
        /* CHANNEL_9 */  {    MCPWM_UNIT_1,     MCPWM1A,           GPIO_NUM_15,     MCPWM_TIMER_1,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F        }, /*!< Shared with JTAG pin: TDO */
#endif
        /* CHANNEL_10 */ {    MCPWM_UNIT_1,     MCPWM1B,           GPIO_NUM_2,      MCPWM_TIMER_1,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F        },
        /* CHANNEL_11 */ {    MCPWM_UNIT_1,     MCPWM2A,           GPIO_NUM_0,      MCPWM_TIMER_2,    MCPWM_OPR_A,    DD_MCPWM_MODE_OFF,    0.0F        },
        /* CHANNEL_12 */ {    MCPWM_UNIT_1,     MCPWM2B,           GPIO_NUM_4,      MCPWM_TIMER_2,    MCPWM_OPR_B,    DD_MCPWM_MODE_OFF,    0.0F        }
};

const mcpwm_config_t dd_mcpwm_timer_cfg_s = {

    .frequency    = DD_MCPWM_FREQUANCY,    /*!< Default PWM frequency for all channels */
    .cmpr_a       = 0.0F,                  /*!< Default duty cycle for operator a(MCPWMXA) set to 0.0 */
    .cmpr_b       = 0.0F,                  /*!< Default duty cycle for operator b(MCPWMXB) set to 0.0 */
    .counter_mode = DD_MCPWM_COUNTER_MODE, /*!< Default counter mode for all channels */
    .duty_mode    = DD_MCPWM_DUTY_MODE     /*!< Default duty mode for all channels */

};
