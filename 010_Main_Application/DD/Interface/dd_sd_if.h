/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_sd_if.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_SD_IF_H
#define DD_SD_IF_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "esp_log.h"

#include "dd_public_types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
FILE*   dd_sd_create_file( const char* p_file_name_c );
FILE*   dd_sd_create_binary_file( const char* p_file_name_c );
BOOLEAN dd_sd_close_file( void );

#endif /* DD_SD_IF_H */
