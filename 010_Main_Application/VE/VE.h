/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file VE.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef TS_TS_H_
#define TS_TS_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "Interface/ve_public_types.h"
#include "Interface/ve_grid_if.h"


/*************************************************************/
/*      GLOBAL DEFINES                                       */
/*************************************************************/

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/


/*********************************************************************/
/*   FUNCTION PROTOTYPES                                             */
/*********************************************************************/

/**
 * @details This function initializes the entire Vibration Engine (VE) domain
 * and shall by only called once.
 */
extern void ve_init( void );

/**
 * @details This function executes all Vibration Engine (VE) domain related
 * functions and shall be called in a cyclic fashion.
 */
extern void ve_main( void );

#endif /* TS_TS_H_ */
