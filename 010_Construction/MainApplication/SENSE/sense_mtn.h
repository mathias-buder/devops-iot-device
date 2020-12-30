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

        @file sense_mtn.h
        @details Some detailed description

*********************************************************************/
#ifndef SENSE_MTN_H_
#define SENSE_MTN_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../types.h"

/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
#define SENSE_MTN_LOG_MSG_TAG              "SENSE_MTN"


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/



/*************************************************************
*       ENUMERATORS                                          *
*************************************************************/



/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/
/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup SenseStructures
 */
typedef struct SENSE_MTN_QUATERNION_TAG
{
    F32 Q1_f32;
    F32 Q2_f32;
    F32 Q3_f32;
    F32 Q4_f32;
} SENSE_MTN_QUATERNION;


/**
 * @brief   ICM-20600 Output Interface Data Structure
 * @details ICM-20600 Output Interface Data Structure gathers all required
            motion information such as pitch, roll, yaw, ...
 * @ingroup SenseStructures
 */
typedef struct SENSE_MTN_DATA_OUT_TAG
{
    SENSE_MTN_QUATERNION Quaternion_s;  /**< @details Acceleration raw data */
} SENSE_MTN_OUT_DATA;


/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/
class SENSE_MTN_C {
  private:

    BOOLEAN quaternion_update( SENSE_MTN_QUATERNION& quaternion_s,
                               F32                   accel_x_f32,
                               F32                   accel_y_f32,
                               F32                   accel_z_f32,
                               F32                   gyro_x_f32,
                               F32                   gyro_y_f32,
                               F32                   gyro_z_f32,
                               F32                   dt_f32,
                               F32                   zeta_f32,
                               F32                   beta_f32 );

  public:
    /**
       * @details Default constructor
       */
    SENSE_MTN_C( void );

    /**
       * @details Default destructor
       */
    ~SENSE_MTN_C();

    void init( void );
    void main( void );
};

#endif /* SENSE_CORE_MTN_H_ */
