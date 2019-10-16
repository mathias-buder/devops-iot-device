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
#include <stdio.h>
#include <math.h>

#include "dd_icm-20600.h"
#include "dd_i2c.h"
#include "dd_database.h"

#include "../Config/dd_icm-20600_Cfg.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE BOOLEAN dd_icm_20600_self_test( DD_ICM_20600_DATA* p_input_data_s );


/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

BOOLEAN dd_icm_20600_reset_soft(void)
{
    BOOLEAN state_b = FALSE;

    state_b = dd_i2c_read_modify_write( DD_ICM_20600_I2C_ADDR,
                                        DD_ICM_20600_PWR_MGMT_1,
                                        DD_ICM_20600_PWR_MGMT_1_DEVICE_RESET_B,
                                        TRUE );

    return state_b;
}


BOOLEAN dd_icm_20600_who_am_i_read( DD_ICM_20600_DATA* p_input_data_s )
{
    BOOLEAN state_b = FALSE;
    U8      register_value_u8;

    if( NULL != p_input_data_s )
    {
        state_b = dd_i2c_read_single( DD_ICM_20600_I2C_ADDR,
                                      DD_ICM_20600_WHO_AM_I,
                                     &register_value_u8 );
        /* Check for I2C error */
        if( FALSE != state_b )
        {
            p_input_data_s->chip_id_u8 = register_value_u8;
        }
        else
        {
            /* Reset chip id */
            p_input_data_s->chip_id_u8 = 0xFF;
        }
    }
    else
    {
        assert( NULL != p_input_data_s );
    }

    return state_b;
}


BOOLEAN dd_icm_20600_temperature_read( DD_ICM_20600_DATA* p_input_data_s )
{
    BOOLEAN state_b             = FALSE;
    U16     temperature_raw_u16 = 0xFFFF;
    F32     temperature_deg_f32 = -1000.0F;
    U8      register_data_v8[2];

    if( NULL != p_input_data_s)
    {
        state_b = dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR,
                                     DD_ICM_20600_TEMP_OUT_H,
                                     register_data_v8,
                                     sizeof(register_data_v8) );
        /* Check for I2C error */
        if( FALSE != state_b )
        {
            temperature_raw_u16 = ( register_data_v8[0] << 8U ) | register_data_v8[1];
            temperature_deg_f32 = ( temperature_raw_u16 * DD_ICM_20600_TEMP_SENSITIVITY_INV ) + DD_ICM_20600_TEMP_ROOM_OFFSET;
        }

        p_input_data_s->temperature_raw_u16 = temperature_raw_u16;
        p_input_data_s->temperature_deg_f32 = temperature_deg_f32;
    }
    else
    {
        assert( NULL != p_input_data_s );
    }

    return state_b;
}


BOOLEAN dd_icm_20600_accel_data_read_raw( DD_ICM_20600_DATA* p_input_data_s )
{
    BOOLEAN state_b = FALSE;
    U8      register_data_vu8[2 * DD_ICM_20600_ACCEL_SIZE];
    U16     accel_data_vu16[DD_ICM_20600_ACCEL_SIZE] = {0xFFFF, 0xFFFF, 0xFFFF};

    if( NULL != p_input_data_s)
    {
        state_b = dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR,
                                     DD_ICM_20600_ACCEL_XOUT_H,
                                     register_data_vu8,
                                     sizeof(register_data_vu8) );
        /* Check for I2C error */
        if( FALSE != state_b )
        {
            accel_data_vu16[DD_ICM_20600_ACCEL_X] = ( register_data_vu8[0] << 8U ) | register_data_vu8[1];
            accel_data_vu16[DD_ICM_20600_ACCEL_Y] = ( register_data_vu8[2] << 8U ) | register_data_vu8[3];
            accel_data_vu16[DD_ICM_20600_ACCEL_Z] = ( register_data_vu8[4] << 8U ) | register_data_vu8[5];
        }

        p_input_data_s->accel_data_raw_u16[DD_ICM_20600_ACCEL_X] = accel_data_vu16[DD_ICM_20600_ACCEL_X];
        p_input_data_s->accel_data_raw_u16[DD_ICM_20600_ACCEL_Y] = accel_data_vu16[DD_ICM_20600_ACCEL_Y];
        p_input_data_s->accel_data_raw_u16[DD_ICM_20600_ACCEL_Z] = accel_data_vu16[DD_ICM_20600_ACCEL_Z];
    }
    else
    {
        assert( NULL != p_input_data_s );
    }

    return state_b;
}


