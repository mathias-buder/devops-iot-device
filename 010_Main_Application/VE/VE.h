/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file VE.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/
#ifndef VE_H_
#define VE_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "ve_grid.h"

/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
#define VE_LOG_MSG_TAG       "VE"        /**< @details Domain log message tag string */


/*************************************************************
*      STRUCTURES                                            *
*************************************************************/
/**
 * @brief   Device Driver Input Structure
 * @details Contains all data that is required by the Device Driver (DD) domain
 * @ingroup DriverStructures
 */
typedef struct VE_DATA_IN_TYPE_TAG
{
    VE_GRID_DATA_IN_TYPE ve_grid_data_in_s;
} VE_DATA_IN_TYPE;

/**
 * @brief   Device Driver Output Data Structure
 * @details Contains all data provided by the Device Driver (DD) domain
 * @ingroup DriverStructures
 */
typedef struct VE_DATA_OUT_TYPE_TAG
{
    const VE_GRID_DATA_OUT_TYPE* p_grid_data_out_s;
} VE_DATA_OUT_TYPE;

/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/
class VE_C {

  private:
    static VE_DATA_OUT_TYPE data_out_s; /**< @details Contains all data provided by the Device Driver (DD) domain */

  public:
    /**
     * @details This function initialized the INA-219 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    static void init( void );

    /**
     * @details This function initialized the INA-219 device
     * @param[in] Pointer device input structure
     * @return Pointer to global device data structure
     */
    static VE_DATA_OUT_TYPE main( VE_DATA_IN_TYPE &r_data_in_s );
};

#endif /* VE_H_ */
