/*********************************************************************
    MODULE SPECIFICATION:

        $URL$
        $Revision$
        $Author$
        $Date$

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2019

*********************************************************************/

/**
 * @file dd_i2c.h
 * @details Some detailed description
 */

#ifndef DD_I2C_H
#define DD_I2C_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <driver/i2c.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <esp_err.h>

#include "../Config/dd_i2c_Cfg.h"
#include "../Config/dd_types_Cfg.h"

#include "../Interface/dd_i2c_interface.h"


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/**
 * @details This function returns pointer to the osl object for the given id
 * of the linked environment object.
 * @param[in] linked_env_id_u8 is id of the linked environment object of the requested osl object
 * @return pointer to osl object or NULL if no appropriate object exists.
 */

BOOLEAN dd_i2c_init();

BOOLEAN dd_i2c_write_single( U8 device_addr_u8,
                             U8 register_addr_u8,
                             U8 data_u8 );

BOOLEAN dd_i2c_write_burst( U8  device_addr_u8,
                            U8  register_addr_u8,
                            U8 *p_data_u8,
                            U8  data_size_u8 );

U8 dd_i2c_read_burst( U8  device_addr_u8,
                      U8  register_addr_u8,
                      U8* p_data_u8,
                      U8  data_size_u8 );

BOOLEAN dd_i2c_read_bit( U8  device_addr_u8,
                         U8  register_addr_u8,
                         U8  bit_position_u8 );

BOOLEAN dd_i2c_read_modify_write( U8 device_addr_u8,
                                  U8 register_addr_u8,
                                  U8 bit_position_u8,
                                  BOOLEAN bit_value_b );


#endif /* DD_I2C_H */
