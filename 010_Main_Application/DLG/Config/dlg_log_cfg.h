/*********************************************************************
    MODULE SPECIFICATION:

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dlg_log_cfg.h
        @details Some detailed description

*********************************************************************/
#ifndef DLG_CONFIG_LOG_CFG_H_
#define DLG_CONFIG_LOG_CFG_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

/* Log message tag string */
#define DLG_LOG_LOG_MSG_TAG                      "DLG_LOG"

/* File name specific */
#define DLG_LOG_DEV_SERIAL_NO                    "XC12C45D"    /**< Device specific serial number string (will be provided by Non-volatile storage library in the future) */
#define DLG_LOG_MAX_FILE_NAME_LENGTH             30U
#define DLG_LOG_FILE_DOT_EXTENSION               ".sbf"        /**< Format: .sbf: (s)ewela-(b)inary-(f)ile */
#define DLG_LOG_NUM_FILES_PER_MINUTE             6U            /**< Number of .sbf files per minute */
#define DLG_LOG_FILE_SIZE_IN_KBYTE               20U           /**< File size in kByte */
#define DLG_LOG_MAIN_CYCLE_TIME_MS               100U          /* Main loop cycle time -> will be replaced with value provided by os_time module in the future */

/* Flag to enable logging test mode.
 * If defined every signal will be overwritten by
 * log_test_mode_cnt_u8 with an value of 0 <= n <= 255*/
// #define DLG_LOG_TEST_ENABLE

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
extern BOOLEAN dlg_log_use_files_per_minute_cfg_b;

#endif /* DLG_CONFIG_LOG_CFG_H_ */
