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
#define DD_TMP_102_PTR_TEMP                   0x00       /**< Temperature Register (Read Only) */
#define DD_TMP_102_PTR_CONFIG                 0x01       /**< Configuration Register (Read/Write) */
#define DD_TMP_102_PTR_TEMP_LOW               0x02       /**< T_LOW Register (Read/Write) */
#define DD_TMP_102_PTR_TEMP_HIGH              0x03       /**< T_HIGH Register (Read/Write) */

#define DD_TMP_102_TEMP_LSB                  ( 0.0625F ) /**< Temperature ADC LSB @unit °C */

/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
extern BOOLEAN dd_tmp_102_init( void );
extern void    dd_tmp_102_main( void );

#endif /* DD_TMP_102_H_ */