BOOLEAN dd_icm_20600_gyro_data_read_raw( DD_ICM_20600_DATA* p_input_data_s )
{
    BOOLEAN state_b = FALSE;
    U8      register_data_vu8[2 * DD_ICM_20600_ACCEL_SIZE];
    U16     gyro_data_vu16[DD_ICM_20600_ACCEL_SIZE] = {0xFFFF, 0xFFFF, 0xFFFF};

    if( NULL != p_input_data_s)
    {
        state_b = dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR,
                                     DD_ICM_20600_GYRO_XOUT_H,
                                     register_data_vu8,
                                     sizeof(register_data_vu8) );
        /* Check for I2C error */
        if( FALSE != state_b )
        {
            gyro_data_vu16[DD_ICM_20600_GYRO_X] = ( register_data_vu8[0] << 8U ) | register_data_vu8[1];
            gyro_data_vu16[DD_ICM_20600_GYRO_Y] = ( register_data_vu8[2] << 8U ) | register_data_vu8[3];
            gyro_data_vu16[DD_ICM_20600_GYRO_Z] = ( register_data_vu8[4] << 8U ) | register_data_vu8[5];
        }

        p_input_data_s->gyro_data_raw_u16[DD_ICM_20600_GYRO_X] = gyro_data_vu16[DD_ICM_20600_GYRO_X];
        p_input_data_s->gyro_data_raw_u16[DD_ICM_20600_GYRO_Y] = gyro_data_vu16[DD_ICM_20600_GYRO_Y];
        p_input_data_s->gyro_data_raw_u16[DD_ICM_20600_GYRO_Z] = gyro_data_vu16[DD_ICM_20600_GYRO_Z];
    }
    else
    {
        assert( NULL != p_input_data_s );
    }

    return state_b;
}


