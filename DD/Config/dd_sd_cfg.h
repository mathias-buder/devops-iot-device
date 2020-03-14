/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_sd_cfg.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_SD_CFG_H
#define DD_SD_CFG_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <driver/gpio.h>
#include "dd_types_cfg.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
#define DD_SD_LOG_MSG_TAG           "DD_SD"

/* Hard-wired pins to SD card */
#define DD_SD_MISO_PIN              GPIO_NUM_2
#define DD_SD_MOSI_PIN              GPIO_NUM_15
#define DD_SD_CLK_PIN               GPIO_NUM_14
#define DD_SD_CS_PIN                GPIO_NUM_13

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/


#endif /* DD_SD_CFG_H */
