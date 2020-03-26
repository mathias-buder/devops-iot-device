/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_i2c_if.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_I2C_IF_H
#define DD_I2C_IF_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "esp_log.h"

#include "dd_public_types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
DD_I2C_ERROR_INFO_TYPE* dd_i2c_get_last_error( void );
DD_I2C_ERROR_TYPE*      dd_i2c_get_error_database( void );

#endif /* DD_I2C_IF_H */
