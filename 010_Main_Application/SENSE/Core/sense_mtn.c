/*********************************************************************
    MODULE SPECIFICATION:

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file sense_mtn.c
        @details Some detailed description

*********************************************************************/


/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "sense_mtn.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      LOCAL VARIABLES                                              */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/



/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/
void sense_mtn_init( void )
{

}


void sense_mtn_main( void )
{

}

BOOLEAN sense_mtn_quaternion_update( SENSE_MTN_QUATERNION* p_quaternion_s,
                                     F32                   accel_x_f32,
                                     F32                   accel_y_f32,
                                     F32                   accel_z_f32,
                                     F32                   gyro_x_f32,
                                     F32                   gyro_y_f32,
                                     F32                   gyro_z_f32,
                                     F32                   delta_t_f32,
                                     F32                   zeta_f32,
                                     F32                   beta_f32 )
{

    /* Short name local variable for readability */
    F32 q1_f32 = p_quaternion_s->Q1_f32;
    F32 q2_f32 = p_quaternion_s->Q2_f32;
    F32 q3_f32 = p_quaternion_s->Q3_f32;
    F32 q4_f32 = p_quaternion_s->Q4_f32;

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
    if ( SMALL_NUMBER >= norm_f32 )
    {
        assert( SMALL_NUMBER >= norm_f32 );
        return FALSE;
    }
    else
    {
        norm_f32     = 1.0F / norm_f32;
        accel_x_f32 *= norm_f32;
        accel_y_f32 *= norm_f32;
        accel_z_f32 *= norm_f32;

        /* Compute the objective function and Jacobian */
        f1_f32       = two_q2_f32 * q4_f32 - two_q1_f32 * q3_f32 - accel_x_f32;
        f2_f32       = two_q1_f32 * q2_f32 + two_q3_f32 * q4_f32 - accel_y_f32;
        f3_f32       = 1.0F - two_q2_f32 * q2_f32 - two_q3_f32 * q3_f32 - accel_z_f32;
        J_11or24_f32 = two_q3_f32;
        J_12or23_f32 = two_q4_f32;
        J_13or22_f32 = two_q1_f32;
        J_14or21_f32 = two_q2_f32;
        J_32_f32     = 2.0F * J_14or21_f32;
        J_33_f32     = 2.0F * J_11or24_f32;

        /* Compute the gradient (matrix multiplication) */
        hatDot1_f32 = J_14or21_f32 * f2_f32 - J_11or24_f32 * f1_f32;
        hatDot2_f32 = J_12or23_f32 * f1_f32 + J_13or22_f32 * f2_f32 - J_32_f32 * f3_f32;
        hatDot3_f32 = J_12or23_f32 * f2_f32 - J_33_f32 * f3_f32 - J_13or22_f32 * f1_f32;
        hatDot4_f32 = J_14or21_f32 * f1_f32 + J_11or24_f32 * f2_f32;

        /* Normalize the gradient */
        norm_f32     = sqrt( hatDot1_f32 * hatDot1_f32 + hatDot2_f32 * hatDot2_f32 + hatDot3_f32 * hatDot3_f32 + hatDot4_f32 * hatDot4_f32 );
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
        q1_f32 += ( qDot1_f32 - ( beta_f32 * hatDot1_f32 ) ) * delta_t_f32;
        q2_f32 += ( qDot2_f32 - ( beta_f32 * hatDot2_f32 ) ) * delta_t_f32;
        q3_f32 += ( qDot3_f32 - ( beta_f32 * hatDot3_f32 ) ) * delta_t_f32;
        q4_f32 += ( qDot4_f32 - ( beta_f32 * hatDot4_f32 ) ) * delta_t_f32;

        /* Normalize the quaternion */
        norm_f32               = sqrt( q1_f32 * q1_f32 + q2_f32 * q2_f32 + q3_f32 * q3_f32 + q4_f32 * q4_f32 );
        norm_f32               = 1.0F / norm_f32;
        p_quaternion_s->Q1_f32 = q1_f32 * norm_f32;
        p_quaternion_s->Q2_f32 = q2_f32 * norm_f32;
        p_quaternion_s->Q3_f32 = q3_f32 * norm_f32;
        p_quaternion_s->Q4_f32 = q4_f32 * norm_f32;
    }

    return TRUE;
}
