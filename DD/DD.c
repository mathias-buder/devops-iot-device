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

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "DD.h"

#include "Core/dd_i2c.h"
#include "Core/dd_icm-20600.h"
#include "Core/dd_database.h"

/* Need to be removed later, only for testing purpose */
#include "Config/dd_icm-20600_Cfg.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

void dd_init(void)
{
    /* Initialize I2C basic device driver */
    dd_i2c_init();
}


void dd_main(void)
{

    /* Read all required sensor data form ICM-20600 */
    dd_icm_20600_acquire_sensor_data();

    printf("Temperature %0.2f\n", dd_icm_20600_output_s.temperature_deg_f32 );

    printf("X: %i, Y: %i, Z: %i\nX: %i, Y: %i, Z: %i\n", dd_icm_20600_output_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_X],
                                                         dd_icm_20600_output_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_Y],
                                                         dd_icm_20600_output_s.accel_data_raw_u16[DD_ICM_20600_ACCEL_Z],
                                                         dd_icm_20600_output_s.gyro_data_raw_u16[DD_ICM_20600_GYRO_X],
                                                         dd_icm_20600_output_s.gyro_data_raw_u16[DD_ICM_20600_GYRO_Y],
                                                         dd_icm_20600_output_s.gyro_data_raw_u16[DD_ICM_20600_GYRO_Z] );


}
