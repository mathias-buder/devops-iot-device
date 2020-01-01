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

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "DD.h"

#include "Core/dd_i2c.h"
#include "Core/dd_icm-20600.h"
#include "Core/dd_database.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/


/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/

/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/


void dd_init(void)
{

    /* Initialize I2C basic device driver */
    dd_i2c_init();

    /* Initialize ICM-2600 motion subsystem */
    if( TRUE != dd_icm_20600_init() )
    {
        printf( "dd_icm_20600_init() failed with error: 0x%x\n", dd_i2c_get_error()->current_t );
    }
}


void dd_main(void)
{
    dd_icm_20600_main();
}
