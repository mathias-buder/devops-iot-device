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




BOOLEAN dd_icm_20600_init(void)
{

    BOOLEAN state_b     = FALSE;
    U8      register_u8 = 0U;

    do
    {
        /* Clear sleep mode bit (6), enable all sensors */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_PWR_MGMT_1,
                                       0x00 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Delay 100 ms for PLL to get established on x-axis gyro;
         * should check for PLL ready interrupt */
        vTaskDelay(portTICK_PERIOD_MS);

        /* Get stable time source and set clock source to be PLL with x-axis
        * gyroscope reference, bits 2:0 = 001 */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_PWR_MGMT_1,
                                       0x01 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Configure gyroscope and accelerometer
        * Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively.
        * DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
        * Maximum delay is 4.9 ms which is just over a 200 Hz maximum rate */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_CONFIG,
                                       0x03 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Set sample rate = gyroscope output rate / ( 1 + SMPLRT_DIV ).
         * Use a 200 Hz rate as it was set in CONFIG above */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_SMPLRT_DIV,
                                       0x04 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Set gyroscope full scale range. Range selects FS_SEL and AFS_SEL are 0 - 3,
         * so 2-bit values are left-shifted into positions 4:3 */
        state_b = dd_i2c_read_single(  DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_GYRO_CONFIG,
                                      &register_u8 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Clear self-test bits [7:5] and AFS bits [4:3] */
        register_u8 &= ~0xF8;

        /* Set gyroscope scale range */
        register_u8 |= ( DD_ICM_20600_G_SCALSE << 3U );

        /* Write updated register value */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_GYRO_CONFIG,
                                       register_u8 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Set accelerometer configuration */
        state_b = dd_i2c_read_single(  DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_ACCEL_CONFIG,
                                      &register_u8 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Clear self-test bits [7:5] and AFS bits [4:3] */
        register_u8 &= ~0xF8;

        /* Set accelerometer scale range */
        register_u8 |= ( DD_ICM_20600_A_SCALSE << 3U );

        /* Write updated register value */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_ACCEL_CONFIG,
                                       register_u8 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Configure Interrupts and Bypass Enable. Set interrupt pin active high,
         * push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so
         * additional chips can join the I2C bus */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_INT_PIN_CFG,
                                       0x22 );

        /* Check for error during I2C operation */
        if( TRUE != state_b )
        {
            break;
        }

        /* Enable data ready (bit 0) interrupt */
        state_b = dd_i2c_write_single( DD_ICM_20600_I2C_ADDR,
                                       DD_ICM_20600_INT_ENABLE,
                                       0x01 );
    }
    while(FALSE);

    return state_b;
}


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
    F32     factory_trim_vf32[DD_ICM_20600_SELF_TEST_SIZE];

    /* Check NULL pointer */
    if( NULL != p_input_data_s )
    {
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
            factory_trim_vf32[DD_ICM_20600_SELF_TEST_XA] = (4096.0F*0.34F) * (pow( (0.92F/0.34F) , ((self_test_vu8[DD_ICM_20600_SELF_TEST_XA] - 1.0F)/30.0F))); // FT[Xa] factory trim calculation
            factory_trim_vf32[DD_ICM_20600_SELF_TEST_YA] = (4096.0F*0.34F) * (pow( (0.92F/0.34F) , ((self_test_vu8[DD_ICM_20600_SELF_TEST_YA] - 1.0F)/30.0F))); // FT[Ya] factory trim calculation
            factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZA] = (4096.0F*0.34F) * (pow( (0.92F/0.34F) , ((self_test_vu8[DD_ICM_20600_SELF_TEST_ZA] - 1.0F)/30.0F))); // FT[Za] factory trim calculation
            factory_trim_vf32[DD_ICM_20600_SELF_TEST_XG] = ( 25.0F*131.0F) * (pow( 1.046F , (self_test_vu8[DD_ICM_20600_SELF_TEST_XG] - 1.0F) ));               // FT[Xg] factory trim calculation
            factory_trim_vf32[DD_ICM_20600_SELF_TEST_YG] = (-25.0F*131.0F) * (pow( 1.046F , (self_test_vu8[DD_ICM_20600_SELF_TEST_YG] - 1.0F) ));               // FT[Yg] factory trim calculation
            factory_trim_vf32[DD_ICM_20600_SELF_TEST_ZG] = ( 25.0F*131.0F) * (pow( 1.046F , (self_test_vu8[DD_ICM_20600_SELF_TEST_ZG] - 1.0F) ));               // FT[Zg] factory trim calculation

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
                p_input_data_s->self_test_vu8[idx_u8] = self_test_vu8[idx_u8];

                /* Store factory trim data */
                p_input_data_s->factory_trim_vf32[idx_u8] = factory_trim_vf32[idx_u8];

                /* Calculate deviation of factory trim values in percent, +/- 14 or less deviation is a pass */
                p_input_data_s->fac_trim_deviation_vf32[idx_u8] = 100.0F + 100.0F * ( self_test_vu8[idx_u8] - factory_trim_vf32[idx_u8] ) / factory_trim_vf32[idx_u8];
            }
        }
        while(FALSE);
    }
    else
    {
        assert( NULL != p_input_data_s );
    }

    return state_b;
}




