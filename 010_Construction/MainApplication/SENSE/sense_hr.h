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

        @file sense_hr.h
        @details Some detailed description

*********************************************************************/
#ifndef SENSE_HR_H_
#define SENSE_HR_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"

/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
#define SENSE_HR_LOG_MSG_TAG              "SENSE_HR"


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/



/*************************************************************
*       ENUMERATORS                                          *
*************************************************************/



/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/



/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/
class SENSE_HR_C {
  private:
  public:
    /**
       * @details Default constructor
       */
    SENSE_HR_C( void );

    /**
       * @details Default destructor
       */
    ~SENSE_HR_C();

    void init ( void );
    void main ( void );
};

#endif /* SENSE_HR_H_ */
