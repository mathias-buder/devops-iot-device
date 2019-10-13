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


/**
 * @file dd_icm-20600_Cfg.h
 * @details Some detailed description
 */

#ifndef DD_H
#define DD_H


/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "Interface/dd_public_types.h"
#include "Interface/dd_icm-20600_interface.h"
#include "Interface/dd_max-30102_interface.h"
#include "Interface/dd_xr-18910_interface.h"


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
 * @details This function initializes the entire Device Driver (DD) domain
 * and shall by only called once.
 */
extern void dd_init(void);


/**
 * @details This function executes all Device Driver (DD) domain related
 * functions and shall be called in a cyclic fashon.
 */
extern void dd_main(void);

#endif /* DD_H */