BOOLEAN dd_icm_20600_quaternion_update( DD_ICM_20600_QUATERNION* p_input_data_s,
                                        F32                      accel_x_f32,
                                        F32                      accel_y_f32,
                                        F32                      accel_z_f32,
                                        F32                      gyro_x_f32,
                                        F32                      gyro_y_f32,
                                        F32                      gyro_z_f32,
                                        F32                      delta_t_f32,
                                        F32                      zeta_f32,
                                        F32                      beta_f32 )
{
    BOOLEAN state_b = FALSE;

    /* Short name local variable for readability */
    F32 q1_f32 = p_input_data_s->Q1_f32;
    F32 q2_f32 = p_input_data_s->Q2_f32;
    F32 q3_f32 = p_input_data_s->Q3_f32;
    F32 q4_f32 = p_input_data_s->Q4_f32;

    /* Vector norm */
    F32 norm_f32;

    /* Objective function elements */
    F32 f1_f32, f2_f32, f3_f32;

    /* Objective function Jacobian elements */
    F32 J_11or24_f32, J_12or23_f32, J_13or22_f32, J_14or21_f32, J_32_f32, J_33_f32;

    F32 qDot1_f32, qDot2_f32, qDot3_f32, qDot4_f32;

    F32 hatDot1_f32, hatDot2_f32, hatDot3_f32, hatDot4_f32;

    /* Gyro bias error */
    F32 gyro_error_x_f32, gyro_error_y_f32, gyro_error_z_f32, gyro_bias_x_f32, gyro_bias_y_f32, gyro_bias_z_f32;

    /* Auxiliary variables to avoid repeated arithmetic */
    F32 half_q1_f32 = 0.5F * q1_f32;
    F32 half_q2_f32 = 0.5F * q2_f32;
    F32 half_q3_f32 = 0.5F * q3_f32;
    F32 half_q4_f32 = 0.5F * q4_f32;
    F32 two_q1_f32  = 2.0F * q1_f32;
    F32 two_q2_f32  = 2.0F * q2_f32;
    F32 two_q3_f32  = 2.0F * q3_f32;
    F32 two_q4_f32  = 2.0F * q4_f32;

    /* Normalize accelerometer measurement */
    norm_f32 = sqrt( accel_x_f32 * accel_x_f32 + accel_y_f32 * accel_y_f32 + accel_z_f32 * accel_z_f32 );

    /* Check for devision by zero */
    if( SMALL_NUMBER < norm_f32 )
    {
        norm_f32 = 1.0F / norm_f32;
        accel_x_f32 *= norm_f32;
        accel_y_f32 *= norm_f32;
        accel_z_f32 *= norm_f32;

        /* Compute the objective function and Jacobian */
        f1_f32 = two_q2_f32 * q4_f32 - two_q1_f32 * q3_f32 - accel_x_f32;
        f2_f32 = two_q1_f32 * q2_f32 + two_q3_f32 * q4_f32 - accel_y_f32;
        f3_f32 = 1.0F - two_q2_f32 * q2_f32 - two_q3_f32 * q3_f32 - accel_z_f32;
        J_11or24_f32 = two_q3_f32;
        J_12or23_f32 = two_q4_f32;
        J_13or22_f32 = two_q1_f32;
        J_14or21_f32 = two_q2_f32;
        J_32_f32 = 2.0F * J_14or21_f32;
        J_33_f32 = 2.0F * J_11or24_f32;

        /* Compute the gradient (matrix multiplication) */
        hatDot1_f32 = J_14or21_f32 * f2_f32 - J_11or24_f32 * f1_f32;
        hatDot2_f32 = J_12or23_f32 * f1_f32 + J_13or22_f32 * f2_f32 - J_32_f32 * f3_f32;
        hatDot3_f32 = J_12or23_f32 * f2_f32 - J_33_f32 *f3_f32 - J_13or22_f32 * f1_f32;
        hatDot4_f32 = J_14or21_f32 * f1_f32 + J_11or24_f32 * f2_f32;

        /* Normalize the gradient */
        norm_f32 = sqrt( hatDot1_f32 * hatDot1_f32 + hatDot2_f32 * hatDot2_f32 + hatDot3_f32 * hatDot3_f32 + hatDot4_f32 * hatDot4_f32 );
        hatDot1_f32 /= norm_f32;
        hatDot2_f32 /= norm_f32;
        hatDot3_f32 /= norm_f32;
        hatDot4_f32 /= norm_f32;

        /* Compute estimated gyroscope biases */
        gyro_error_x_f32 = two_q1_f32 * hatDot2_f32 - two_q2_f32 * hatDot1_f32 - two_q3_f32 * hatDot4_f32 + two_q4_f32 * hatDot3_f32;
        gyro_error_y_f32 = two_q1_f32 * hatDot3_f32 + two_q2_f32 * hatDot4_f32 - two_q3_f32 * hatDot1_f32 - two_q4_f32 * hatDot2_f32;
        gyro_error_z_f32 = two_q1_f32 * hatDot4_f32 - two_q2_f32 * hatDot3_f32 + two_q3_f32 * hatDot2_f32 - two_q4_f32 * hatDot1_f32;

        /* Compute and remove gyroscope biases */
        gyro_bias_x_f32 += gyro_error_x_f32 * delta_t_f32 * zeta_f32;
        gyro_bias_y_f32 += gyro_error_y_f32 * delta_t_f32 * zeta_f32;
        gyro_bias_z_f32 += gyro_error_z_f32 * delta_t_f32 * zeta_f32;

        /* Compute the quaternion derivative */
        qDot1_f32 = -half_q2_f32 * gyro_x_f32 - half_q3_f32 * gyro_y_f32 - half_q4_f32 * gyro_z_f32;
        qDot2_f32 =  half_q1_f32 * gyro_x_f32 + half_q3_f32 * gyro_z_f32 - half_q4_f32 * gyro_y_f32;
        qDot3_f32 =  half_q1_f32 * gyro_y_f32 - half_q2_f32 * gyro_z_f32 + half_q4_f32 * gyro_x_f32;
        qDot4_f32 =  half_q1_f32 * gyro_z_f32 + half_q2_f32 * gyro_y_f32 - half_q3_f32 * gyro_x_f32;

        /* Compute then integrate estimated quaternion derivative */
        q1_f32 += (qDot1_f32 -(beta_f32 * hatDot1_f32)) * delta_t_f32;
        q2_f32 += (qDot2_f32 -(beta_f32 * hatDot2_f32)) * delta_t_f32;
        q3_f32 += (qDot3_f32 -(beta_f32 * hatDot3_f32)) * delta_t_f32;
        q4_f32 += (qDot4_f32 -(beta_f32 * hatDot4_f32)) * delta_t_f32;

        /* Normalize the quaternion */
        norm_f32 = sqrt( q1_f32 * q1_f32 + q2_f32 * q2_f32 + q3_f32 * q3_f32 + q4_f32 * q4_f32 );
        norm_f32 = 1.0F / norm_f32;
        p_input_data_s->Q1_f32 = q1_f32 * norm_f32;
        p_input_data_s->Q2_f32 = q2_f32 * norm_f32;
        p_input_data_s->Q3_f32 = q3_f32 * norm_f32;
        p_input_data_s->Q4_f32 = q4_f32 * norm_f32;

        state_b = TRUE;
    }

    return state_b;

}




