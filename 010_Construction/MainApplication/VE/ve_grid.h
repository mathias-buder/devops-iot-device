/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file ve_grid.h
        @details Vibration grid module implementation

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
#define VE_GRID_LOG_MSG_TAG "VE_GRID" /**< @details Domain log message tag string */

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
    VE_GRID_VIBRATOR_11,     /**< @details Vibrator 11 */
    VE_GRID_VIBRATOR_SIZE    /**< @details Maximum number of vibrators */
} VE_GRID_VIBRATOR;

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief   Vibration data structure
 * @details Contains all data required handle the internal
 *          state for each vibrator
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_VIBRATOR_TYPE_TAG
{
    POINT_2D_TYPE position_s;           /**< @details Vibrator x/y-distance to reference point within grid @unit [mm] */
    F32           pwm_duty_cycle_f32;   /**< @details PW duty cycle @unit [%] */
    F32           dist_to_vp_f32;       /**< @details Euclidean distance to to virtual point @unit mm % */
    BOOLEAN       is_inside_act_area_b; /**< @details Indicates whether it is inside the activation area */
} VE_GRID_VIBRATOR_TYPE;

/**
 * @brief   Virtual vibration point within grid
 * @details Contains position and intensity of the
 *          virtual vibration point within the vibration grid
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_VIRTUAL_POINT_TYPE_TAG
{
    POINT_2D_TYPE position_s;    /**< @details Vibrator x/y-distance to reference point within grid @unit [mm] */
    F32           intensity_f32; /**< @details Vibration intensity @unit % */
} VE_GRID_VIRTUAL_POINT_TYPE;

/**
 * @brief   Virtual vibration point properties
 * @details Contains all data to define the virtual vibration point
 *          within the vibration grid
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_VIRTUAL_POINT_PROPS_TYPE_TAG
{
    VE_GRID_VIRTUAL_POINT_TYPE point_s;       /**< @details Virtual point structure */
    F32                        rx_f32;        /**< @details Ellipse radius in x direction @unit mm */
    F32                        ry_f32;        /**< @details Ellipse radius in y direction @unit mm */
} VE_GRID_VIRTUAL_POINT_PROPS_TYPE;

/**
 * @brief   Vibration grid configuration data structure
 * @details Contains all parameters required to configure
 *          Engine Grid module.
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_CONFIG_TYPE_TAG
{
    POINT_2D_TYPE vibrator_pos_vs[VE_GRID_VIBRATOR_SIZE];
    F32                       vp_size_x_f32;
    F32                       vp_size_y_f32;
} VE_GRID_CONFIG_TYPE;

/**
 * @brief   Vibration grid input interface data structure
 * @details Data structure that holds all data required by
 *          the Vibration grid.
 * @ingroup VibrationEngineStructures
 */
typedef struct VE_GRID_DATA_IN_TYPE_TAG
{
    VE_GRID_VIRTUAL_POINT_TYPE virtual_point_s;
} VE_GRID_DATA_IN_TYPE;

/**
 * @brief   Vibration grid output interface data structure
 * @details Data structure that holds all data provided by
 *          the Vibration grid.
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
    VE_GRID_DATA_OUT_TYPE            data_out_s;                         /**< @details Unit output data structure */
    VE_GRID_VIBRATOR_TYPE            vibrator_vs[VE_GRID_VIBRATOR_SIZE]; /**< @details Internal vibrator management structure */
    VE_GRID_VIRTUAL_POINT_PROPS_TYPE virtual_point_props_s;              /**< @details Virtual vibration point */

  public:
    /**
     * @details Default constructor
     */
    VE_GRID_C( void );

    /**
     * @details Default destructor
     */
    ~VE_GRID_C();

    /**
     * @details This function initializes the vibration grid
     *          with default configuration
     * @return Pointer to global device data out structure
     */
    VE_GRID_DATA_OUT_TYPE* init( void );

    /**
     * @details This function initializes the vibration grid
     * @param[in] r_config_s Reference to device configuration structure
     * @return Pointer to global device data out structure
     */
    VE_GRID_DATA_OUT_TYPE* init( VE_GRID_CONFIG_TYPE& r_config_s );

    /**
     * @details This is the unit main function that shall be called
     * cyclicly and will provide all data through VE_GRID_DATA_OUT_TYPE
     */
    void                   main( VE_GRID_DATA_IN_TYPE& r_data_in_s );

    /**
     * @details This function computes the euclidean distance between
     *          a vibrator and virtual point
     * @param[in] r_vib_pos_s Reference to vibrator position @UNIT mm
     * @param[in] r_vp_pos_s Reference to virtual point position @UNIT mm
     * @param[out] r_dx_square_f32 Reference to squared x-distance between
     *                             vibrator and virtual point @UNIT mm
     * @param[out] r_dx_square_f32 Reference to squared y-distance between
     *                             vibrator and virtual point @UNIT mm
     * @return Euclidean distance @UNIT mm
     */
    F32 compute_euclidean_distance( POINT_2D_TYPE& r_vib_pos_s,
                                    POINT_2D_TYPE& r_vp_pos_s,
                                    F32&           r_dx_square_f32,
                                    F32&           r_dy_square_f32 );

    /**
     * @details This function checks if a vibrator is within the ellispe area.
     * @param[in] dx_square_f32 Squared x-distance between vibrator and virtual point @unit mm
     * @param[in] dx_square_f32 Squared y-distance between vibrator and virtual point @unit mm
     * @param[in] rx_f32 Ellipse x-radius @unit mm
     * @param[in] ry_f32 Ellipse y-radius @unit mm
     * @return Flag that indicates whether the vibrator is inside the ellipse or not
     */

    BOOLEAN check_point_inisde_ellipse( F32 dx_square_f32,
                                        F32 dy_square_f32,
                                        F32 rx_f32,
                                        F32 ry_f32 );

    /**
     * @details This function returns a reference to the virtual
     *          point structure
     */
    VE_GRID_VIRTUAL_POINT_PROPS_TYPE& get_virtual_point( void );
};

#endif /* VE_GRID_H_ */
