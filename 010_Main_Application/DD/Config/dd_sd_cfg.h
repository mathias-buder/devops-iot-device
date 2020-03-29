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
/* Log message TAG string */
#define DD_SD_LOG_MSG_TAG               "DD_SD"

/* Hard-wired pins to SD card */
#define DD_SD_MISO_PIN                  GPIO_NUM_19
#define DD_SD_MOSI_PIN                  GPIO_NUM_23
#define DD_SD_CLK_PIN                   GPIO_NUM_18
#define DD_SD_CS_PIN                    GPIO_NUM_5

/* File system properties */
#define DD_SD_MOUNT_POINT               "/sdcard"
#define DD_SD_FORMAT_ON_FAILED_MOUNT    FALSE
#define DD_SD_MAX_OPEN_FILES            5U
#define DD_SD_ALLOC_UNIT_SIZE           ( 16U * 1024U )

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/


#endif /* DD_SD_CFG_H */
