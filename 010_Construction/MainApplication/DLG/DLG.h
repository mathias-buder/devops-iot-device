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

        @file DLG.h
        @details Some detailed description

*********************************************************************/
#ifndef DLG_H_
#define DLG_H_


/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dlg_log.h"

/*************************************************************
*      STRUCTURES                                            *
*************************************************************/
/**
 * @brief   Data Logging Input Structure
 * @details Contains all data that is required by the Device Driver (DD) domain
 * @ingroup LoggingStructures
 */
typedef struct LOG_DATA_IN_TYPE_TAG
{
    DLG_LOG_DATA_IN_TYPE log_data_in_s;
} LOG_DATA_IN_TYPE;


class DLG_C {
  private:
  public:
    static void init( void );
    static void main( const LOG_DATA_IN_TYPE &dlg_data_in_s );
};

#endif /* DLG_H_ */
