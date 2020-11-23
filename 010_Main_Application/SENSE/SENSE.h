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

        @file SENSE.h
        @details Some detailed description

*********************************************************************/
#ifndef SENSE_H_
#define SENSE_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "sense_ts.h"


/*************************************************************
*      STRUCTURES                                            *
*************************************************************/
/**
 * @brief   Device Driver Input Structure
 * @details Contains all data that is required by the Device Driver (DD) domain
 * @ingroup DriverStructures
 */
typedef struct SENSE_DATA_IN_TYPE_TAG
{
    SENSE_TS_DATA_IN_TYPE ts_data_in_s;
} SENSE_DATA_IN_TYPE;

/**
 * @brief   Device Driver Output Data Structure
 * @details Contains all data provided by the Device Driver (DD) domain
 * @ingroup DriverStructures
 */
typedef struct SENSE_DATA_OUT_TYPE_TAG
{
    const SENSE_TS_DATA_OUT_TYPE* p_ts_data_out_s;      /**< @details Pointer to Touch Sensor data structure */
} SENSE_DATA_OUT_TYPE;


/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/
class SENSE_C {

  private:
    static SENSE_DATA_OUT_TYPE sense_data_out_s; /**< @details Contains all data provided by the Device Driver (DD) domain */

  public:
    /**
     * @details This function initialized the SENSE domain
     */
    static void init( void );

    /**
     * @details This is the SENSE main function that shall be called
     * cyclicly and will provide all data through SENSE_DATA_OUT_TYPE
     * @param[in] data_in_s Reference to input structure
     * @return Copy of global domain output structure
     */
    static SENSE_DATA_OUT_TYPE main( SENSE_DATA_IN_TYPE &data_in_s );
};

#endif /* SENSE_H_ */
