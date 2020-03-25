/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file DD.c
        @details Some detailed description

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "DD.h"
#include "Core/dd_database.h"
#include "Core/dd_i2c.h"
#include "Core/dd_icm-20600.h"
#include "Core/dd_sd.h"


/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/
BOOLEAN file_written_b = FALSE;
U32     idx_u32        = 100U;
U32     time_in_ms_u32;
FILE*   p_file;
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
    if( FALSE == dd_icm_20600_init() )
    {
        ESP_LOGE( DD_LOG_MSG_TAG, "dd_icm_20600_init() failed with error: 0x%x\n", dd_i2c_get_error()->current_t );
    }

    /* Initialize SD card driver */
    dd_sd_init();
}

void dd_main(void)
{
    dd_icm_20600_main();
}
