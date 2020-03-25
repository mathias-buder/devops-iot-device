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

        @file sense_mtn_if.h
        @details Some detailed description

*********************************************************************/

#ifndef SENSE_INTERFACE_MTN_IF_H_
#define SENSE_INTERFACE_MTN_IF_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../Core/sense_types.h"
#include "../Core/sense_mtn.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/*************************************************************/
/*      PROTOTYPES                                           */
/*************************************************************/
F32 sense_mtn_get_yaw( void );
F32 sense_mtn_get_pitch( void );
F32 sense_mtn_get_roll( void );

#endif /* SENSE_INTERFACE_MTN_IF_H_ */
