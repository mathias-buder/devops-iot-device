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

        @file sense_ts_if.h
        @details Some detailed description

*********************************************************************/

#ifndef SENSE_INTERFACE_TS_IF_H_
#define SENSE_INTERFACE_TS_IF_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "esp_log.h"

#include "sense_public_types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
extern SENSE_TS_DATA_TYPE * sense_ts_get_database( void );


#endif /* SENSE_INTERFACE_TS_IF_H_ */
