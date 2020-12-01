/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file ve_grid.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef VE_GRID_H_
#define VE_GRID_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../types.h"


/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
#define VE_GRID_LOG_MSG_TAG              "VE_GRID"



/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/



/*************************************************************
*       ENUMERATORS                                          *
*************************************************************/
/**
 * @details Vibrator number definition
 */
typedef enum VE_GRID_VIBRATOR_TAG
{
    VE_GRID_VIBRATOR_1 = 0U, /**< @details Vibrator 1  */
    VE_GRID_VIBRATOR_2,      /**< @details Vibrator 2  */
    VE_GRID_VIBRATOR_3,      /**< @details Vibrator 3  */
    VE_GRID_VIBRATOR_4,      /**< @details Vibrator 4  */
    VE_GRID_VIBRATOR_5,      /**< @details Vibrator 5  */
    VE_GRID_VIBRATOR_6,      /**< @details Vibrator 6  */
    VE_GRID_VIBRATOR_7,      /**< @details Vibrator 7  */
    VE_GRID_VIBRATOR_8,      /**< @details Vibrator 8  */
    VE_GRID_VIBRATOR_9,      /**< @details Vibrator 9  */
    VE_GRID_VIBRATOR_10,     /**< @details Vibrator 10 */
    VE_GRID_VIBRATOR_SIZE    /**< @details Maximum number of vibrators */
} VE_GRID_VIBRATOR;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/
typedef struct VE_GRID_VIRBATOR_TYPE_TAG
{
    F32 x_f32; /**< @details Vibrator x-distance to reference point within grid @unit [mm] */
    F32 y_f32; /**< @details Vibrator y-distance to reference point within grid @unit [mm] */
    F32 pwm_duty_cycle_f32;
} VE_GRID_VIRBATOR_TYPE;



typedef struct VE_GRID_VIRBATOR_POS_TYPE_TAG
{
    F32 x_f32; /**< @details Vibrator x-distance to reference point within grid @unit [mm] */
    F32 y_f32; /**< @details Vibrator y-distance to reference point within grid @unit [mm] */
} VE_GRID_VIRBATOR_POS_TYPE;



/**
 * @brief   Vibration Engine Grid Output Interface Data Structure
 * @details Data structure that holds all data provided by the Vibration
 *          Engine Grid module.
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_VIRTUAL_POINT_TYPE_TAG
{
    F32 x_f32;         /**< @details Vibration x coordinate @unit mm */
    F32 y_f32;         /**< @details Vibration y coordinate  @unit mm */
    F32 intensity_f32; /**< @details Vibration intensity @unit % */
} VE_GRID_VIRTUAL_POINT_TYPE;

/**
 * @brief   Vibration Engine Grid Output Interface Data Structure
 * @details Data structure that holds all data provided by the Vibration
 *          Engine Grid module.
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_CONFIG_TYPE_TAG
{
    VE_GRID_VIRBATOR_POS_TYPE vibrator_vs[VE_GRID_VIBRATOR_SIZE];
} VE_GRID_CONFIG_TYPE;

/**
 * @brief   Vibration Engine Grid Output Interface Data Structure
 * @details Data structure that holds all data provided by the Vibration
 *          Engine Grid module.
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_DATA_IN_TYPE_TAG
{
    VE_GRID_VIRTUAL_POINT_TYPE virtual_point_s;

} VE_GRID_DATA_IN_TYPE;


/**
 * @brief   Vibration Engine Grid Output Interface Data Structure
 * @details Data structure that holds all data provided by the Vibration
 *          Engine Grid module.
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_DATA_OUT_TYPE_TAG
{
    F32 duty_cycle_vf32[VE_GRID_VIBRATOR_SIZE];

} VE_GRID_DATA_OUT_TYPE;

/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/

class VE_GRID_C {
  private:
    VE_GRID_DATA_OUT_TYPE data_out_s;

  public:
    /**
         * @details Default constructor
         */
    VE_GRID_C( void );

    /**
         * @details Default destructor
         */
    ~VE_GRID_C();

    BOOLEAN                init( void );
    BOOLEAN                init( VE_GRID_CONFIG_TYPE& r_config_s );
    VE_GRID_DATA_OUT_TYPE* main( VE_GRID_DATA_IN_TYPE& r_data_in_s );
};

#endif /* VE_GRID_H_ */
