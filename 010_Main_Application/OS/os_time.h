/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file os_time.h
        @details Some detailed description

*********************************************************************/
#ifndef OS_TIME_H_
#define OS_TIME_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
class OS_TIME_C
{
  private:

  public:
    OS_TIME_C();
    ~OS_TIME_C();
    void update( void );
};

extern F32 os_time_stamp_ms_f32;

void os_time_init( void );
void os_time_update( void );

#endif /* OS_TIME_H_ */
