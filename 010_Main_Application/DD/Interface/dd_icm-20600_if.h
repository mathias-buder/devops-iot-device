/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_icm-20600_if.h
        @details Some detailed description

*********************************************************************/
#ifndef DD_ICM_20600_IF_H
#define DD_ICM_20600_IF_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../Core/dd_icm-20600.h"


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/**
 * @details This function will write the current acceleration raw data into
 * p_data_u16
 * @param[in] p_data_u16 pointer to the acceleration raw data to be written to
 * @param[in] type_e type of acceleration data [x, y ,z]
 * @return flag to indicate if data stored in p_data_u16 shall be used or not
 */
BOOLEAN dd_icm_20600_get_accl_raw_data( U16*                    p_data_u16,
                                        DD_ICM_20600_ACCEL_TYPE type_e );

/**
 * @details This function will write the current gyro raw data into
 * p_data_u16
 * @param[in] p_data_u16 pointer to the gyro raw data to be written to
 * @param[in] type_e type of gyro data [x, y ,z]
 * @return flag to indicate if data stored in p_data_u16 shall be used or not
 */
BOOLEAN dd_icm_20600_get_gyro_raw_data( U16*                   p_data_u16,
                                        DD_ICM_20600_GYRO_TYPE type_e );

/**
 * @details This function will return the pointer to the DD_ICM_20600_DATA
 * data structure
 * @return pointer to the DD_ICM_20600_DATA data structure
 */
DD_ICM_20600_DATA* dd_icm_20600_get_database( void );
#endif /* DD_ICM_20600_IF_H */