//// Function which accumulates gyro and accelerometer data after device initialization. It calculates the average
//// of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.
//void calibrateMPU6050(float * dest1, float * dest2)
//{
//  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
//  uint16_t ii, packet_count, fifo_count;
//  int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
//
//// reset device, reset all registers, clear gyro and accelerometer bias registers
//  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
//  wait(0.1);
//
//// get stable time source
//// Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
//  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);
//  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, 0x00);
//  wait(0.2);
//
//// Configure device for bias calculation
//  writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
//  writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
//  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);   // Turn on internal clock source
//  writeByte(MPU6050_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
//  writeByte(MPU6050_ADDRESS, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
//  writeByte(MPU6050_ADDRESS, USER_CTRL, 0x0C);    // Reset FIFO and DMP
//  wait(0.015);
//
//// Configure MPU6050 gyro and accelerometer for bias calculation
//  writeByte(MPU6050_ADDRESS, CONFIG, 0x01);      // Set low-pass filter to 188 Hz
//  writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
//  writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
//  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity
//
//  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
//  uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g
//
//// Configure FIFO to capture accelerometer and gyro data for bias calculation
//  writeByte(MPU6050_ADDRESS, USER_CTRL, 0x40);   // Enable FIFO
//  writeByte(MPU6050_ADDRESS, FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO  (max size 1024 bytes in MPU-6050)
//  wait(0.08); // accumulate 80 samples in 80 milliseconds = 960 bytes
//
//// At end of sample accumulation, turn off FIFO sensor read
//  writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
//  readBytes(MPU6050_ADDRESS, FIFO_COUNTH, 2, &data[0]); // read FIFO sample count
//  fifo_count = ((uint16_t)data[0] << 8) | data[1];
//  packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging
//
//  for (ii = 0; ii < packet_count; ii++) {
//    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
//    readBytes(MPU6050_ADDRESS, FIFO_R_W, 12, &data[0]); // read data for averaging
//    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
//    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
//    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;
//    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
//    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
//    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;
//
//    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
//    accel_bias[1] += (int32_t) accel_temp[1];
//    accel_bias[2] += (int32_t) accel_temp[2];
//    gyro_bias[0]  += (int32_t) gyro_temp[0];
//    gyro_bias[1]  += (int32_t) gyro_temp[1];
//    gyro_bias[2]  += (int32_t) gyro_temp[2];
//
//}
//    accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
//    accel_bias[1] /= (int32_t) packet_count;
//    accel_bias[2] /= (int32_t) packet_count;
//    gyro_bias[0]  /= (int32_t) packet_count;
//    gyro_bias[1]  /= (int32_t) packet_count;
//    gyro_bias[2]  /= (int32_t) packet_count;
//
//  if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
//  else {accel_bias[2] += (int32_t) accelsensitivity;}
//
//// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
//  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
//  data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
//  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
//  data[3] = (-gyro_bias[1]/4)       & 0xFF;
//  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
//  data[5] = (-gyro_bias[2]/4)       & 0xFF;
//
//// Push gyro biases to hardware registers
//  writeByte(MPU6050_ADDRESS, XG_OFFS_USRH, data[0]);
//  writeByte(MPU6050_ADDRESS, XG_OFFS_USRL, data[1]);
//  writeByte(MPU6050_ADDRESS, YG_OFFS_USRH, data[2]);
//  writeByte(MPU6050_ADDRESS, YG_OFFS_USRL, data[3]);
//  writeByte(MPU6050_ADDRESS, ZG_OFFS_USRH, data[4]);
//  writeByte(MPU6050_ADDRESS, ZG_OFFS_USRL, data[5]);
//
//  dest1[0] = (float) gyro_bias[0]/(float) gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
//  dest1[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
//  dest1[2] = (float) gyro_bias[2]/(float) gyrosensitivity;
//
//// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
//// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
//// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
//// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
//// the accelerometer biases calculated above must be divided by 8.
//
//  int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
//  readBytes(MPU6050_ADDRESS, XA_OFFSET_H, 2, &data[0]); // Read factory accelerometer trim values
//  accel_bias_reg[0] = (int16_t) ((int16_t)data[0] << 8) | data[1];
//  readBytes(MPU6050_ADDRESS, YA_OFFSET_H, 2, &data[0]);
//  accel_bias_reg[1] = (int16_t) ((int16_t)data[0] << 8) | data[1];
//  readBytes(MPU6050_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
//  accel_bias_reg[2] = (int16_t) ((int16_t)data[0] << 8) | data[1];
//
//  uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
//  uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis
//
//  for(ii = 0; ii < 3; ii++) {
//    if(accel_bias_reg[ii] & mask) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
//  }
//
//  // Construct total accelerometer bias, including calculated average accelerometer bias from above
//  accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
//  accel_bias_reg[1] -= (accel_bias[1]/8);
//  accel_bias_reg[2] -= (accel_bias[2]/8);
//
//  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
//  data[1] = (accel_bias_reg[0])      & 0xFF;
//  data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
//  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
//  data[3] = (accel_bias_reg[1])      & 0xFF;
//  data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
//  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
//  data[5] = (accel_bias_reg[2])      & 0xFF;
//  data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers
//
//  // Push accelerometer biases to hardware registers
////  writeByte(MPU6050_ADDRESS, XA_OFFSET_H, data[0]);
////  writeByte(MPU6050_ADDRESS, XA_OFFSET_L_TC, data[1]);
////  writeByte(MPU6050_ADDRESS, YA_OFFSET_H, data[2]);
////  writeByte(MPU6050_ADDRESS, YA_OFFSET_L_TC, data[3]);
////  writeByte(MPU6050_ADDRESS, ZA_OFFSET_H, data[4]);
////  writeByte(MPU6050_ADDRESS, ZA_OFFSET_L_TC, data[5]);
//
//// Output scaled accelerometer biases for manual subtraction in the main program
//   dest2[0] = (float)accel_bias[0]/(float)accelsensitivity;
//   dest2[1] = (float)accel_bias[1]/(float)accelsensitivity;
//   dest2[2] = (float)accel_bias[2]/(float)accelsensitivity;
//}

















