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
/**
 * @details enumerator of ...
 */
typedef enum VE_GRID_VIBRATOR_TAG
{
    VE_GRID_VIBRATOR_1,
    VE_GRID_VIBRATOR_2,
    VE_GRID_VIBRATOR_3,
    VE_GRID_VIBRATOR_4,
    VE_GRID_VIBRATOR_5,
    VE_GRID_VIBRATOR_6,
    VE_GRID_VIBRATOR_7,
    VE_GRID_VIBRATOR_8,
    VE_GRID_VIBRATOR_9,
    VE_GRID_VIBRATOR_10,
    VE_GRID_VIBRATOR_SIZE
} VE_GRID_VIBRATOR;

/*************************************************************/
/*                      STRUCTURES                           */
/*************************************************************/
typedef struct VE_GRID_VIRBATOR_TYPE_TAG
{
    const DD_MCPWM_CHANNEL assignment_e;   /**< @details Channel assignment */
    const F32              x_f32;          /**< @details Vibrator x-distance to reference
                                                                  point within grid @unit [mm] */
    const F32              y_f32;          /**< @details Vibrator y-distance to reference
                                                                  point within grid @unit [mm] */
    F32                    duty_cycle_f32; /**< @details Channel duty cycle @unit [%] */
} VE_GRID_VIRBATOR_TYPE;

/**
 * @brief   Vibration Engine Grid Output Interface Data Structure
 * @details Data structure that holds all data provided by the Vibration
 *          Engine Grid module.
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_DATA_TAG
{
    VE_GRID_VIRBATOR_TYPE* p_vibrator_config_s;

} VE_GRID_DATA;

#endif /* VE_PUBLIC_TYPES_H_ */

