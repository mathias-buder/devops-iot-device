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
#include "../Config/dd_types_Cfg.h"
#include "../Interface/dd_public_types.h"


/*************************************************************/
/*      ENUMERATORS                                          */
/*************************************************************/


/**
 * @details enumerator of ...
 */
typedef enum DD_I2C_ERROR_TAG
{
    DD_I2C_ERROR_FAIL         = ESP_FAIL,             /*!< Generic esp_err_t code indicating failure */
    DD_I2C_ERROR_OK           = ESP_OK,               /*!< esp_err_t value indicating success (no error) */
    DD_I2C_ERROR_INVALID_SIZE = ESP_ERR_INVALID_SIZE,
    DD_I2C_ERROR_INVALID_ARG  = ESP_ERR_INVALID_ARG
}DD_I2C_ERROR;


/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_SCALE_A_TAG
{
    DD_ICM_20600_AFS_2G = 0,        /**< @details ... */
    DD_ICM_20600_AFS_4G,            /**< @details ... */
    DD_ICM_20600_AFS_8G,            /**< @details ... */
    DD_ICM_20600_AFS_16G,            /**< @details ... */
    DD_ICM_20600_SCALE_A_SIZE
}DD_ICM_20600_SCALE_A;

/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_SCALE_G_TAG
{
    DD_ICM_20600_GFS_250DPS = 0,    /**< @details ... */
    DD_ICM_20600_GFS_500DPS,        /**< @details ... */
    DD_ICM_20600_GFS_1000DPS,       /**< @details ... */
    DD_ICM_20600_GFS_2000DPS,        /**< @details ... */
    DD_ICM_20600_SCALE_G_SIZE
}DD_ICM_20600_SCALE_G;


/**
 * @details enumerator of ...
 */
typedef enum DD_ICM_20600_ACCEL_TYPE_TAG
{
    DD_ICM_20600_ACCEL_X = 0,    /**< @details ... */
    DD_ICM_20600_ACCEL_Y,    /**< @details ... */
    DD_ICM_20600_ACCEL_Z,    /**< @details ... */
    DD_ICM_20600_ACCEL_SIZE
}DD_ICM_20600_ACCEL_TYPE;


/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/

/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup SensorStructures

 */
typedef struct DD_ICM_20600_DATA_TYPE_TAG
{
    F32 temperature_f32;                /** @details Internal core (die) temperature */

} DD_ICM_20600_DATA_TYPE;


typedef struct DD_I2C_ERROR_TYPE_TAG
{
    DD_I2C_ERROR current_t;
    DD_I2C_ERROR previous_t;
    BOOLEAN      state_b;
}DD_I2C_ERROR_TYPE;

#endif /* DD_TYPES_H */
