/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_pga-302_cfg.h
        @details Some detailed description

*********************************************************************/

#ifndef DD_PGA_302_CFG_H
#define DD_PGA_302_CFG_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../Core/dd_types.h"


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/
/* Log message TAG string */
#define DD_PGA_302_LOG_MSG_TAG                    "DD_PGA_302"

/* I2C configuration
 * The I2C digital interface (DI) is organized in different memory
 * pages which requires dedicated address offsets to access the
 * a certain memory page */

#define DD_PGA_302_I2C_ADDR_TEST_REG              0x40     /* PGA-302 I2C slave address: test registers */
#define DD_PGA_302_I2C_ADDR_DI_OFFSET_2           0x42     /* PGA-302 I2C slave address: test control and status register, offset: 0x02 */
#define DD_PGA_302_I2C_ADDR_DI_OFFSET_7           0x47     /* PGA-302 I2C slave address: test control and status register, offset: 0x07 */
#define DD_PGA_302_I2C_ADDR_DI_OFFSET_EEPROM      0x45     /* PGA-302 I2C address offset: EEPROM cach + cells */

// #define DD_PGA_302_DEVICE_ID                      0x00 /* Unique device id */

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/


#endif /* DD_PGA_302_CFG_H */
