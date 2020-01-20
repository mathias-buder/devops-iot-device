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

/**
 * @file dd_i2c_Cfg.h
 * @details Some detailed description
 */

#ifndef DD_I2C_CFG_H
#define DD_I2C_CFG_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <driver/i2c.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

#include "../../types.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

#define DD_I2C_OPERATION_MODE                       I2C_MODE_MASTER

#define DD_I2C_PORT_NUM                             I2C_NUM_0

#define DD_I2C_SDA_IO_NUM                           GPIO_NUM_21

#define DD_I2C_SCL_IO_NUM                           GPIO_NUM_22

#define DD_I2C_SDA_PULLUP_CFG                       GPIO_PULLUP_ENABLE

#define DD_I2C_SCL_PULLUP_CFG                       GPIO_PULLUP_ENABLE

#define DD_I2C_SCL_FREQ                             (100000)  /*  100 kHz  */

#define DD_I2C_BUS_BUSY_TIME_OUT                    (1000 / portTICK_PERIOD_MS)

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/



#endif /* DD_I2C_CFG_H */
