/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_mcpwm_cfg.c
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_MCPWM_CFG_H
#define DD_MCPWM_CFG_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "driver/gpio.h"

#include "../../types.h"
#include "../Core/dd_types.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
/* Log message TAG string */
#define DD_MCPWM_LOG_MSG_TAG              "DD_MCPWM"

#define DD_MCPWM_FREQUANCY                1000U                 /*!< Hz */
#define DD_MCPWM_COUNTER_MODE             MCPWM_UP_COUNTER      /*!< Asymmetric MCPWM*/
#define DD_MCPWM_DUTY_MODE                MCPWM_DUTY_MODE_0     /*!< Active high duty, i.e. duty cycle proportional to high time for asymmetric MCPWM */

#define DD_MCPWM_MAX_DUTY_CYCLE           100.0F               /*!< Active Maximum allowed duty cycle per channel @unit % */

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
extern DD_MCPWM_CHANNEL     dd_mcpwm_channel_cfg_vs[DD_MCPWM_CHANNEL_SIZE];
extern const mcpwm_config_t dd_mcpwm_timer_cfg_s;

#endif /* DD_MCPWM_CFG_H */
