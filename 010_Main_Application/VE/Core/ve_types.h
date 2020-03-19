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
 * @file dd_types.h
 * @details This files defines the domain specific types
 * used in the device driver domain.
 */

#ifndef DD_TYPES_H
#define DD_TYPES_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include <esp_err.h>

#include "../../types.h"
#include "../Config/ve_types_cfg.h"
#include "../Interface/ve_public_types.h"


/*************************************************************/
/*                      ENUMERATORS                           */
/*************************************************************/

/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup SensorStructures
 */
typedef struct VE_DATA_TAG
{
    U8                      chip_id_u8;                                  /** @details Unique chip id */
    F32                     temperature_deg_f32;                         /** @details Internal core (die) temperature @unit °C */
} VE_DATA;

#endif /* DD_TYPES_H */
