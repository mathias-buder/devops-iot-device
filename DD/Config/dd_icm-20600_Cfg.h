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
 * @file dd_icm-20600_Cfg.h
 * @details Some detailed description
 */

#ifndef DD_ICM_20600_CFG_H
#define DD_ICM_20600_CFG_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_types_Cfg.h"

/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

#define DD_ICM_20600_I2C_ADDR               0x68 /* addr-pin low (GND): 0x68, addr-pin high (VCC): 0x69 */
#define DD_ICM_20600_DEVICE_ID              0x73 /* Unique device id */

#define DD_ICM_20600_A_SCALSE               DD_ICM_20600_AFS_2G
#define DD_ICM_20600_G_SCALSE               DD_ICM_20600_GFS_250DPS
#define DD_ICM_20600_ALLOWED_FAC_DEVIATION  (1.0F) /* % */


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/**
 * @details This function returns pointer to the osl object for the given id
 * of the linked environment object.
 * @param[in] linked_env_id_u8 is id of the linked environment object of the requested osl object
 * @return pointer to osl object or NULL if no appropriate object exists.
 */

#endif /* DD_ICM_20600_CFG_H */
