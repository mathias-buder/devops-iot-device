/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_sd.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_SD_H
#define DD_SD_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "dd_types.h"


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
BOOLEAN dd_sd_init( void );
BOOLEAN dd_sd_deinit( void );
BOOLEAN dd_sd_open_file( const char*           p_file_name_c,
                         const DD_SD_FILE_MODE file_mode_e,
                         BOOLEAN               overwrite_b );

#endif /* DD_DATABASE_H */
