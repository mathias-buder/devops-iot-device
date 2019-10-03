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

#ifndef DD_H
#define DD_H

#include "Interface/dd_icm-20600_interface.h"
#include "Interface/dd_max-30102_interface.h"
#include "Interface/dd_xr-18910_interface.h"
#include "Interface/dd_i2c_interface.h" /* shall be removed after testing, no external component shall be able to access it */

#include "Core/dd_i2c.h"


extern void dd_init(void);
extern void dd_main(void);

#endif /* DD_H */
