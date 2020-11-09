/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_tmp_102.h
        @details Some detailed description

*********************************************************************/
#ifndef DD_TMP_102_H_
#define DD_TMP_102_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_types.h"
#include "../Config/dd_tmp-102_cfg.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
/* Pointer Register Byte */
#define DD_TMP_102_TEMP                 0x00U       /**< Temperature Register (Read Only) */
#define DD_TMP_102_CONFIG               0x01U       /**< Configuration Register (Read/Write) */
#define DD_TMP_102_TEMP_LOW             0x02U       /**< T_LOW Register (Read/Write) */
#define DD_TMP_102_TEMP_HIGH            0x03U       /**< T_HIGH Register (Read/Write) */

/* Configuration register */
#define DD_TMP_102_CONFIG_OS_ENABLE     0x8000U
#define DD_TMP_102_CONFIG_OS_DISABLE    0x0000U
#define DD_TMP_102_CONFIG_F_1FAULT      0x0000U
#define DD_TMP_102_CONFIG_F_2FAULTS     0x0800U
#define DD_TMP_102_CONFIG_F_4FAULTS     0x1000U
#define DD_TMP_102_CONFIG_F_6FAULTS     0x1800U
#define DD_TMP_102_CONFIG_POL_LOW       0x0000U
#define DD_TMP_102_CONFIG_POL_HIGH      0x0400U
#define DD_TMP_102_CONFIG_TM_COMP       0x0000U
#define DD_TMP_102_CONFIG_TM_TSTAT      0x0200U
#define DD_TMP_102_CONFIG_SD_CC         0x0000U
#define DD_TMP_102_CONFIG_SD_SD         0x0100U
#define DD_TMP_102_CONFIG_CR_0P25HZ     0x0000U
#define DD_TMP_102_CONFIG_CR_1HZ        0x0040U
#define DD_TMP_102_CONFIG_CR_4HZ        0x0080U
#define DD_TMP_102_CONFIG_CR_8HZ        0x00C0U
#define DD_TMP_102_CONFIG_AL_ENABLE     0x0020U
#define DD_TMP_102_CONFIG_AL_DISABLE    0x0000U
#define DD_TMP_102_CONFIG_EM_12BIT      0x0000U
#define DD_TMP_102_CONFIG_EM_13BIT      0x0010U

/* ADC specific */
#define DD_TMP_102_TEMP_LSB                  ( 0.0625F ) /**< Temperature ADC LSB @unit °C */

/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
extern BOOLEAN dd_tmp_102_init( void );
extern void    dd_tmp_102_main( void );

#endif /* DD_TMP_102_H_ */