/* Accelerometer and gyroscope self test; check calibration wrt factory settings
   Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass */
PRIVATE BOOLEAN dd_icm_20600_self_test( DD_ICM_20600_DATA* p_input_data_s )
{
    BOOLEAN state_b = FALSE;
    U8      idx_u8;
    U8      register_data_vu8[4];
    U8      self_test_vu8[DD_ICM_20600_SELF_TEST_SIZE];
    F32     factory_trim_vu8[DD_ICM_20600_SELF_TEST_SIZE];

    do
    {
        /* Enable self test on all three axes and set accelerometer range to +/- 8 g */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_GYRO_CONFIG,
                                       0xF0 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Enable self test on all three axes and set gyro range to +/- 250 degrees/s */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_ACCEL_CONFIG,
                                       0xE0 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Delay a while to let the device execute the self-test */
        vTaskDelay(3 * portTICK_PERIOD_MS);

        /* X-axis, Y-axis, Z-axis and Mixed-axis self-test results */
        state_b = dd_i2c_read_burst( DD_ICM_20600_I2C_ADDR,
                                     DD_ICM_20600_SELF_TEST_X,
                                     register_data_vu8,
                                     sizeof(register_data_vu8) );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Extract the acceleration test results first */
        self_test_vu8[DD_ICM_20600_SELF_TEST_XA] = (register_data_vu8[0] >> 3) | (register_data_vu8[3] & 0x30) >> 4; // XA_TEST result is a five-bit unsigned integer
        self_test_vu8[DD_ICM_20600_SELF_TEST_YA] = (register_data_vu8[1] >> 3) | (register_data_vu8[3] & 0x0C) >> 2; // YA_TEST result is a five-bit unsigned integer
        self_test_vu8[DD_ICM_20600_SELF_TEST_ZA] = (register_data_vu8[2] >> 3) | (register_data_vu8[3] & 0x03) >> 0; // ZA_TEST result is a five-bit unsigned integer

        /* Extract the gyration test results first */
        self_test_vu8[DD_ICM_20600_SELF_TEST_XG] = register_data_vu8[0]  & 0x1F ; // XG_TEST result is a five-bit unsigned integer
        self_test_vu8[DD_ICM_20600_SELF_TEST_YG] = register_data_vu8[1]  & 0x1F ; // YG_TEST result is a five-bit unsigned integer
        self_test_vu8[DD_ICM_20600_SELF_TEST_ZG] = register_data_vu8[2]  & 0x1F ; // ZG_TEST result is a five-bit unsigned integer

        /* Process results to allow final comparison with factory set values */
        factory_trim_vu8[DD_ICM_20600_SELF_TEST_XA] = (4096.0F*0.34F) * (pow( (0.92F/0.34F) , ((self_test_vu8[DD_ICM_20600_SELF_TEST_XA] - 1.0F)/30.0F))); // FT[Xa] factory trim calculation
        factory_trim_vu8[DD_ICM_20600_SELF_TEST_YA] = (4096.0F*0.34F) * (pow( (0.92F/0.34F) , ((self_test_vu8[DD_ICM_20600_SELF_TEST_YA] - 1.0F)/30.0F))); // FT[Ya] factory trim calculation
        factory_trim_vu8[DD_ICM_20600_SELF_TEST_ZA] = (4096.0F*0.34F) * (pow( (0.92F/0.34F) , ((self_test_vu8[DD_ICM_20600_SELF_TEST_ZA] - 1.0F)/30.0F))); // FT[Za] factory trim calculation
        factory_trim_vu8[DD_ICM_20600_SELF_TEST_XG] = ( 25.0F*131.0F) * (pow( 1.046F , (self_test_vu8[DD_ICM_20600_SELF_TEST_XG] - 1.0F) ));               // FT[Xg] factory trim calculation
        factory_trim_vu8[DD_ICM_20600_SELF_TEST_YG] = (-25.0F*131.0F) * (pow( 1.046F , (self_test_vu8[DD_ICM_20600_SELF_TEST_YG] - 1.0F) ));               // FT[Yg] factory trim calculation
        factory_trim_vu8[DD_ICM_20600_SELF_TEST_ZG] = ( 25.0F*131.0F) * (pow( 1.046F , (self_test_vu8[DD_ICM_20600_SELF_TEST_ZG] - 1.0F) ));               // FT[Zg] factory trim calculation

        //  Output self-test results and factory trim calculation if desired
        //  Serial.println(self_test_vu8[0]); Serial.println(self_test_vu8[1]); Serial.println(self_test_vu8[2]);
        //  Serial.println(self_test_vu8[3]); Serial.println(self_test_vu8[4]); Serial.println(self_test_vu8[5]);
        //  Serial.println(factory_trim_vu8[0]); Serial.println(factory_trim_vu8[1]); Serial.println(factory_trim_vu8[2]);
        //  Serial.println(factory_trim_vu8[3]); Serial.println(factory_trim_vu8[4]); Serial.println(factory_trim_vu8[5]);

        /* Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
           To get to percent, must multiply by 100 and subtract result from 100 */
        for( idx_u8 = 0; idx_u8 < DD_ICM_20600_SELF_TEST_SIZE; idx_u8++ )
        {
            /* Store self test data */
            p_input_data_s->self_test_vf32[idx_u8] = self_test_vu8[idx_u8];

            /* Store factory trim data */
            p_input_data_s->factory_trim_vf32[idx_u8] = factory_trim_vu8[idx_u8];

            /* Calculate deviation of factory trim values in percent, +/- 14 or less deviation is a pass */
            p_input_data_s->fac_trim_deviation_vf32[idx_u8] = 100.0F + 100.0F * ( self_test_vu8[idx_u8] - factory_trim_vu8[idx_u8] ) / factory_trim_vu8[idx_u8]; // Report percent differences
        }
    }
    while(FALSE);

    return state_b;
}


