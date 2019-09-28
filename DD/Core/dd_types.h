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
 * @file osl_aeb_types.h
 * @details This files defines the domain specific types
 * used in the Object Selection domain.
 */

#ifndef DD_TYPES_H
#define DD_TYPES_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../Config/dd_types_Cfg.h"
#include "../Interface/dd_public_types.h"


/*************************************************************/
/*      ENUMERATORS                                          */
/*************************************************************/

/**
 * @details enumerator of impeding objects.
 */
//typedef enum OSL_AEB_IMPEDING_OBJECT_TYPE_TAG
//{
//    OSL_AEB_IMPEDING_OBJECT_MCP,        /**< @details Most Critical Pedestrian */
//    OSL_AEB_IMPEDING_OBJECT_MCB,        /**< @details Most Critical Bicyclist  */
//    OSL_AEB_IMPEDING_OBJECT_MCV,        /**< @details Most Critical Vehicle    */
//    OSL_AEB_IMPEDING_OBJECT_DIMENSION   /**< @details Dimension, last entry, number of impeding object types. */
//} OSL_AEB_IMPEDING_OBJECT_TYPE;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief   Object Selection Input Interface Vehicle Structure
 * @details The Object Selection Input Interface Vehicle Structure gathers vehicle related
            information such as car characteristics and dynamics.
 * @ingroup VehiStructs
 * @sa      TRACK/declar.h
 * @sa      CVD/lp_off.h
 * @sa      INFOBASE/INFOBASE.h
 * @sa      FUSION/Config/osl_aeb_input_interface.h
 */
//typedef struct OSL_AEB_INPUT_VEHICLE_DATA_TAG
//{
//    F32             yawrate_cc_f32;             /**< @details ego yaw rate, positive to left [rad/s] */
//    F32             pt2_acceleration_f32;       /**< @details ego acceleration [m/s^2]*/
//    F32             speed_f32;                  /**< @details ego speed [m/s] */
//} OSL_AEB_INPUT_VEHICLE_DATA;

/**
 * @details struct type defining the attributes for Object Input from Object fusion.
 */
//typedef struct OSL_AEB_INPUT_OBJ_STRUCT_TAG   /*  osl input object struct */
//{
//    U8                                  env_id_u8;
//    OF_CLASS                            class_e;                                                    /** @details object class (pedestrian, truck, ...) */
//    STATE_STRUCT                        Kalman_state_s;                                             /** @details structure containing the state of the object's associated Kalman model*/
//    F32                                 width_f32;                                                  /** @details width state. */
//    BOOLEAN                             is_oncoming_b;                                              /** @details indicates whether the object is oncoming or not */
//    F32                                 video_confidence_f32;                                       /** @details objects video confidence                             */
//    F32                                 radar_confidence_f32;                                       /** @details objects radar confidence                             */
//    OSL_AEB_INTEGRITY_LEVEL             last_integrity_level_e;                                     /** @details objects integrity level of previous cycle            */
//} OSL_AEB_INPUT_OBJECT_STRUCT;



#endif /* DD_TYPES_H */
