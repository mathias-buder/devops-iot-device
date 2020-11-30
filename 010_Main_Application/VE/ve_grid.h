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
/*      STRUCTURES                                           */
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


typedef struct VE_GRID_VIRTUAL_POINT_TYPE_TAG
{
    F32 x_f32;
    F32 y_f32;
    F32 intensity_f32; /**< @details Channel duty cycle @unit [%] */

} VE_GRID_VIRTUAL_POINT_TYPE;


typedef struct VE_GRID_CONFIG_TYPE_TAG
{

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

    VE_GRID_DATA_OUT_TYPE* init( VE_GRID_CONFIG_TYPE& r_config_s );
    void                   main( VE_GRID_DATA_IN_TYPE& r_data_in_s );
};

#endif /* VE_GRID_H_ */
