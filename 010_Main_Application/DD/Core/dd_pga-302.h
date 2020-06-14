/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file dd_pga-302.h
        @details Some detailed description

*********************************************************************/

#ifndef DD_PGA_302_H
#define DD_PGA_302_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_types.h"

#include "../Config/dd_pga-302_cfg.h"
#include "../Config/dd_types_cfg.h"


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
BOOLEAN dd_pga_302_init( void );
void    dd_pga_302_main( void );

#endif /* DD_PGA_302_H */
