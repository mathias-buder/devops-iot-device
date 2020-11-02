/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file OS.h

*********************************************************************/

#ifndef OS_H_
#define OS_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"

#include "freertos/FreeRTOS.h"

#include "os_ota.h"
#include "os_sc.h"
#include "os_tm.h"
#include "os_ws.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
/* Log message TAG string */
#define OS_LOG_MSG_TAG                 "OS"

#define OS_MAIN_CYCLE_TIME_MS          100U   /* 100 ms cycle time ( minimum cycle time = portTICK_PERIOD_MS ) */

#define OS_MAIN_CYCLE_TIME_SEC         ( OS_MAIN_CYCLE_TIME_MS / 1000.0F )

#define OS_MAIN_CYCLE_TIME_INCREMENT   ( OS_MAIN_CYCLE_TIME_MS / portTICK_PERIOD_MS )

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

#endif /* OS_H_ */
