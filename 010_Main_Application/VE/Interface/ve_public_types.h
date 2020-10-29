/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file ve_public_types.h
        @details ...

        (c) SEWELA 2020

*********************************************************************/

#ifndef VE_PUBLIC_TYPES_H_
#define VE_PUBLIC_TYPES_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../DD/DD.h"

/*************************************************************/
/*                        VE_GRID                            */
/*************************************************************/

/*************************************************************/
/*                      ENUMERATORS                          */
/*************************************************************/


/*************************************************************/
/*                      STRUCTURES                           */
/*************************************************************/
typedef struct VE_GRID_CHANNEL_TAG
{
    DD_MCPWM_CHANNEL_NUM assignment_e;   /**< @details Channel assignment */
    F32                  duty_cycle_f32; /**< @details Channel duty cycle @unit [%] */
    F32                  x_f32;          /**< @details Vibrator x-distance to reference
                                                       point within grid @unit [mm] */
    F32                  y_f32;          /**< @details Vibrator y-distance to reference
                                                       point within grid @unit [mm] */
} VE_GRID_CHANNEL;

/**
 * @brief   Vibration Engine Grid Output Interface Data Structure
 * @details Data structure that holds all data provided by the Vibration
 *          Engine Grid module.
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_DATA_TAG
{
    VE_GRID_CHANNEL* p_channel_s;

} VE_GRID_DATA;

#endif /* VE_PUBLIC_TYPES_H_ */

