/*********************************************************************
    MODULE SPECIFICATION:

        File: ts_types.c

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

#ifndef TS_TYPES_H
#define TS_TYPES_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/

#include "../Config/ts_types_Cfg.h"
#include "../Interface/osl_aeb_external.h"

#include "../Interface/ts_public_types.h"

/*************************************************************/
/*      ENUMERATORS                                          */
/*************************************************************/

/**
 * @details enumerator of impeding objects.
 */
typedef enum OSL_AEB_IMPEDING_OBJECT_TYPE_TAG
{
    OSL_AEB_IMPEDING_OBJECT_MCP,        /**< @details Most Critical Pedestrian */
    OSL_AEB_IMPEDING_OBJECT_MCB,        /**< @details Most Critical Bicyclist  */
    OSL_AEB_IMPEDING_OBJECT_MCV,        /**< @details Most Critical Vehicle    */
    OSL_AEB_IMPEDING_OBJECT_DIMENSION   /**< @details Dimension, last entry, number of impeding object types. */
} OSL_AEB_IMPEDING_OBJECT_TYPE;

/**
 * @details enumerator of selection area type
 */
typedef enum OSL_AEB_SELECTION_AREA_TYPE_TAG
{
    OSL_AEB_SELECTION_AREA_OUTSIDE      = 0,    /**< @details Outside selection path */
    OSL_AEB_SELECTION_AREA_INSIDE_CW,           /**< @details Inside collision warning (CW) path */
    OSL_AEB_SELECTION_AREA_INSIDE_EB,           /**< @details Inside emergency breaking warning (EB) path */
    OSL_AEB_SELECTION_AREA_TYPE_DIMENSION       /**< @details Dimension, last entry, number of */
}OSL_AEB_SELECTION_AREA_TYPE;

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
typedef struct OSL_AEB_INPUT_VEHICLE_DATA_TAG
{
    F32             yawrate_cc_f32;             /**< @details ego yaw rate, positive to left [rad/s] */
    F32             pt2_acceleration_f32;       /**< @details ego acceleration [m/s^2]*/
    F32             speed_f32;                  /**< @details ego speed [m/s] */
} OSL_AEB_INPUT_VEHICLE_DATA;

/**
 * @details struct type defining the attributes for Object Input from Object fusion.
 */
typedef struct OSL_AEB_INPUT_OBJ_STRUCT_TAG   /*  osl input object struct */
{
    U8                                  env_id_u8;
    OF_CLASS                            class_e;                                                    /** @details object class (pedestrian, truck, ...) */
    STATE_STRUCT                        Kalman_state_s;                                             /** @details structure containing the state of the object's associated Kalman model*/
    F32                                 width_f32;                                                  /** @details width state. */
    BOOLEAN                             is_oncoming_b;                                              /** @details indicates whether the object is oncoming or not */
    F32                                 video_confidence_f32;                                       /** @details objects video confidence                             */
    F32                                 radar_confidence_f32;                                       /** @details objects radar confidence                             */
    OSL_AEB_INTEGRITY_LEVEL             last_integrity_level_e;                                     /** @details objects integrity level of previous cycle            */
} OSL_AEB_INPUT_OBJECT_STRUCT;

typedef struct OSL_AEB_INPUT_DATA_TAG
{
    OSL_AEB_INPUT_VEHICLE_DATA     vehi_s;                                                          /** @details Object Selection Input Interface Vehicle Structure */
    OSL_AEB_INPUT_OBJECT_STRUCT    object_vs[OSL_AEB_MAX_NUMBER_OBJECTS];                           /** @details Object Selection Input Interface Object Structure */
    U8                             last_aeb_impeding_env_id_vu8[OSL_AEB_IMPEDING_OBJECT_DIMENSION];
    OSL_AEB_SELECTION_STATE        last_aeb_selection_state_ve[OSL_AEB_IMPEDING_OBJECT_DIMENSION];
    U8                             number_objects_u8;                                               /** @details number of valid objects */
} OSL_AEB_INPUT_DATA;

/**
 * @details
 */

typedef struct MCO_DATA_TAG
{
    F32                           TTX0_f32;                     /** @details time to x=0 in reference coordiantes */
    SCALAR_STATE_STRUCT           y_at_TTX0_state_s;            /** @details predicted lateral position at time to x=0 in reference coordiantes */
    F32                           y_pred_prob_in_path_f32;      /** @details probability of lateral prediction within a defined range */
    BOOLEAN                       inside_selection_area_b;      /** @details flag to show, if the object is within the selection area */
    BOOLEAN                       inside_critical_nearfield_b;  /** @details flag to show, if the object is within the critical nearfield */
    OSL_AEB_SELECTION_AREA_TYPE   selection_area_type_e;        /** @details enum to show the type of the current selection area */
    OSL_AEB_INTEGRITY_LEVEL       integrity_level_e;            /** @details enum to show the objects integrity level */
} MCO_DATA_STRUCT;

typedef struct OSL_AEB_OBJECT_TAG
{
    U8                                  env_id_u8;              /** @details unique id, same as from linked input object (copied) */
    OSL_AEB_INPUT_OBJECT_STRUCT*        p_input_object_s;       /** @details pointer to input environment objects                 */
    MCO_DATA_STRUCT                     mco_data_s;             /** @details struct for MCO specific data                         */
} OSL_AEB_OBJECT;

/**
 * @details struct type defining the attributes for Object Selection Environment Model.
 */
typedef struct OSL_AEB_ENVIRONMENT_STRUCT_TAG  /*  Object Selection environment struct */
{
    OSL_AEB_OBJECT               object_vs[OSL_AEB_MAX_NUMBER_OBJECTS];                          /**< @details array of objects for object selection */
    OSL_AEB_OBJECT*              impeding_objects_vp[OSL_AEB_IMPEDING_OBJECT_DIMENSION];         /**< @details array of pointers to special objects (ACC, NIV left/right, MCP) */
    U8                           number_objects_u8;
    OSL_AEB_SELECTION_STATE      aeb_selection_state_ve[OSL_AEB_IMPEDING_OBJECT_DIMENSION];
} OSL_AEB_ENVIRONMENT_STRUCT;

#endif /* TS_TYPES_H */
